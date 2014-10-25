/*************************************************************************
    > File Name: string.cc
    > Author: wyl
    > Mail: 
    > Created Time: Mon 20 Oct 2014 01:38:36 AM PDT
 ************************************************************************/
#include <string.h>
#include<iostream>

using namespace std;

#include "options.h"

#include "utils/text.h"
#include "utils/string.h"

LarbinString::LarbinString(uint size)
{
	chaine = new char[size];
	this->size = size;
	pos = 0;
	chaine[0] = 0;
}

LarbinString::~LarbinString()
{
	delete [] chaine;
}

void LarbinString::recycle(uint size)
{
	if(this->size > size)
	{
		delete [] chaine;
		chaine = new char[size];
		this->size = size;
	}

	pos = 0;
	chaine[0] = 0;
}

char *LarbinString::getString()
{
	return chaine;
}

char *LarbinString::giveString()
{
	return newString(chaine);
}

void LarbinString::addChar(char c)
{
	chaine[pos] = c;
	++pos;
	if(pos >= size)
	{
		char *tmp = new char[size * 2];
		memcpy(tmp, chaine, pos);
		delete [] chaine;
		chaine = tmp;
		size *= 2;
	}

	chaine[pos] = 0;
}

void LarbinString::addString(char *s)
{
	uint len = strlen(s);
	addBuffer(s, len);
}

void LarbinString::addBuffer(char *s, uint len)
{
	if(size <= pos + len)
	{
		size *= 2;
		if(size <= pos + len) size = pos + len + 1;
		char *tmp = new char[size];
		memcpy(tmp, chaine, pos);
		delete [] chaine;
		chaine = tmp;
	}
	memcpy(chaine+pos, s, len);
	pos += len;
	chaine[pos] = 0;
}

void LarbinString::setChar(uint i, char c)
{
	chaine[i] = c;
}
