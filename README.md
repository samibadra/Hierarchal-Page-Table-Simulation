Copyright 2015 Sami Badra. All Rights Reserved


Sami Badra

CS 570, Spring 2015

Assignment #4

FILE: README

##Description
This project is a simulation of an N-level page tree. The program reads from a file consisting of memory accesses for a single process, constructs the tree, and assigns frame indices to each page. The user shall indicate how many bits are to be used for each of the page table levels, and a user-specified file containing hexadecimal addresses is used to construct the page table.

##Optional arguments
-n N           Sets a limit to only process the first N memory references.
-p <filename>  Prints to the specified file, each page# and corresponding frame#, sorted in order by page number.
-t             Show the logical to physical address translation for each memory reference.

————————————————————————————————————————————————————————————————————————————————

##Operating Instructions
1. Compile all of the files, using the Makefile (run the command 'make' while in the directory conaining the files.
2. type 'pagetable <filename>' where <filename> is the name of the file containing addresses.
3. if desired, include one or more of the optional arguments described above. (-n & -p require 1 addition argument)
4. type the number of bits for each level (starting at level 0) the amount of numbers provided will determine the level count for the page tree (i.e. entering 4 number will create 4 level page tree)

##Sample Invocations
pagetable -t trace_file 8 12
(This will construct a 2 level page table with 8 bits for level 0, and 12 bits for level 1. It will process ALL of the memory references in 'trace_file', and output each logical to physical address to stdout)

pagetable –n 1000000 –p page.txt trace_file 8 7 4
(This will Construct a 3 level page table with 8 bits for level 0, 7 bits for level 1, and 4 bits for level 2. It will only process the first 1 million memory references, and write the mappings of valid pages to file page.txt)


##Lessons Learned
- The two most significant aspects I learned from this project are bit manipulation and dynamic memory allocation.
- I used bit manipulation in order to generate a bitmask for each of the page tree levels. I then used these bitmasks to get the index of a given logical address for any level in the page tree.
- I used dynamic memory allocation in order to minimize the size of the page table. The 'level' struct contains a double pointer to the 'nextLevel', basically, an array of pointers pointing to another 'level' struct. For the leaf node levels, I would allocate map pointers instead (which maps a specific page to a specific frame). Doing it this way allows for the program to only allocate memory for each level/map when a logical address needs to be stored under one of its branches.
