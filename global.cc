/*************************************************************************
    > File Name: global.cc
    > Author: wyl
    > Mail: 
    > Created Time: Sun 26 Oct 2014 07:26:20 AM PDT
 ************************************************************************/

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <adns.h>
#include <netdb.h>
#include <signal.h>
#include <ctype.h>
#include<iostream>

using namespace std;

#include "options.h"

#include "types.h"
#include "global.h"
#include "utils/text.h"
#include "utils/debug.h"
#include "fetch/site.h"
#include "interf/output.h"
#include "interf/inout.h"

time_t global::now;
hashTable *global::seen;
#ifdef NO_DUP
hashDup *global::hDuplicate;
#endif // NO_DUP
SyncFifo<url> *global::URLsPriority;
SyncFifo<url> *global::URLsPriorityWait;
uint global::readPriorityWait=0;
PersistentFifo *global::URLsDisk;
PersistentFifo *global::URLsDiskWait;
uint global::readWait=0;
IPSite *global::IPSiteList;
NamedSite *global::namedSiteList;
Fifo<IPSite> *global::okSites;
Fifo<NamedSite> *global::dnsSites;
Connexion *global::connexions;
adns_state global::ads;
uint global::nbDnsCalls = 0;
ConstantSizedFifo<Connexion> *global::freeConns;
#ifdef THREAD_OUTPUT
ConstantSizedFifo<Connexion> *global::userConns;
#endif
Interval *global::inter;
int8_t global::depthInSite;
bool global::externalLinks = true;
time_t global::waitDuration;
char *global::userAgent;
char *global::sender;
char *global::headers;
char *global::headersRobots;
sockaddr_in *global::proxyAddr;
Vector<char> *global::domains;
Vector<char> global::forbExt;
uint global::nb_conn;
uint global::dnsConn;
unsigned short int global::httpPort;
unsigned short int global::inputPort;
struct pollfd *global::pollfds;
uint global::posPoll;
uint global::sizePoll;
short *global::ansPoll;
int global::maxFds;
#ifdef MAXBANDWIDTH
long int global::remainBand = MAXBANDWIDTH;
#endif // MAXBANDWIDTH
int global::IPUrl = 0;

global::global(int argc, char *argv[])
{
	char *configFile = "larbin.conf";
#ifdef RELOAD
	bool reload = true;
#else
	bool reload = false;
#endif

	now = time(NULL);

	int pos = 1;
	while(pos < argc)
	{
		if(!strcmp(argv[pos], "-c") && argc > pos + 1)
		{
			configFile = argv[pos] + 1;
			pos += 2;
		}
		if(!strcmp(argv[pos], "-scratch"))
		{
			reload = false;
			pos ++;
		}else{
			break;
		}
	}

	if(pos != argc)
	{
		cerr << "usage : " << argv[0];
		cerr << "[-c configFile] [-scratch]\n";
		exit(0);
	}

	waitDuration = 60;
	depthInSite = 5;
	userAgent = "larbin";
	sender = "larbin@unspecified.mail";
	nb_conn = 20;
	dnsConn = 3;
	httpPort = 0;
	inputPort = 0;
	proxyAddr = NULL;
	domains = NULL;

	URLsDisk = new PersistentFifo(relload, fifoFile);
	URLsDiskWait = new PersistentFifo(reload, fifoFileWait);
	
	URLsPriority = new SyncFifo<url>;
	inter = new Iterval(ramUrls);
	namedSiteList = new IPSite[IPSiteListSize];
	okSites = new Fifo<IPSite>(2000);
	dnsSites = new Fifi<NamedSite>(2000);
	seen = new hashTable(!reload);
#ifdef NO_DUP
	hDuplicate = new hashDup(dupsize, dupFile, !reload);
#endif
	crash("Read the configureation file");
	ParseFile(configFile);
	crash("create global Values");

	LarbinString strtmp;
	strtmp.addString("\r\nUser-Agent: ");
	strtmp.addString(userAgent);
	strtmp.addString(" ");
	strtmp.addString(sender);
#ifdef SPECIFICSEARCH
	strtmp.addString("\r\nAccept:text/html");
	int i=0;
	while(contentTypes[i] != NULL)
	{
		strtmp.addString(", ");
		strtmp.addString(contentTypes[i]);
		++i;
	}
#elif !defined(IMAGES) && !defined(ANYTYPE)
	strtmp.addString("\r\nAccept: text/html");
#endif 
	strtmp.addString("\r\n\r\n");
	headers = strtmp.giveString();

	strtmp.recycle();
	strtmp.addString("\r\nUser-Agent: ");
	strtmp.addString(userAgent);
	strtmp.addString(" (");
	strtmp.addstring(sender);
	strtmp.addString(")\r\n\r\n");
	headersRobots = strtmp.giveString();

#ifdef THREAD_OUTPUT
	userConns = new ConstantSizedFifo<Connexion>(nb_conn);
#endif
	freeConns = new ConstantSizedFifo<Connexion>(nb_conn);
	connexions = new Connexion[nb_conn];
	for(uint i=0; i<nb_conn; ++i)
	{
		freeConns->put(connexions +i);
	}

	sizePoll = nb_conn + maxInput;

	pollfds = new struct pollfd[sizePoll];
	posPoll = 0;
	maxFds = sizePoll;
	ansPoll = new short[maxFds];

	adns_initflags flags = adns_initflags(adns_if_nosigpipe | ands_if_noerrprint);
	adns_init(&ads, flags, NULL);
	initSpecific();
	initInput();
	initOutput();
	initSite();

	static struct sigaction sn, so;
	sigemptyset(&sn.sa_mask);
	sn.sa_flags SA_RESTART;
	sn.sa_handler = SIG_IGN;
	if(sigaction(SIGPIPE, &sn, &so))
	{
		cerr << "Unable to disable SIGPIPE : " << strerror(errno) <<endl;
	}
}


