#ifndef CACHE_H
#define CACHE_H

#include <stdio.h>
#include <stdint.h>

#define CACHE_SIZE_1 64
#define CACHE_SIZE (CACHE_SIZE_1 * 1024)
#define CACHE_WAYS 8
#define CACHE_BLOCK 64
#define CACHE_SETS (CACHE_SIZE / (CACHE_WAYS*CACHE_BLOCK))
#define SET_MASK (CACHE_SETS-1)
#define BIT_OFFSET int(log(CACHE_BLOCK)/log(2))

//Enable faulty mechanism
// #define FAULTY_BEHAVIOR
//Sub-Block granularity
#define BLOCK_GRANULARITY 1

#define BDIS   1u
#define SBDIS2 2u
#define SBDIS4 4u
#define SBDIS8 8u


//////////////////////////////////////////////////////////////////////////////////////////////////////////

/* cache block (or line) definition */
struct cache_blk_t {
	unsigned long long int tag ;	
	unsigned long long int addr ;	
	unsigned long long  int timestamp ;	
	unsigned long long int pc ;	
	unsigned int valid_bit ;
	unsigned int faulty ;
	unsigned int faulty_sblks[8] ;
	int dirty;
};

void cache_init();
void update_fault_map(uint8_t num);
void my_cache_dump();
void cache_access (unsigned long long int, unsigned long long  int, unsigned int, unsigned long long int, int);

extern unsigned long int line_counter;
extern unsigned long int misses_per_interval;


#endif /* CACHE_H */
