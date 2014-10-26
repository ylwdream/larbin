/*************************************************************************
    > File Name: global.h
    > Author: wyl
    > Mail: 
    > Created Time: Sun 26 Oct 2014 04:35:53 AM PDT
 ************************************************************************/

#ifndef GLOBAL_H
#define GLOBAL_H

#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>

#include <adns.h>

#include "fetch/file.h"
#include "fetch/hashTable.h"
#include "utils/hashDup.h"
#include "utils/url.h"
#include "utils/Vector.h"
#include "utils/string.h"
#include "utils/PersistentFifo.h"
#include "utils/ConstantSizedFifo.h"
#include "utils/SyncFifo.h"
#include "fetch/site.h"
#include "fetch/checker.h"

#define addIPUrl() global::IPUrl++
#define delIPUrl() global::IPUrl--

struct Connexion
{
	char state;
	int pos;
	FetchError err;
	int socket;
	int timeout;
	LarbinString request;
	file *parser;
	char *buffer[maxPageSize];

	Connexion();
	~Connexion();

	void recycle();
}

struct global
{
	global(int argc, char *argv[]);
	
	~global();

	static time_t now;

	static hashTable *seen;

#ifdef NO_DUP
	static hashDup *hDuplicate;
#endif

	static SyncFifo<url> *URLsPriority;
	static SyncFifo<rul> *URLsPorityWait;
	static uint readPriorityWait;

	static PersistentFifo *URLsDisk;
	static PersistentFifo *URLsDiskWait;
	static uint readWait;

	static NamedSite *namedSiteList;
	static IPSite *IPSiteList;

	static Fifo<IPSite> *okSites;

	static Fifo<NamedSite> *dnsSites;

	static Connexion *connexions;

	static adns_state ads;

	static uint nbDnsCalls;

	static ConstantSizedFifo<Connexion> * freeConns;

#ifdef THREAD_OUTPUT
	static ConstantSizedFifo<Connxion> *userConns;
#endif
	static Interval *inter;

	static int8_t depthInSite;
	static bool externalLinks;

	static time_t waitDuration;

	static char *userAgent;

	static char *sender;
	static char *headers;
	static char *headersRobots;
	static sockaddr_in *proxyAddr;

	static char getProxyFds(Connexion *conn);

	static Vector<char> *domains;

	static Vector<char> forbExt;

	static uint nb_conn;
	
	static uint dnsConn;
	
	static int IPUrl;

	static unsigned short int httpPort;

	static unsigned short int inputPort;

	static void parseFile(char *file);

	static void manageDomain(char **posParse);

	static struct pollfd *pollfds;

	static uint posPoll;

	static uint sizePoll;

	static short *ansPoll;

	static int maxFds;

	static void verifMAx(int fd);
#ifdef MAXBANDWIDTH
	static long int remainBand;
#endif



};

#define setPoll(fds, event) \
	global::pollfds[global::posPoll].fd = fds; \
	global::pollfds[global::posPoll].events = event; \
	global::posPoll++;

#endif
