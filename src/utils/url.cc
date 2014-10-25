#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "options.h"

#include "types.h"
#include "global.h"
#include "utils/url.h"
#include "utils/text.h"
#include "utils/connexion.h"
#include "utils/debug.h"

#ifdef COOKIES
#define initCookie() cookie=NULL;
#else
#define initCookie() ((void) 0)
#endif

static uint siteHashCode(char *host)
{
	uint h = 0;
	uint i = 0;
	while(host[i] != 0)
	{
		h = 37*h + host[i];
		i++;
	}

	return h % namedSiteListSize;
}

static int int_of_hexa(char c)
{
	if(c >= '0' && c <= '9')
		return (c - '0');
	else if( c >= 'a' && c <= 'f')
		return (c - 'a' + 10);
	else if( c >= 'A' && c <= 'F')
		return (c - 'A' + 10);
	else
		return -1;
}

bool fileNormalize (char *file) {
	int i=0;
	while (file[i] != 0 && file[i] != '#') {
		if (file[i] == '/') {
			if (file[i+1] == '.' && file[i+2] == '/') {
				// suppress /./
				int j=i+3;
				while (file[j] != 0) {
					file[j-2] = file[j];
					j++;
				}
				file[j-2] = 0;
			} else if (file[i+1] == '/') {
				// replace // by /
				int j=i+2;
				while (file[j] != 0) {
					file[j-1] = file[j];
					j++;
				}
				file[j-1] = 0;
			} else if (file[i+1] == '.' && file[i+2] == '.' && file[i+3] == '/') {
				// suppress /../
				if (i == 0) {
					// the file name starts with /../ : error
					return false;
				} else {
					int j = i+4, dec;
					i--;
					while (file[i] != '/') { i--; }
					dec = i+1-j; // dec < 0
					while (file[j] != 0) {
						file[j+dec] = file[j];
						j++;
					}
					file[j+dec] = 0;
				}
			} else if (file[i+1] == '.' && file[i+2] == 0) {
				// suppress /.
				file[i+1] = 0;
				return true;
			} else if (file[i+1] == '.' && file[i+2] == '.' && file[i+3] == 0) {
				// suppress /..
				if (i == 0) {
					// the file name starts with /.. : error
					return false;
				} else {
					i--;
					while (file[i] != '/') {
						i--;
					}
					file[i+1] = 0;
					return true;
				}
			} else { // nothing special, go forward
				i++;
			}
		} else if (file[i] == '%') {
			int v1 = int_of_hexa(file[i+1]);
			int v2 = int_of_hexa(file[i+2]);
			if (v1 < 0 || v2 < 0) return false;
			char c = 16 * v1 + v2;
			if (isgraph(c)) {
				file[i] = c;
				int j = i+3;
				while (file[j] != 0) {
					file[j-2] = file[j];
					j++;
				}
				file[j-2] = 0;
				i++;
			} else if (c == ' ' || c == '/') { // keep it with the % notation
				i += 3;
			} else { // bad url
				return false;
			}
		} else { // nothing special, go forward
			i++;
		}
	}
	file[i] = 0;
	return true;
}

url::url(char *u, int8_t depth, url *base)
{
	newUrl();
	this->depth = depth;
	host = NULL;
	port = 80;
	file = NULL;
	initCookie();
#ifdef URL_TAGS
	tag = 0;
#endif
	if(startWith("http:://", u)) 
	{
		parse(u+7);

		if(file != NULL && !normalize(file))
		{
			delete [] file;
			file = NULL;
			delete [] host;
			host = NULL;
		}
	}
	else if(base != NULL)
	{
		if(startWith("http:", u))
		{
			parseWithBase(u+5, base);
		}else if(isProtocol(u)){
			//unknown protocol
		}else {
			parseWithBase(u, base);
		}
	}
} 

