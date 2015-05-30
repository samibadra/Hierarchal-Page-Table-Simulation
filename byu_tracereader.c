#include <stdio.h>
#include "byutr.h"

/*
 * If you are using this program on a big-endian machine (something
 * other than an Intel PC or equivalent) the unsigned longs will need
 * to be converted from little-endian to big-endian.
 */
unsigned long swap_endian(unsigned long num)
{
    return(((num << 24) & 0xff000000) | ((num << 8) & 0x00ff0000) |
           ((num >> 8) & 0x0000ff00) | ((num >> 24) & 0x000000ff) );
}

/* determine if system is big- or little- endian */
ENDIAN endian()
{
    unsigned long *a;
    unsigned char p[4];
    
    a = (unsigned long *)p;
    *a = 0x12345678;
    if(*p == 0x12)
        return BIG;
    else
        return LITTLE;
}

/* int NextAddress(FILE *trace_file, p2AddrTr *Addr)
 * Fetch the next address from the trace.
 *
 * trace_file must be a file handle to an trace file opened
 * with fopen. User provides a pointer to an address structure.
 *
 * Populates the Addr structure and returns non-zero if successful.
 */

int NextAddress(FILE *trace_file, p2AddrTr *addr_ptr) {
    int readN;	/* number of records stored */
    static ENDIAN byte_order = UNKNOWN;	/* don't know machine format */
    
    /* Read the next address record. */
    readN = (int)fread(addr_ptr, sizeof(p2AddrTr), 1, trace_file);
    
    if (readN) {
        switch (byte_order) {
            case LITTLE:
                break;	/* do nothing */
            case BIG:
                /* records stored in little endian format, convert */
                addr_ptr->addr = swap_endian(addr_ptr->addr);
                addr_ptr->time = swap_endian(addr_ptr->time);
                break;
            case UNKNOWN:
                /* Find out with what type of machine we are working... */
                byte_order = endian();
                break;
        }
    }
    return readN;
}

