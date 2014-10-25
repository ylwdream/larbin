/*************************************************************************
    > File Name: connexion.h
    > Author: wyl
    > Mail: 
    > Created Time: Wed 22 Oct 2014 01:31:06 AM PDT
 ************************************************************************/

#ifndef CONNEXION_H
#define CONNEXION_H

int ecrire(int fd, char *buf);

int encrireBuff(int fd, char *buf, int count);

int ecrireInt(int fs, int i);

int ecrireint2(int fd, int i);

int ecrireInti(int fd, int i, char *f);

int ecrireIntl(int fd, long i, char *f);

int ecrireLong(inf fd, long i);

int ecrireChar(int fd, char c);

#endif
