/*
 Sami Badra
 
 CS 570, Spring 2015
 Assignment #4, Page Tree Simulation
 FILE: structures.h
 
 Copyright (c) 2015 Sami Badra. All rights reserved.
 
 DISCLAIMER: Any unauthorized use, including but not limited to, copying or
 redistributing any chunk of the source code (or an entire file) will result in
 punishment by law. I, Sami Badra, own all rights to the files and their contents.
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
