/*************************************************************************
    > File Name: types.h
    > Author: wyl
    > Mail: 
    > Created Time: Fri 17 Oct 2014 05:18:04 AM PDT
 ************************************************************************/

#ifndef _TYPES_H
#define _TYPES_H

#define hashSize 64000000

#define dupSize hashSize
#define dupFile "dupfile.bak"

#define namedSiteListSize 20000
#define IPSiteListSize 10000

#define ramUrls 100000
#define maxIPUrls 80000

#define maxUrlsBySite 40

#define timeoutPage 30 
#define timeoutIncr 2000

#define dnsValidTime 2*24*3600

#define maxPageSize 100000
#define nearlyFullPage 90000

#define maxRobotsSize 10000

#define maxRobotsItem 100

#define fifoFile "fifo"
#define fifoFileWait "fifowait"

#define urlByFile ramUrls

#define BUF_SIZE 16384
#define STRING_SIZE 1024

#define maxUrlSize 512
#define maxSiteSize 40

#define maxCookieSize 128

#define stdVectSize maxRobotsItem

#define maxIput 5

#define filesPerDir 2000
#define saveDir save/
#define indexFile "index.html"
#define nbDir 1000

#define specDir "specific"
#define maxSpecSize 5000000

#define nbAnswers 16

enum FetchError
{
	success,
	noDNS,
	noConnection,
	forbiddenRobots,
	timeout,
	badType,
	tooBig,
	err30X,
	err40X,
	earlyStop,
	duplicate,
	fastRobots,
	fastNoConn,
	fastNoDns,
	tooDeep,
	urlDup
};


typedef unsigned int uint;

#endif
