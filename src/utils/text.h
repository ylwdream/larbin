/*************************************************************************
    > File Name: text.h
    > Author: wyl
    > Mail: 
    > Created Time: Mon 20 Oct 2014 01:57:38 AM PDT
 ************************************************************************/

#ifndef _TEXT_H_
#define _TEXT_H_

#include "utils/string.h"

char lowerCase(char a);

//test if b starts with a
bool startWith(char *a, char *b);

//test if b is forbidden by pattern a
boll robotsMatch(char *a, char *b);

//test if b start with a ignoring case
bool startWithlgnoreCase(char *a, char *b);

bool endWith(char *a, char *b);

bool endWithLgnoreCase(char *amin, char *b, int lb);

//test if b contains a
bool caseContain(char *a, char *b);

//create a copy if a string
char *newString(char *arg);

//read a whole file
char *readfile(int fds);

char *nextToken(char **posParse, char c = ' ');

bool matchPrivExt(char *file);

bool matchContentType(char *ct);
#endif
