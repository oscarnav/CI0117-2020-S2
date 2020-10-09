#ifndef DNA_H
#define DNA_H

#include <pthread.h>


static const char DNA1[] = {'a','a','k','k','p','o','p','k','k','b','r','w','w','w'};
static const char DNA2[] = {'k','k','k','k','m','b','b','n','o','o','o','n','w','w','w'};
static const int DNA1_len = 14; 
static const int DNA2_len = 15; 

void* ocurSecuencia1(void* args);
void* ocurSecuencia2(void* args);
int main(int argc, char* arg[]);

#endif