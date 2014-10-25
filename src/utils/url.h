/*************************************************************************
    > File Name: url.h
    > Author: wyl
    > Mail: 
    > Created Time: Fri 17 Oct 2014 05:47:49 AM PDT
 ************************************************************************/

#ifndef URL_H_
#define URL_H_

#include <netinet/in.h>
#include <sys/socket.h>
#include <stdlib.h>

#include "types.h"

bool fileNormalize(char *file);

class url
{
private:
	char *host;
	char *file;
	uint16_t port;
	int8_t depth;

	void parse(char *s);

	void parseWithBase(char *u, url *base);

	bool normalize(char *file);

	bool isProtocol(char *s);
	/* constructor used by giveBase */
	url(char *host, uint port, char *file);

public:
	
	/* constructor : parse an url (u is deleted)*/
	url(char *u, int8_t depth, url *base);

	url(char *line, int8_t depth);
	
	/* read the url from a file */
	url(char *line);

	~url();

	struct in_addr addr;

	bool isValid();

	void print();

	inline char *getHost() { return host;}

	inline uint getPort() {
		return port;
	}

	inline char *getFile(){
		return file;
	}

	inline int8_t getDepth(){
		return depth;
	}

	bool initOK(url * from);
	
	url *giveBase();

	char *givrUrl();
	
	/* write the url in a buffer
     * buf must be at least of size maxUrlSize
	 * returns the size of what has been written (not including '\0')
	*/
	int writeUrl(char *buf);
	
	/* serilaisze the url for the persistent Fifo*/
	char * serialize();

	char *getUrl();

	uint hostHashCode();

	uint hashCode();

#ifdef URL_TAGS
	uint tag;
#endif

#ifdef COOKIES
	char *cookie;
	void addCookie(char *header);
#else
	inline void addCookie(char * header) {}
#endif
};
#endif
