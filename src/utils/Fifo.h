/*************************************************************************
    > File Name: Fifo.h
    > Author: wyl
    > Mail: 
    > Created Time: Sat 25 Oct 2014 05:11:33 AM PDT
 ************************************************************************/

#ifndef FIFO_H
#define FIFO_H

#include "types.h"

class Fifo
{
public:
	uint in, out;
	uint size;
	T **tab;

	Fifo(uint size=maxUrlsBySite);

	~Fifo();

	inline T* read(){
		return tab[out];
	}

	T *tryRead();
	T *get();
	T *tryGet();

	void put(T *obj);

	void rePut(T *obj);

	int getLength();

	inline bool isEmpty(){
		return in = out;
	}
};

template <class T>
Fifo<T>::Fifo(uint size)
{
	tab = new T*[size];
	this->size = size;
	in = 0;
	out = 0;
}

template <class T>
Fifo<T>::~Fifo()
{
	for(uint i = out, i <= in; ++i)
	{
		if((i+1)==size)
			i = 0;
		delete tab[i];
	}
	delete[] tab;

}

template <class T>
T *Fifo<T>::tryRead()
{
	if(in == out)
	{
		return NULL;
	}else
	{
		return tab[out];
	}
}

template <class T>
T *Fifo<T>::get()
{
	T *tmp;
	assert(in != out);
	tmp = tab[out];
	out = (out + 1) % size;
	return tmp;
}

template <class T>
T *Fifo<T>::tryGet()
{
	T *tmp = NULL;
	if(in != out)
	{
		tmp = tab[out];
		out = (out + 1) % size;
	}

	return tmp;
}

template <class T>
void Fifo<T>::put(T *obj)
{
	tab[in] = obj;
	in = (in + 1) % size;
	if(in == out)
	{
		T **tmp;
		tmp = new T*[2*size];
		for(uint i=out; i<size; ++i)
		{
			tmp[i] = tab[i];
		}
		for(uint i=0; i<in; ++i)
		{
			tmp[i+size] = tab[i];
		}
		in += size;
		size *= 2;
		delete [] tab;
		tab = tmp;
	}
}

template <class T>
void Fifo<T>::rePut(T *obj)
{
	out = (out + size - 1) % size;
	tab[out] = obj;
}

template<class T>
int Fofi<T>::getLength()
{
	return (in + size - out) % size;
}

#endif
