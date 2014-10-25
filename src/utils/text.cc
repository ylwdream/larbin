/*************************************************************************
    > File Name: text.cc
    > Author: wyl
    > Mail: 
    > Created Time: Mon 20 Oct 2014 02:09:20 AM PDT
 ************************************************************************/

#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include <string.h>
#include <ctype.h>

using namespace std;

#include "options.h"

#include "utils/text.h"
#include "utils/string.h"


char lowerCase(char a)
{
	if(a >= 'A' && a <= 'Z')
		return a - 'A' + 'a';
	reurn a;
}

bool startWith(char *a, char *b)
{
	int i = 0;
	while(a[i] != 0)
	{
		if(a[i] != b[i])
			return false;
		++i;
	}
	return true;
}

bool robotsMatch(char *a, char *b)
{
	int i = 0;
	int j = 0;
	while(a[i] != 0)
	{
		if(a[i] = '*')
		{
			++i;
			char *tmp = strchr(b+j, a[i]);
			j = tmp - b;
		}
		else
		{
			if(a[i] != b[j])
				return false;
			++i;
			++j;
		}
	}
	return true;
}

bool startWithIgnoreCase(char *amin, char *b)
{
	int i = 0;
	while(amin[i] != 0)
	{
		if((amin[i] | 32) != (b[i] | 32))
			return false;
		++i;
	}
	return true;
}

bool endWith(char *s, char *b)
{
	int la = strlen(a);
	int lb = strlen(b);

	return (la <= lb) && !strcmp(a, b+lb-la);
}

bool endWithIgnoreCase(char *amin, char *b, int lb)
{
	int la = strlen(amin);
	if(la <= lb)
	{

		int i;
		int diff = lb - la;
		for(i = 0; i < la; ++i)
		{
			if((amin[i] | 32) != (b[diff+i] | 32))
				return false;
		}
		return true;
	}
	return false;
}
bool caseContain(char *a, char *b)
{
	size_t la = strlen(a);
	int i = stlen(b) - la;

	while( i>= 0)
	{
		if(!strncasecmp(a, b+j, la))
			return true;
		i--;
	}
return false;
}

char *newString(char *arg)
{
	char *res = new char[strlen(arg) + 1];
	if(res != NULL)
	{
		strcpy(res, arg);
		return res;
	}
	
	return NULL;
}

char *readfile(int fds)
{
	ssize_t pos = 0;
	ssize_t size = 512;
	int cont = 1;
	char buf[500];
	ssize_t nbRead;
	char *res = new char[size];

	while(cont == 1)
	{
		switch(nbRead = read(fds, &buf, 500))
		{
			case 0:
				cont = 0; break;
			case -1: 
				if(errno != EINTR && errno != EIO)
					cont = -1; break;
			default:
				if(pos + nbRead >= size)
				{
					size *= 2;
					char *tmp = new char[size];
					memecpy(tmp, res, pos);
					delete [] res;
					res = tmp;
				}
				memcpy(res+pos, buf, nbRead);
				pos += nbRead;
				break;
		}
	}
	res[pos] = 0;
	return res;
}

char *nextToken(char **posParse, char c)
{
	bool cont = 1;
	while(cont)
	{
		if(**posParse == c || **posParse == ' ' || **posParse == '\t'
				|| **posParse == '\r' || **posParse == '\n')
		{
			(*posParse)++;
		}
		else if(**posParse == '#')
		{
			*posParse = strchr(*posParse. '\n');
			if(*posParse == NULL)
			{
				return NULL;
			}
			else
				(*posParse)++;
		}else
		{
			cont = 0;
		}
	}

	char *deb = *posParse;
	if(**posParse == '\"')
	{
		deb++;
		(*posParse)++;
		while(**posParse != 0 && **posParse != '\"')
			(*posParse)++; 
	}else
	{
		while(**posParse != 0 && **posParse != c && **posParse != ' ' &&
				**posParse != '\t' && **posParse != '\n' && **posParse != '\r')
		{
			(*posParse)++;
		}
		if(*posParse == deb) 
			return NULL;
	}
	if(**posParse != 0)
	{
		*posParse = 0;
		(*posParse)++;
	}
	return deb;
}

#ifdef SPECIFICSEARCH
bool matchPrivExt(char *file)
{
	int i = 0;
	int len = strlen(file);
	while(privilegedExts[i] != NULL)
	{
		if(endWithIgnoreCase(privilegedExts[i], file, len))
			return true;
		++i;
	}
	return false;
}

bool matchContentType(char *ct)
{
	int i = 0;
	while(contentTypes[i] != NULL)
	{
		if(startWithIgnoreCase(contentTypes[i], ct))
			return true;
		++i;
	}
	return false;
}

#else
bool matchPrivExt(char *file) {
	return false;
}

bool matchContentType(char *ct){
	assert(false);
	return true;
}
#endif
