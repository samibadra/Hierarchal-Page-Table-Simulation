/*
 Sami Badra
 Assignment 4, CS570
 FILE: structures.h
 */

#ifndef _pagetable_h
#define _pagetable_h

typedef enum {false, true} bool;

typedef struct PAGETABLE {
    struct LEVEL *rootNode;
    int levelCount;
    int frameCount;
    int hits;
    int misses;
    unsigned int *bitmaskArray;
    int *shiftArray;
    int *entryCount;
} PAGETABLE;

typedef struct MAP {        //leaf node structre
    bool isValid;
    unsigned int frame;
} MAP;

typedef struct LEVEL {      //interior level structure
    bool isLeafNode;
    struct PAGETABLE *pageTable;
    struct LEVEL **nextLevel;
    struct MAP *map;
    int depth;
} LEVEL;

#endif
