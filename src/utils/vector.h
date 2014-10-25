/*************************************************************************
    > File Name: vector.h
    > Author: wyl
    > Mail: 
    > Created Time: Thu 23 Oct 2014 05:41:09 PM PDT
 ************************************************************************/
#ifndef VECTOR_HH
#define VECTOR_HH

#include "types.h"
#include <assert.h>

template <class T>
class Vector
{
private:
	T **tab;
	uint pos;
	uint size;
public:
	Vector<uint size = StdVectSize>;
	~Vector();
	void recycle();
	void addElement(T *elt);
	inline uint getLength() {
		return pos;
	}
	inline T** getTab(){
		return tab;
	}
	T *operator[] (uint i);
};

template <class T>
Vector<T>::Vector(uint size)
{
	this->size = size;
	pos = 0;
	tab = new T*[size];
}

template<class T>
Vector<T>::~Vector()
{
	for(uint i=0; i < pos; ++i)
	{
		delete tab[i];
	}
	delete [] tab;
}

template<class T>
void Vector<T>::recycle()
{
	for(uint i=0; i < pos; ++i)
	{
		delete tab[i];
	}

	pos = 0;
}

template <class T>
void Vector<T>::addElement(T *elt)
{
	assert(pos <= size);
	if(pos == size)
	{
		size *= 2;
		T **tmp = new T*[size];

		for(uint i=0; i < pos; ++i)
		{
			temp[i] = tab[i];
		}
		delete [] tab;
		tab = tmp;
	}
	tab[pos] = elt;
	pos++;
}

template <class T>
T *Vector<T>:: operator[](uint i)
{
	if(i < pos)
	{
		return tab[i];
	}else
	{
		return NULL;
	}
}

#endif
