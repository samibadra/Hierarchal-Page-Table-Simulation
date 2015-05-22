/*
 Sami Badra
 
 CS 570, Spring 2015
 Assignment #4, Page Tree Simulation
 FILE: pageFunctions.h
 
 Copyright (c) 2015 Sami Badra. All rights reserved.
 
 DISCLAIMER: Any unauthorized use, including but not limited to, copying or
 redistributing any chunk of the source code (or an entire file) will result in
 punishment by law. I, Sami Badra, own all rights to the files and their contents.
 */

#ifndef _pageFunctions_h
#define _pageFunctions_h

#include "structures.h"
#define ADDRESS_LENGTH 32

int initializePageTable(PAGETABLE *pageTable, char *argv[], int firstLevelArg);

void printTableInfo(PAGETABLE *pageTable);

unsigned int calcBitmask(int start, int length);

unsigned int logicalToPage(unsigned int logicalAddress, unsigned int bitmask, unsigned int shift);

void pageInsert(PAGETABLE *pageTable, unsigned int logicalAddress, unsigned int frame);

void pageInsertHelper(LEVEL *level, unsigned int logicalAddress, unsigned int frame);

MAP * pageLookup(PAGETABLE *pageTable, unsigned int logicalAddress);

MAP * pageLookupHelper(LEVEL *level, unsigned int logicalAddress);

LEVEL * createLevel(PAGETABLE *pageTable, LEVEL *level, int depth);

void dumpPageTable(PAGETABLE *pageTable, FILE *fp);

void dumpPageTableHelper(LEVEL *level, FILE *fp);

#endif
