/*
 Sami Badra
 
 CS 570, Spring 2015
 Assignment #4, Page Tree Simulation
 FILE: pageFunctions.c
 
 Copyright (c) 2015 Sami Badra. All rights reserved.
 
 DISCLAIMER: Any unauthorized use, including but not limited to, copying or
 redistributing any chunk of the source code (or an entire file) will result in
 punishment by law. I, Sami Badra, own all rights to the files and their contents.
 */

#include <stdio.h>
#include <stdlib.h>
#include "pageFunctions.h"

/* initializes page table, and general info about page table including the bitmask and number of shift bits
 * for each level in the table, as well as the number of "next level pointers" i.e. entryCount for each level.
 */
int initializePageTable(PAGETABLE *pageTable, char **argv, int position) {
    pageTable->shiftArray = calloc(pageTable->levelCount, sizeof(int));
    pageTable->bitmaskArray = calloc(pageTable->levelCount, sizeof(unsigned int));
    pageTable->entryCount = calloc(pageTable->levelCount, sizeof(unsigned int));
    
    int bitsUsed = 0;
    int i;
    for (i = 0; i < pageTable->levelCount; i++) {
        int levelBits = atoi(argv[position]);
        pageTable->shiftArray[i] = ADDRESS_LENGTH - bitsUsed - levelBits;
        int maskStart = ADDRESS_LENGTH - bitsUsed; //calculate bitmask for current level
        pageTable->bitmaskArray[i] = calcBitmask(maskStart, levelBits);
        pageTable->entryCount[i] = (1 << levelBits); //2 ^ levelBits
        bitsUsed += levelBits;
        position++;
        if (bitsUsed >= ADDRESS_LENGTH) {
            fprintf(stderr,"Logical address length is too long. Max: 32 bits\n");
            exit(1);
        }
    }
    
    pageTable->hits = 0;
    pageTable->misses = 0;
    pageTable->rootNode = createLevel(pageTable, pageTable->rootNode, 0);
    return bitsUsed;
}

/* prints general information about the page table */
void printTableInfo(PAGETABLE *pageTable) {
    int i;
    for (i = 0; i < pageTable->levelCount; i++) {
        printf("LEVEL %i INFO: ", i);
        printf("Mask: %08X\tShift: %i\tEntry Count: %i\n", pageTable->bitmaskArray[i], pageTable->shiftArray[i], pageTable->entryCount[i]);
    }
}

/*generates a bitmask of specified length 'length', and starting at the specified bit 'start'*/
unsigned int calcBitmask(int start, int length) {
    unsigned int mask = (1 << length) - 1; // (2^length) - 1
    mask <<= (start - length); //shifts the 1's to the correct starting position
    return mask;
}

/* returns the index of a particular level of the page tree (given the bitmask and the number of bits to shit right
 * i.e. logicalToPage(0x7B93E1A5, 0x0FF00000, 20) would return 0xB9
 */
unsigned int logicalToPage(unsigned int logicalAddress, unsigned int bitmask, unsigned int shift) {
    unsigned int page = logicalAddress;
    page &= bitmask;
    page >>= shift;
    return page;
}

/* calls pageInsertHelper() on the root node */
void pageInsert(PAGETABLE *pageTable, unsigned int logicalAddress, unsigned int frame) {
    pageInsertHelper(pageTable->rootNode, logicalAddress, frame);
}

/* inserts the given logical address into the page table, and creates new levels when needed */
void pageInsertHelper(LEVEL *level, unsigned int logicalAddress, unsigned int frame) {
    unsigned int index = logicalToPage(logicalAddress, level->pageTable->bitmaskArray[level->depth], level->pageTable->shiftArray[level->depth]);
    
    if (level->isLeafNode) {        //test if current level is a leaf node
        level->map[index].isValid = true;
        level->map[index].frame = frame;
        level->pageTable->frameCount++;
    }
    else {      //traverse to the next level. Create a new level if nextLevel[index] is NULL
        if (level->nextLevel[index] == NULL)
            level->nextLevel[index] = createLevel(level->pageTable, level, level->depth + 1);
        pageInsertHelper(level->nextLevel[index], logicalAddress, frame);
    }
}

/* calls pageLookupHelper() on the root node */
MAP * pageLookup(PAGETABLE *pageTable, unsigned int logicalAddress) {
    return pageLookupHelper(pageTable->rootNode, logicalAddress);
}

/* searches for a given logical address and returns a pointer to the map struct associated with the address, or NULL if address not found */
MAP * pageLookupHelper(LEVEL *level, unsigned int logicalAddress) {
    unsigned int index = logicalToPage(logicalAddress, level->pageTable->bitmaskArray[level->depth], level->pageTable->shiftArray[level->depth]);
    if (level->isLeafNode) {        //test if current level is a leaf node
        if (level->map[index].isValid)        //test if map[index] is valid
            return &level->map[index];
        else                        //page has been seen for the first time
            return NULL;
    }
    else {
        if (level->nextLevel[index] == NULL)      //interior level at [index] not found, return NULL
            return NULL;
        return pageLookupHelper(level->nextLevel[index], logicalAddress);   //traverse to next level
    }
}

/* allocates space for a new level in the page table */
LEVEL * createLevel(PAGETABLE *pageTable, LEVEL *level, int depth) {
    level = calloc(1, sizeof(LEVEL));
    level->pageTable = pageTable;
    level->depth = depth;
    level->isLeafNode = (depth+1 >= pageTable->levelCount);
    if (level->isLeafNode) {        //allocate maps for leaf nodes
        level->map = calloc(pageTable->entryCount[depth], sizeof(MAP));
        int i;
        for (i = 0; i < pageTable->entryCount[depth]; i++) {
            level->map[i].isValid = false;
        }
    }
    else {          //allocate next level pointers
        level->nextLevel = calloc(pageTable->entryCount[depth], sizeof(LEVEL *));
    }
    return level;
}
