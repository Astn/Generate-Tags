//
// Created by astn on 3/2/18.
//

#ifndef GENERATERACEC99_CONFIG_H
#define GENERATERACEC99_CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

#define GETARG(A1,A2,R1,R2) if(i+1 < argc && strcmp(A1,argv[i])==0 || strcmp(A2,argv[i])==0){ R1 = R2(argv[++i]); }
#define IFARG(A1,A2,R1) if(i+1 < argc && strcmp(A1,argv[i])==0 || strcmp(A2,argv[i])==0) { R1 ; }

#define OneHourInSeconds 216000
#define OneSecondsInMicroSeconds 1000000

struct config{
    float readRadiusInMiles;// = 0.00189394;
    struct timeval maxFidelity;
    struct timeval advanceRate;
    int numEntrants;
    float distance;
    int nCheckpoints;
    unsigned int seed;
};

void configInit(struct config * c);
void configPrintHelp();
void configParseFromArgs(struct config *c, int argc, char **argv);
void configPrint(struct config * c);


#endif //GENERATERACEC99_CONFIG_H
