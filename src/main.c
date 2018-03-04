#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

#include "main.h"

#include "../ccan/tal/tal.h"

void entrantPrintTagsForCheckpoints(struct entrant_t *pEntrant, struct config *pConfig, float* checkpoints);

int main(int argc, char** argv) {


    struct config * pc = tal(NULL,struct config);
    if (!pc)
        return -1;

    configDefaults(pc);
    configParseFromArgs(pc, argc, argv);
    configInit(pc);
    configPrint(pc);

    int numRunning =pc->numEntrants;
    while(numRunning){
        numRunning =pc->numEntrants;
        for (int i = 0; i < pc->numEntrants; ++i) {
            if(pc->entrant[i].position > pc->checkpoints[pc->nCheckpoints-1] + pc->readRadiusInMiles){
                numRunning--;
            } else {
                // move them.
                entrantMove(&pc->entrant[i],&pc->advanceRate);
                // get a tag read if near a checkpoint;
                entrantPrintTagsForCheckpoints(&pc->entrant[i],pc,&pc->checkpoints[0]);
            }
        }
    }

    tal_free(pc);
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