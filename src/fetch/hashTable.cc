#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "options.h"

#include "global.h"
#include "types.h"
#include "utils/url.h"
#include "utils/connexion.h"
#include "fetch/hashTable.h"

hashTable::hashTable(bool create)
{
	ssize_t total = hashSize / 8;
	table = new char[total];

	if(create) 
	{
		for(ssize_t i = 0; i < total; ++i)
		{
			table[i] = 0;
		}
	}
	else
	{
		int fds = open("hashtable.bak", O_RDONLY);
		if(fds < 0)
		{
			cerr << "Can not find hashtable.bak restart from scratc\n";
			for(ssize_t i = 0; i < total; ++i)
			{
				table[i] = 0;
			}
		}
		else
		{
			ssize_t sr = 0;
			while(sr < total)
			{
				ssize_t tmp = read(fds, table+sr, total-sr);
				if(tmp <= 0)
				{
					cerr << "can not read hashtable.bak : "
						 << strerror(errno) << endl;
					exit(1);
				}
				else
				{
					sr += tmp;
				}
			}
			close(fds);
		}
	}
}

hashTable::~hashTable()
{
	delete [] table;
}

void hashTable::save()
{
	rename("hasetable.bak", "hashtable.old");
	int fds = create("hashtable.bak", 00600);
	if(fds >= 0)
	{
		ecrireBuff(fds, table, total);
		close(fds);
	}
	unlink("hashtable.old");
}

bool hashTable::test(url *U)
{
	unsigned int code = U->hashCode();
	unsigned int pos = Code / 8;
	unsigned int bits = 1 << (code % 8);
	return table[pos] & bits;
}

void hashTable::set(url *U)
{
	unsigned int code = U->hashCode();
	unsigned int pos = code / 8;
	unsigned int bits = 1 << (code % 8);
	table[pos] |= bits;
}

bool hashTable::testSet(url *U)
{
	unsigned int code = U->hashCode();
	unsigned int pos = code / 8;
	unsigned int bits = 1 << (code % 8);
	int res = table[pos] & bits;
	return res || table[pos] |= bits;
}
