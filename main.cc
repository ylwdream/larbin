// wyl
// 14-10-15

#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "options.h"

#include "global.h"

static void cron();

#ifdef PROF

static bool stop = false;
static void handler(int i)
{
	stop = true;
}
#endif

#ifdef MAXBANDWIDTH

static void waitBandwidth(time_t *old)
{
	while(global::remainBand < 0)
	{

		poll(NULL, 0, 10);
		global::now = time(NULL);
		if(*old != global::now)
		{
			*old = global::now;
			cron();
		}
	}
}

#else
#define waitBandwidth(x) ((void) 0)
#endif

#ifndef NDEBUG
static uint count = 0;
#endif


int main(int argc, char *argv[])
{
	global glob(argc, argv);
#ifdef	PROF
	signal(2, handler);
#endif

#ifndef NOWEBSERVER
	if(global::httpPort != 0)
		startThread(startWebserver, NULL);
#endif

	printf("%s is starting its serach\n", global::userAgent);
	time_t old = global::now;

	for(;;)
	{
		global::now = time(NULL);
		if(old != global::now)
		{
			old = global::now;
			cron();
		}

		stateMain(-count);
		waitBandwidth(&old);
		stateMain(1);
		for(int i=0; i<global::maxFds; ++i)
			global::ansPoll[i] = 0;
		for(uint i=0; i<global::posPoll; ++i)
			global::ansPoll[global::pollfds[i].fd] = global::pollfds[i].revents;
		global::posPoll = 0;
		stateMain(2);
		input();
		stateMain(3);
		sequencer();
		stateMain(4);
		fetchDns();
		stateMain(5);
		fetchOpen();
		stateMain(6);
		checkAll();

		stateMain(count++);
		poll(global::pollfds, global::posPoll, 10);
		stateMain(7);
	}
}

static void cron()
{
#ifdef PROF
	if(stop) exit(2);
#endif
#ifdef EXIT_AT_END
	if(global::URLsDisk->getLength() == 0
		&& global::URLsDiskWait->getLength() == 0
		&& debUrl == 0)
		exit(0);
#endif

chechTimeOut();

	if((global::now % 300) == 0)
	{
		global::readPriorityWait = global::URLsPriorityWait->getLength();
		global::readWait = global::URLsDiskWait->getLength();
	}
	if((global::now % 300) == 150)
	{
		global::readPriorityWait = 0;
		global::readWait = 0;
	}

#ifdef MAXBANDWIDTH
	if(global::remainBand > 0)
	{
		global::remainBand = MAXBANDWIDTH;
	}
	else
	{
		global::remainBand += MAXBANDWIDTH;
	}
#endif

#ifdef NOSTATS
	histoHit(pages, answers[success]);

	if((global::now & 7) == 0)
	{
		urlsRate = (urls - urlsPrev) >> 3; urlsPrev = urls;
		pagesRate = (pages - pagesPrev) >> 3; pagesPrev = pages;
		successRate = (answer[success] - successPrev) >> 3;
		successPrev = answers[success];
		siteSeenRate = (siteSeen - siteSeenPrev) >> 3; siteSeenPrev = siteSeen;
		siteDNSRate = (siteDNS - siteDNSPrev) >> 3; siteDNSPrev = siteDNS;
#ifndef NDUBUG
	readRate = (byte_read - readPrev) >> 3; readPrev = byte_read;
	writeRate = (byte_write - writePrev) >> 3; writePrev = byte_write;
#endif

#ifdef STATS
	printf("\n%surls : %d (rate : %d)\npages : %d (rate : %d)\nsuccess : %d (rate : %d)\n", ctime(&global::now),
			urls, urlsRate, pages, pagesRate, answers[success], successRate);
#endif
	}
#endif
}
