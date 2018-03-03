//
// Created by astn on 3/2/18.
//



#include "config.h"

void configInit(struct config * c){
    c->readRadiusInMiles = 0.00189394;
    c->maxFidelity.tv_sec = 0;
    c->maxFidelity.tv_usec = OneSecondsInMicroSeconds / 10;
    c->advanceRate.tv_sec = 0;
    c->advanceRate.tv_usec = OneSecondsInMicroSeconds / 20;
    c->numEntrants = 1000;
    c->distance = 10;
    c->nCheckpoints = 11;
}

void configPrintHelp(){
    printf("Usage\n");
    printf("\t(-r | --readRadiusInMiles) %%f : The read radius in miles where a checkpoint can detect tags.\n");
}

void configParseFromArgs(struct config *c, int argc, char **argv) {
    for (int i = 0; i < argc; ++i) {
        GETARG("-r"     ,"--readRadiusInMiles"    , c->readRadiusInMiles, atof)
        GETARG("-n"     ,"--numEntrants"          , c->numEntrants,atoi)
        GETARG("-d"     ,"--distance"             , c->distance,atof)
        GETARG("-c"     ,"--numCheckpoints"       , c->nCheckpoints,atoi)
        GETARG("-m.sec" ,"--maxFidelity.tv_sec"   , c->maxFidelity.tv_sec,atoi)
        GETARG("-m.usec","--maxFidelity.tv_usec"  , c->maxFidelity.tv_usec,atoi)
        GETARG("-a.sec" ,"--advanceRate.tv_sec"   , c->advanceRate.tv_sec,atoi)
        GETARG("-a.usec","--advanceRate.tv_usec"  , c->advanceRate.tv_usec,atoi)
        IFARG("-h"      ,"--help"                 , configPrintHelp())
    }
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
            , c->numEntrants
            , c->distance
            , c->nCheckpoints
            , c->seed);
}
