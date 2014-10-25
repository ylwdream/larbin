/*************************************************************************
    > File Name: string.h
    > Author: wyl
    > Mail: 
    > Created Time: Mon 20 Oct 2014 01:17:02 AM PDT
 ************************************************************************/
#ifndef _STRING_H_
#define _STRING_H_
#include <assert.h>

#include "types.h"
#include "utils/debug.h"

class LarbinString{
private:
	char *chaine;
	uint pos;
	uint size;
public:
	LarbinString(uint size = STRING_SIZE);
	~LarbinString();

	void recycle(uint size=STRING_SIZE);
	//get the char*: it is deleted when you delete this String Objetc
	char *getString();
	// give a char *: it creates a new one : you mast delete it yourslef
	char *giveString();

	void addChar(char c);

	void addString(char *s);
	//append a buffer
	void addBuffer(char *s, uint len);

	inline uint getLength() {
		return pos;
	}

	inline char operator[](uint i) {
		assert(i <= pos)
			return chaine[i];
	}

	void setChar(uint i, char c);
}

#endif

