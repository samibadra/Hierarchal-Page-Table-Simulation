/*
 Sami Badra
 Assignment 4, CS570
 FILE: main.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "structures.h"
#include "pageFunctions.h"
#include "byutr.h"

bool nFlag = false, pFlag = false, tFlag = false;
FILE *printFile;
int limit;
char command[50];
int levelCount;

void parseArguments(int argc, char **argv) {
    levelCount = argc-2;
    int c;
    while((c = getopt(argc, argv, "n:p:t")) != -1) {
        switch(c) {
            case 'n':           //-n flag sets a limit to number of addresses to process
                nFlag = true;
                limit = atoi(optarg);
                levelCount -= 2;
                break;
            case 'p':           //-p flag will write each page# and corresponding frame# to a file
                pFlag = true;
                if (!(printFile = fopen(optarg, "w"))) {
                    printf("Error opening file!\n");
                    exit(1);
                }
                strcpy(command, "sort -k1 -o ");
                strcat(command, optarg);
                strcat(command, " ");
                strcat(command, optarg);
                levelCount -= 2;
                break;
            case 't':           //-t flag will output each logical address and (translated) actual address
                tFlag = true;
                levelCount--;
                break;
            default:
                break;
        }
    }
}

void runSimulation(int argc, char **argv) {
    FILE *traceFile;
    
    /*create the page table and initialize it*/
    PAGETABLE *pageTable = calloc(1, sizeof(PAGETABLE));
    pageTable->levelCount = levelCount;
    int offsetBits = ADDRESS_LENGTH - initializePageTable(pageTable, argv, (argc - levelCount));
    
    p2AddrTr trace;     //this struct contains address when fetched from trace file
    if ((traceFile = fopen(argv[argc-levelCount-1],"rb")) == NULL) {
        fprintf(stderr,"cannot open %s for reading\n",argv[argc-levelCount-1]);
        exit(1);
    }
    
    unsigned long addressCount = 0;
    while (!feof(traceFile)) {
        if (nFlag)
            if (addressCount >= limit)      //break out of loop if limit is reached
                break;
        if (NextAddress(traceFile, &trace)) {
            unsigned int address = (unsigned int)trace.addr;
            if (pageLookup(pageTable, address) == NULL) {   //Page has not been seen yet
                pageTable->misses++;
                pageInsert(pageTable, address, pageTable->frameCount);  //insert the new page
                if (pFlag) {
                    unsigned int page = address;
                    page >>= offsetBits;
                    MAP *map = pageLookup(pageTable, address);
                    fprintf(printFile, "%08X -> %08X\n", page, map->frame);
                }
            }
            else
                pageTable->hits++;
            if (tFlag) {
                MAP *map = pageLookup(pageTable, address);
                unsigned int translated = address;
                translated &= ((1 << offsetBits) - 1);
                translated += (map->frame << (offsetBits));
                printf("%08X -> %08X\n", address, translated);
            }
        }
        addressCount++;
    }
    
    if (pFlag) {
        fflush(printFile);
        system(command);        //system call to sort the output file by page number
    }
    
    int hitPercent = (float)pageTable->hits/(float)addressCount * 100;
    int missPercent = (float)pageTable->misses/(float)addressCount * 100;
    printf("\nAddresses Processed: %lu\n", addressCount);
    printf("Hits: %i (%i percent)\n", pageTable->hits, hitPercent);
    printf("Misses: %i (%i percent)\n", pageTable->misses, missPercent);
    if (pFlag)
        fclose(printFile);
    
    fclose(traceFile);
}

int main(int argc, char **argv) {
    parseArguments(argc, argv);
    runSimulation(argc, argv);
    return 0;
}


