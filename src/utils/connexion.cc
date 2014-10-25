/*************************************************************************
    > File Name: connexion.cc
    > Author: wyl
    > Mail: 
    > Created Time: Wed 22 Oct 2014 01:36:16 AM PDT
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <ctype.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#include<iostream>
using namespace std;

#include "options.h"

int ecrire(int fd, char *buf)
{
	int pos = 0;
	int count = strlen(buf);
	while(pos < count)
	{
		int i = write(fd, buf + pos, count - pos);
		if( i== -1)
		{
			if(errno != EINTR)
				pos = count + 1;
		}else
		{
			pos += i;
		}
	}
	return pos != count;
}


int ecrireBuff(int fd, char *buf, int count)
{
	int pos = 0;
	while(pos < count)
	{
		int i = write(fd, buf + pos, count - pos);
		if(i == -1)
		{
			switch(errno)
			{
				case EINTR:
					break;
				default:
					pos = count + 1;
					perror("Problem in ecrireBuff");
					break;
			}
		}
		else
		{
			pos += i;
		}
	}

	return pos != count;
}

int ecrireInt(int fd, int i)
{
	char buf[20];
	sprintf(buf, "%d", i);
	return ecrire(fd, buf);
}

int ecrireInt2(int fd, int i)
{
	char buf[20];
	sprintf(buf, "%d%c", i/10, i%10 + '0');
	return ecrire(fd, buf);
}

int ecrireInti(int fd, int i, char *buf)
{
	char buf[100];
	sprintf(buf, f, i);
	return ecrire(fd, buf);
}

int ecrireIntl(int fd, long i, char *buf)
{
	char buf[100];
	sprintf(buf, f, i);
	return ecrire(fd, buf);
}

/* Write a char on a fds
 *  * return 0 on success, 1 otherwise
 *   * Don't work on non-blocking fds...
 */
int ecrireChar(int fd, char c)
{
	int pos = 0;
	while(pos < 1)
	{
		int i = write(fd, &c, 1);
		if(i == -1)
		{
			if(errno != EINTR)
			{
				pos = 2;
			}
		}
		else
		{
			pos += i;
		}
	}
	return pos != 1;
}


