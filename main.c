#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

struct entrant{
    int id;
    struct timeval startAt;
    struct timeval lastTag;
    struct timeval myTime;
    float mph;
    int age;
    double position;
    char* gender;
};
void entrantInit(struct entrant * e, int id){
    e->id = id;
    e->position = 0.0;
    e->lastTag.tv_usec = 0;
    e->lastTag.tv_sec = 0;
    __timezone_ptr_t tpz = NULL;
    gettimeofday(&e->startAt,tpz);
    e->myTime.tv_usec = e->startAt.tv_usec;
    e->myTime.tv_sec = e->startAt.tv_sec;
    e->mph = (float)( rand() % 3 + 4 + drand48() - 0.5);
    e->age = rand() % 60 + 10;
    if(rand() % 2 > 0)
        e->gender = "male";
    else
        e->gender = "female";
}
const int OneHourInSeconds = 216000;
const int OneSecondsInMicroSeconds = 1000000;
void entrantMove(struct entrant * e, struct timeval * delta){
    double secondMove = 0;
    double microMove = 0;

    secondMove = delta->tv_sec * (e->mph * 0.000277778);
    microMove = delta->tv_usec * (e->mph * 2.77778e-10);

    e->position += secondMove + microMove;
    struct timeval tadd;
    timeradd(&e->myTime, delta, &tadd);
    e->myTime = tadd;
}
void entrantPrint(struct entrant * e){
    printf("{ id: %i, startAt: %lu.%lu, mph: %f, age: %i, gender: %s}\n",
     e->id,
     e->startAt.tv_sec,
     e->startAt.tv_usec,
     e->mph,
     e->age,
     e->gender);
}
void entrantPrintTag(struct entrant * e, double gatePosition){
    struct tagRead{
        int id;
        struct timeval tagTime;
        double tagPosition;
        double gatePosition;
    };
    printf("{ \"id\": %i, \"chipTime\": \"%lu s %lu ms\", \"tagPosition\": %f, \"gatePosition\": %f}\n",
           e->id,
           e->myTime.tv_sec  - e->startAt.tv_sec,
           e->myTime.tv_usec / 1000, // to millis
           e->position,
           gatePosition
           );
}

int main(int argc, char** argv) {
    //struct timeval time;
    double readRadiusInMiles = 0.00189394; // about 10 feet radius.
    struct timeval maxFidelity;
    maxFidelity.tv_sec = 0;
    maxFidelity.tv_usec = OneSecondsInMicroSeconds / 10;
    struct timeval advanceRate;
    advanceRate.tv_sec = 0;
    advanceRate.tv_usec = OneSecondsInMicroSeconds / 20;
    int numEntrants =1000;

    double distance = 3.1;
    int ncheckpoints = 5;
    double checkpoints[ncheckpoints];
    for (int j = 0; j < ncheckpoints; ++j) {
        checkpoints[j] = distance / (ncheckpoints -1) * j;
    }

    unsigned int seed = 2;
    srand(seed);

    struct entrant * entrants = malloc(numEntrants * sizeof(struct entrant));

    for (int i = 0; i < numEntrants; ++i) {
        entrantInit(&entrants[i],i);
        //entrantPrint(&entrants[i]);
    }

    int numRunning =numEntrants;
    while(numRunning){
        numRunning =numEntrants;
        for (int i = 0; i < numEntrants; ++i) {
            if(entrants[i].position > checkpoints[ncheckpoints-1] + readRadiusInMiles){
                numRunning--;
            } else {
                // move them.
                entrantMove(&entrants[i],&advanceRate);
                // get a tag read if near a checkpoint;
                for (int j = 0; j < ncheckpoints; ++j) {
                    double distToCheckpoint =entrants[i].position - checkpoints[j];
                    if(distToCheckpoint < readRadiusInMiles && distToCheckpoint > -readRadiusInMiles){
                        // near a checkpoint, make sure we dont emit tags to frequently
                        struct timeval timeDelta;
                        timersub(&entrants[i].myTime, &entrants[i].lastTag, &timeDelta);
                        if((entrants[i].lastTag.tv_sec == 0 && entrants[i].lastTag.tv_usec == 0)
                                || timercmp(&timeDelta,&maxFidelity,>=)){
                            entrants[i].lastTag = entrants[i].myTime;
                            entrantPrintTag(&entrants[i], checkpoints[j]);
                            break;
                        }
                    }
                }
            }
        }
    }

    free(entrants);
    return 0;
}