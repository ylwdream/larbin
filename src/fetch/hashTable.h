/*************************************************************************
    > File Name: hashTable.h
    > Author: wyl
    > Mail: 
    > Created Time: Fri 17 Oct 2014 12:43:27 AM PDT
 ************************************************************************/
#ifndef _HASHTABLE_H
#define _HASHTABLE_H

#include "types.h"
#include "utils/url.h"
class hashTable
{
private:
	ssize_t size;
	char * table;

public:
	hashTable(bool create);
	~hashTable();
	/* save the hashTable in a file */
	void save();
	/* test if the url is allready in the hashTable 
	 * return true if it has been added
	 */
	bool test(url *U);
	
	/*set the url as present in the hashtable
	 */
	void set(url *U);

	bool testSet(url *U);
}

#endif

