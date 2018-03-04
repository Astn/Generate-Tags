//
// Created by astn on 3/2/18.
//

#ifndef GENERATERACEC99_ENTRANT_H
#define GENERATERACEC99_ENTRANT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define MPHtoMilesPerSecond 0.000277778f
#define MPHtoMilesPerMicroSecond 2.77778e-10f

struct entrant_t{
    int id;
    struct timeval startAt;
    struct timeval lastTag;
    struct timeval myTime;
    float mph;
    int age;
    double position;
    char* gender;
};

void entrantInit(struct entrant_t * e, int id);
void entrantMove(struct entrant_t * e, struct timeval * delta);
void entrantPrintTag(struct entrant_t * e, double gatePosition);
void entrantPrint(struct entrant_t * e);

#endif //GENERATERACEC99_ENTRANT_H
