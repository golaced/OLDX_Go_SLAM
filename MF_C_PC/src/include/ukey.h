#ifndef __UKEY_H__
#define  __UKEY_H__
#include <stdlib.h>
#include <stdio.h>
typedef unsigned short     int u16;
typedef struct 
{
	int state;//1-> good  2->wrong lisence
	char id[3];//board id
	char lis[20];//your lisence
}LIS;
extern LIS license;
char set_lisence(char *in);
#endif