url::url(char *line, int8_t depth)
{
	newUrl();
	this->depth = depth;
	host = NULL;
	port = 80;
	file = NULL;
	initCookie();
	int i = 0;
#ifdef URL_TAGS
	tag = 0;
	while(line[i] >= '0' && line[i] <= '9')
	{
		tag = 10 * tag + line[i] - '0';
		i++;
	}
	i++;
#endif

	if(startWith("http://", line+i))
	{
		parse(line+i+7);
		if (file != NULL && !normalize(file)) {
			delete [] file;
			file = NULL;
			delete [] host;
			host = NULL;
		}
	}
}

url::rul(char *line)
{
	newUrl();
	int i = 0;
	depth = 0;
	while(line[i] >= '0' && line [i] <= '9')
	{
		depth = 10*depth + line[i] - '0';
		i++;
	}
#ifdef URL_TAGS
	tag = 0; i++;
	while (line[i] >= '0' && line[i] <= '9')
	{
		tag = 10 * tag + line[i] - '0';
		i++;
	}
#endif
	int deb = ++i;
	while(line[i] != ':')
	{
		i++;
	}
	
	line[i] = 0;
	host = newString(line + deb);
	i++;
	
	//5 0 www.usc.edu:80/bus-affairs/admin_serv/uscard_serv/

	port = 0;
	while(line[i] >= '0' && line[i] <= '9')
	{
		port = 10*port + line[i] - '0';
	}

#ifdef COOKIES
	file = newString(line + i);
#else
	char *cpos = strchr(line+i, ' ');
	if (cpos == NULL)
	{
		cookie = NULL;
	} 
	else
	{
		*cpos = 0;
		cookie = new char[maxCookieSize];
		strcpy(cookie, cpos + 1);
	}
	file = newString(line + i);
#endif
}

url::url(char *host, uint port, char *file)
{
	newUrl();
	initCookie();
	this->host = host;
	this->port = port;
	this->file = file;
}

url::~url()
{
	delUrl();
	delete [] host;
	delete [] file;
#ifdef COOKIES
	delete [] cookie;
#endif
}

bool url::isValid()
{
	if(host == NULL) return false;
	int lh = strlen(host);
	return file != NULL && lh < maxSiteSize
			&& lh + strlen(file) + 18 < maxUrlSize;
}

void url::print()
{
	printf("http://%s:%u%s\n", host, port, file);
}

bool url::initOK(url *from)
{
#if defined(DEPTHBYSITE) || defined(COOKIES)
	if(strcmp(from->getHost(), host)) {
#ifdef DEPTHBYSITE
	depth = global::depthInSite;
#endif
} else{
#ifdef COOKIES
	if(from->cookie != NULL)
	{
		cookie = new char[maxCookieSize];
		strcpy(cookie, from->cookie);
	}
#endif
}
#endif

	if(depth < 0)
	{
		errno = tooDeep;
		return false;
	}
	NamedSite *ns = global::namedSiteList + (hostHashCode());
	if(!strcmp(ns->name, host) && ns->port == port)
	{
		switch (ns->dnsState)
		{
		case errorDns:
			errno = fastNoDns;
			return false;
		case noConnDns:
			errno = fastNoConn;
			return false;
		case doneDns:
			if(!ns->testRobots(file))
			{
				errno = fastRobots;
				return false;
			}	
		}
	}
	return true;
}

url *url::giveBase()
{
	int i = strlen(file);
	assert(file[0] == '/');
	while (file[i] != '/')
	{
		i--;
	}

	char *newFile = new char[i+2];
	memcpy(newFile, file, i+1);
	newFile[i+1] = '\0';

	return new url(newString(host), port, newFile);
}

char *url::giveUrl()
{
	
	int i = strlen(file);
	int j = strlen(host);
	
	// 7 + j + 1 + 9 + i + 1
	// http://(host):(port)(file)\0
	char *tmp = new char[18+i+j];

	strcpy(tmp, "http:://");
	strcpy(tmp+7, host);
	j += 7;
	
	if(port != 80)
	{
		j += sprintf(tmp + j, ":%u", port);
	}

	while (i >= 0)
	{
		tmp[j+i] = file[i];
		i--;
	}

	return tmp;
}

