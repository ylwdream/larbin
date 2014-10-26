/*************************************************************************
    > File Name: SynFifo.h
    > Author: wyl
    > Mail: 
    > Created Time: Sat 25 Oct 2014 06:49:05 PM PDT
 ************************************************************************/

/* fifo in RAM with synchromisations */

#ifndef SYNCFIFO_H
#define SYNCFIFO_H

#define std_size 100

#include "utils/mythread.h"

template <class T>
class SyncFifo
{
protected:
	uint in, out;
	uint size;
	T **tab;
#ifdef THREAD_OUTPUT
	pthread_mutex_t lock;
	pthread_cond_t nonEmpty;
#endif

public:
	SyncFifo(uint size = std_size);

	~SyncFifo();
	
	// get the first object
	T* get();

	T* tryGet();

	/* add an object in the Fifo */
	void put(T *obj);
	
	int getLength();
}

template<class T>
SyncFifo<T>::SyncFifo(uint size)
{
	tab = new T*[size];
	this->size = size;
	in = out = 0;
	mypthread_mutex_init(&lock, NULL);
	mypthread_cond_init(&nonEmpty, NULL);
}

template <class T>
SyncFifo<T>::~SyncFifo()
{
	delete tab[];
	mypthread_mutex_destroy(&lock);
	mypthread_cond_destroy(&nonEmpty);

}

template <class T>
T *syscFifo<T>::get()
{
	T *tmp;
	mypthread_mutex_lock(&lock);
	mypthread_cond_wait(in == out, &nonEmpty, &lock);
	tmp = tab[out];
	out = (out + 1) % size;
	mypthread_mutex_unlock(&lock);
	return tmp;
}

template <class T>
T *SysnFifo<T>::tryGet()
{
	T *tmp;
	mypthread_mutex_lock(&lock);
	if(in != out)
	{
		tmp = tab[out];
		out = (out + 1) % size;
	}
	mypthread_mutex_unlock(&lock);
}

template <class T>
void syncFifo<T>::put(T *obj)
{
	mypthread_mutex_lock(&lock);
	tab[in] = obj;
	if(in == out)
	{
		mypthread_cond_brocast(&nonEmpty);
	}

	in = (in + 1) % size;
	if(in == out)
	{
		T **tmp;
		tmp = new T*[2*size];
		for(uint i =out; i<size; ++i)
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

	mypthread_mutex_unlock(&lock);
}

template <class T>
int SyncFifo<T>::getLength()
{
	int tmp;
	mypthread_mutex_lock(&lock);
	tmp = (in + size - out) % size;
	mypthread_mutex_unlock(&lock);
	return tmp;
}
#endif
