#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

#include "main.h"

void entrantPrintTagsForCheckpoints(struct entrant_t *pEntrant, struct config *pConfig, float* checkpoints);

int main(int argc, char** argv) {
    struct config c;
    struct config * pc = &c;
    configInit(pc);
    // parse input args
    configParseFromArgs(pc, argc, argv);
    configPrint(pc);

    srand(pc->seed);
    float checkpoints[pc->nCheckpoints];
    for (int j = 0; j < pc->nCheckpoints; ++j) {
        checkpoints[j] = pc->distance / (pc->nCheckpoints -1) * j;
    }

    struct entrant_t * entrants = malloc(pc->numEntrants * sizeof(struct entrant_t));

    for (int i = 0; i < pc->numEntrants; ++i) {
        entrantInit(&entrants[i],i);
        //entrantPrint(&entrants[i]);
    }

    int numRunning =pc->numEntrants;
    while(numRunning){
        numRunning =pc->numEntrants;
        for (int i = 0; i < pc->numEntrants; ++i) {
            if(entrants[i].position > checkpoints[pc->nCheckpoints-1] + pc->readRadiusInMiles){
                numRunning--;
            } else {
                // move them.
                entrantMove(&entrants[i],&pc->advanceRate);
                // get a tag read if near a checkpoint;
                entrantPrintTagsForCheckpoints(&entrants[i],pc,&checkpoints[0]);
            }
        }
    }

    free(entrants);
    return 0;
}

void entrantPrintTagsForCheckpoints(struct entrant_t * entrant, struct config *pc, float* checkpoints) {
    for (int j = 0; j < pc->nCheckpoints; ++j) {
        double distToCheckpoint =entrant->position - checkpoints[j];
        if(distToCheckpoint < pc->readRadiusInMiles && distToCheckpoint > -pc->readRadiusInMiles){
            // near a checkpoint, make sure we dont emit tags to frequently
            struct timeval timeDelta;
            timersub(&entrant->myTime, &entrant->lastTag, &timeDelta);
            if((entrant->lastTag.tv_sec == 0 && entrant->lastTag.tv_usec == 0)
               || timercmp(&timeDelta,&pc->maxFidelity,>=)){
                entrant->lastTag = entrant->myTime;
                entrantPrintTag(entrant, checkpoints[j]);
                break;
            }
        }
    }
}