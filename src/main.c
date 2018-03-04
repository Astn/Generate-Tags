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
    if(configParseFromArgs(pc, argc, argv) < 0) {
        return -1;
    }

    configInit(pc);

    if(pc->verbose){
        configPrint(pc);
        for (int i = 0; i < pc->nEntrant; ++i) {
            entrantPrint(&pc->entrant[i]);
        }
    }

    if(pc->exit){
        return 0;
    }


    int numRunning =pc->nEntrant;
    while(numRunning){
        numRunning =pc->nEntrant;
        for (int i = 0; i < pc->nEntrant; ++i) {
            if(pc->entrant[i].position > pc->checkpoints[pc->nCheckpoint-1] + pc->readRadiusInMiles){
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
    struct timeval timeDelta;
    for (int j = 0; j < pc->nCheckpoint; ++j) {
        double distToCheckpoint =entrant->position - checkpoints[j];
        if(distToCheckpoint < pc->readRadiusInMiles && distToCheckpoint > -pc->readRadiusInMiles){
            // near a checkpoint, make sure we dont emit tags to frequently
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