/*************************************************************************
    > File Name: mypthread.cc
    > Author: wyl
    > Mail: 
    > Created Time: Wed 22 Oct 2014 01:13:18 AM PDT
 ************************************************************************/

#include<iostream>
#include <stdlib.h>

using namespace std;

void startThread(StartFun run, void *arg)
{
	pthread_t t;
	pthread_attr_t attr;

	if(pthread_attr_init(&attr) != 0
		|| pthread_create(&t, &attr, run, arg) != 0
		|| pthread_attr_destory(&attr) != 0
		|| pthread_detach(t) != 0)
	{
		cerr << "Unable to launch a thread\n";
		exit(1);
	}
}