int url::writeUrl(char *buf)
{
	if (port == 80)
		return sprintf(buf, "http://%s%s", host, file);
	else
		return sprintf(buf, "http://%s:%u%s", host, port, file);
}

char *url::serialize() 
{
	//2 0 www.samba.gr.jp:80/doc/rfc1001.txt
	static char statstr[maxUrlSize + 40 + maxCookieSize];
	int pos = sprintf(statstr, "%u ", depth);
#ifdef URL_TAGS
	pos += sprintf(statstr+pos, "%u ", tag);
#endif
	pos += sprintf(statstr+pos, "%s:%u%s", host, port, file);
#ifdef COOKIES
	if(cookie != NULL)
	{
		pos += sprintf(statstr+pos, "%s", cookie);
	}
#endif
	statstr[pos] = "\n";
	statstr[pos+1] = 0;
	return statstr;
}

char *url::getUrl()
{
	static char statstr[maxUrlSize + 40];
	sprintf(statstr, "http://%s:%u%s", host, port, file);
	return statstr;
}

uint url::hostHashCode()
{
	return siteHashCode(host);
}

uint url::hashcode()
{
	unsigned int h = port;
	unsigned int i = 0;
	while(host[i] != 0)
	{
		h = 31 *h + host[i];
		++i;
	}
	i= 0;
	while(file[i] != 0)
	{
		h = 31 *h + file[i];
		++i;
	}

	return h % hashSize;
}

void url::parse(char *arg)
{
	int deb = 0, fin = deb;

	while(arg[fin] != '/' && arg[fin] != ':' && arg[fin] != 0)
	{
		++fin; 
	}
	if(fin == 0)
		return;
	//得到主机名字
	host = new char[fin+1];
	for(int i = 0; i < fin; ++i)
		host[i] = lowCase(arg[i]);
	host[fin] = '\0';

	//得到端口号
	if(arg[fin] == ':')
	{
		port = 0;
		fin++;
		while(arg[fin] >= '0' && arg[fin] <= '9')
		{
			port = port * 10 + arg[fin] - '0';
			++fin; 
		}
	}

	//得到主机上的文件名字
	if(arg[fin] != '/')
	{
		// www.inria.fr => add the final /
		file = newString("/");
	}
	else
	{
		//直到这个字符串的最后
		file = newString(arg + fin);
	}
}
//递归处理，也就是此url含有父亲url
// www.soumu.go.jp:80/kanku/chubu/uketsuke.html
//base  www.soumu.go.jp:80/kanku/
//在构造函数的时候被调用
void url::parseWithBase(char *u, url *base)
{
	if(u[0] == '/')
	{
		file = newString(u);
	}else
	{
		uint lenb = strlen(base->file);
		char *tmp = new char[lenb + strlen(u) + 1];
		memcpy(tmp, base->file, lenb);
		memcpy(tmp+lenb, u);
		file = tmp;
	}

	if(!normalize(file))
	{
		delete [] file;
		file = NULL;
		return;
	}
	host = newString(base->host);
	port = base->port;
}

bool url::normalize(char *file)
{
	return fileNormalize(file);
}

bool url::isProtocol(char *s)
{
	uint i = 0;
	while(isalnum(s[i]))
	{
		++i;
	}

	return s[i] == ':';
}


#ifdef COOKIES
#define addToCookie(s) len = strlen(cookie); \
	strncpy(cookie+len, s, maxCookieSize - len); \
	cookie[maxCookieSize - 1] = 0;

void url::addCookie(char *header)
{
	if(startWithIgnoreCase("set-cookie: ", header))
	{
		char * pos = strchr(header + 12, ';');
		if(pos != NULL)
		{
			int len;
			if(cookie == NULL)
			{
				cookie = nw char[maxCookieSize];
				cookie[0] = 0;
			}else
			{
				addToCookie("; ");
			}
			*pos = 0;
			addToCookie(header+12);
			*pos = ';';
		}
	}
}
#endif