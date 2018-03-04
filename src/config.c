//
// Created by astn on 3/2/18.
//



#include <tal.h>
#include "config.h"
#include "entrant_t.h"

void configDefaults(struct config * c){
    c->readRadiusInMiles = 0.00189394;
    c->maxFidelity.tv_sec = 0;
    c->maxFidelity.tv_usec = OneSecondsInMicroSeconds / 10;
    c->advanceRate.tv_sec = 0;
    c->advanceRate.tv_usec = OneSecondsInMicroSeconds / 10;
    c->nEntrant = 1000;
    c->distance = 3.10686;
    c->nCheckpoint = 4;
    c->seed = 1;
    c->checkpoints = NULL;
    c->entrant = NULL;
    c->verbose = false;
    c->exit = false;
}
void configInit(struct config * c){

    // checkpoints
    c->checkpoints = tal_arr(c,float,(size_t)c->nCheckpoint);
    srand(c->seed);
    for (int j = 0; j < c->nCheckpoint; ++j) {
        c->checkpoints[j] = c->distance / (c->nCheckpoint -1) * j;
    }

    // entrant
    c->entrant = tal_arr(c,struct entrant_t,(size_t)c->nEntrant);
    for (int i = 0; i < c->nEntrant; ++i) {
        entrantInit(&c->entrant[i],i);
    }
}

void configPrintHelp(){
    printf("About: Generate-Tags will simulate tag reads along a configurable course length with multiple checkpoints. "
    "It does this by moving each tag forward in time by the advanceRace, then checking to see if the tag is in the read zone radius of a checkpoint. "
    "If it is, the tag is printed to standard out with its ID,Time it was seend,position in miles,and the checkpoint position in miles.\n");
    printf("Usage : Default values show in square brackets [default] \n");
    printf("\t(-v | --verbose) :\tDisplay Config information and entrant details.\n");
    printf("\t(-x | --exit) :\tExit without simulating the race.\n");
    printf("\t(-s | --seed) int [1] : The seed used for the random number generator.");
    printf("\t(-r | --readRadiusInMiles) float [0.00189394] :\tThe read radius in miles where a checkpoint can detect tags.\n");
    printf("\t(-n | --numEntrants) int [1000]:\t\tThe number of people to simulate in the race.\n");
    printf("\t(-d | --distance) float [3.10686]:\t\tThe distance in miles for the course.\n");
    printf("\t(-c | --numCheckpoints) int [4]:\t\tThe number of checkpoints where tags will be observed.\n");
    printf("\t(-m.sec | --maxFidelity.tv_sec) int [0]:\tThe minimum Seconds that must pass before another observation of the same\n"
    "\t\t\t\t\t\t\t tag. The larger the value the bigger the time gap between seeing the same tag.\n");
    printf("\t(-m.usec | --maxFidelity.tv_usec) int [100000]:\tThe minimum uSeconds that must pass before another observation of the same\n"
    "\t\t\t\t\t\t\t tag. The larger the value the bigger the time gap between seeing the same tag.\n");
    printf("\t(-a.sec | --advanceRate.tv_sec) int [0]:\tThe time advance step Seconds component. The larger the value the quicker the \n"
    "\t\t\t\t\t\t\t simulation at the expense of fidelity.\n");
    printf("\t(-a.usec | --advanceRate.tv_usec) int [100000]:\tThe time advance step uSeconds component. The larger the value the quicker\n"
    "\t\t\t\t\t\t\t the simulation at the expense of fidelity.\n");
    printf("\t(-h | --help) :\tPrint this help.\n");
}

int configParseFromArgs(struct config *c, int argc, char **argv) {
    int parsed =0;
    for (int i = 0; i < argc; ++i) {
        GETARG("-r"     ,"--readRadiusInMiles"    , parsed++;c->readRadiusInMiles, atof)
        GETARG("-n"     ,"--numEntrants"          , parsed++;c->nEntrant,atoi)
        GETARG("-d"     ,"--distance"             , parsed++;c->distance,atof)
        GETARG("-c"     ,"--numCheckpoints"       , parsed++;c->nCheckpoint,atoi)
        GETARG("-m.sec" ,"--maxFidelity.tv_sec"   , parsed++;c->maxFidelity.tv_sec,atoi)
        GETARG("-m.usec","--maxFidelity.tv_usec"  , parsed++;c->maxFidelity.tv_usec,atoi)
        GETARG("-a.sec" ,"--advanceRate.tv_sec"   , parsed++;c->advanceRate.tv_sec,atoi)
        GETARG("-a.usec","--advanceRate.tv_usec"  , parsed++;c->advanceRate.tv_usec,atoi)
        GETARG("-s"     ,"seed"                   , parsed++;c->seed,atoi)
        IFARG("-v"      ,"--verbose"              , parsed++;c->verbose=true)
        IFARG("-x"      ,"--exit"                 , parsed++;c->exit=true)
        IFARG("-h"      ,"--help"                 , parsed=-1;configPrintHelp();break)
    }
    return parsed;
}

void configPrint(struct config * c){
    printf("config = {\n"
                   "readRadiusInMiles: %f\n"
                   "maxFidelity: {\n"
                   "  tv_sec: %lu\n"
                   "  tv_usec: %lu\n"
                   "}\n"
                   "advanceRate: {\n"
                   "  tv_sec: %lu\n"
                   "  tv_usec: %lu\n"
                   "}\n"
                   "numEntrants: %i\n"
                   "distance: %f\n"
                   "nCheckpoints: %i\n"
                   "seed: %i\n"
                   "}\n"
            , c->readRadiusInMiles
            , c->maxFidelity.tv_sec
            , c->maxFidelity.tv_usec
            , c->advanceRate.tv_sec
            , c->advanceRate.tv_usec
            , c->nEntrant
            , c->distance
            , c->nCheckpoint
            , c->seed);
}
