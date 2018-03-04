//
// Created by astn on 3/2/18.
//

#include "entrant_t.h"

void entrantInit(struct entrant_t * e, int id){
    e->id = id;
    e->position = 0.0;
    e->lastTag.tv_usec = 0;
    e->lastTag.tv_sec = 0;
    __timezone_ptr_t tpz = NULL;
    gettimeofday(&e->startAt,tpz);
    e->myTime.tv_usec = e->startAt.tv_usec;
    e->myTime.tv_sec = e->startAt.tv_sec;
    e->mph = (float)( rand() % 4 + 5 + drand48() - 0.5);
    e->age = rand() % 60 + 10;
    if(rand() % 2 > 0)
        e->gender = "m";
    else
        e->gender = "f";
}


void entrantMove(struct entrant_t * e, struct timeval * delta){
    double secondMove = 0;
    double microMove = 0;

    secondMove = delta->tv_sec * (e->mph * MPHtoMilesPerSecond);
    microMove = delta->tv_usec * (e->mph * MPHtoMilesPerMicroSecond);

    e->position += secondMove + microMove;
    struct timeval tadd;
    timeradd(&e->myTime, delta, &tadd);
    e->myTime = tadd;
}

void entrantPrint(struct entrant_t * e){
    printf("{ \"id\": %i, \"startAt\": { \"sec\": %lu \"usec\": %lu }, \"mph\": %f, \"age\": %i, \"gender\": \"%s\"}\n",
           e->id,
           e->startAt.tv_sec,
           e->startAt.tv_usec,
           e->mph,
           e->age,
           e->gender);
}

void entrantPrintTag(struct entrant_t * e, double gatePosition){
    struct tagRead{
        int id;
        struct timeval tagTime;
        double tagPosition;
        double gatePosition;
    };
    printf("{ \"id\": %i, \"chipTime\": { \"sec\": %lu \"usec\": %lu }, \"tagPosition\": %f, \"gatePosition\": %f}\n",
           e->id,
           e->myTime.tv_sec  - e->startAt.tv_sec,
           e->myTime.tv_usec / 1000, // to millis
           e->position,
           gatePosition
    );
}