#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "cache.h"

/* create bitmap */
#include "create-subitmap.h"
#include "create-subitmap_sub2.h"
#include "create-subitmap_sub4.h"
#include "create-subitmap_sub8.h"

unsigned long int line_counter;
unsigned long int accesses;
unsigned long int misses;
unsigned long int hits;
unsigned long int replacements;
unsigned long int false_hits;

struct cache_blk_t cache_s[CACHE_SETS][CACHE_WAYS];
struct cache_blk_t *blk;
struct cache_blk_t *repl;

void cache_init()
{
    for (uint16_t set = 0; set < CACHE_SETS; set++)
    {
        for (uint8_t way = 0; way < CACHE_WAYS; way++)
        {
            cache_s[set][way].tag = 0;
            cache_s[set][way].addr = 0;
            cache_s[set][way].timestamp = 0;
            cache_s[set][way].pc = 0;
            cache_s[set][way].valid_bit = 0;
            cache_s[set][way].dirty = 0;
                    
            for (uint8_t i=0; i<8; i++)
            {
                cache_s[set][way].faulty_sblks[i] = 0;   
            }
        }
    }

    line_counter = 0;
    accesses     = 0;
    misses       = 0;
    replacements = 0;
    hits         = 0;
    false_hits   = 0;
}

void update_fault_map(uint8_t subBlocks)
{
    switch (subBlocks) 
    {   
    case BDIS:
        updatesublkbitmap(CACHE_SIZE_1, CACHE_WAYS);
        break;
    case SBDIS2:
        updatesublkbitmap_sub2(CACHE_SIZE_1, CACHE_WAYS);
        break;
    case SBDIS4:
        updatesublkbitmap_sub4(CACHE_SIZE_1, CACHE_WAYS);
        break;
    case SBDIS8:
        updatesublkbitmap_sub8(CACHE_SIZE_1, CACHE_WAYS);
        break;
    default: 
        assert(0);
    }       
    
    for (uint16_t set = 0; set < CACHE_SETS; set++)
    {
        for (uint8_t way = 0; way < CACHE_WAYS; way++)
        {
            for (uint8_t subblk=0; subblk < subBlocks; subblk++)
            {
                switch (subBlocks) 
                {   
                case BDIS:
                    cache_s[set][way].faulty_sblks[subblk] = sbitmap[set][subblk+way*subBlocks];
                    break;
                case SBDIS2:
                    cache_s[set][way].faulty_sblks[subblk] = sbitmap2[set][subblk+way*subBlocks];
                    break;
                case SBDIS4:
                    cache_s[set][way].faulty_sblks[subblk] = sbitmap4[set][subblk+way*subBlocks];
                    break;
                case SBDIS8:
                    cache_s[set][way].faulty_sblks[subblk] = sbitmap8[set][subblk+way*subBlocks];
                    break;
                default: 
                    assert(0);
                }   
            }
        }
    }
}    

void cache_access(unsigned long long int new_addr, 
                  unsigned long long int new_pc, 
                  unsigned int new_cmd, 
                  unsigned long long int new_sim_cycle, 
                  int subBlocks)
{

    uint8_t subbits = uint8_t(log(subBlocks)/log(2)); 
    uint8_t sub_shift = uint8_t(log(CACHE_BLOCK)/log(2)) - subbits;
    uint8_t sub_mask = (1 << (subbits)) - 1;
    uint8_t sblk = (((new_addr) >> sub_shift) & sub_mask); 
	
    unsigned long long int tag = new_addr >> BIT_OFFSET;
    uint16_t set = tag & SET_MASK;

    bool found = false;
    unsigned long long int timestamp_min;

    line_counter ++ ;
    accesses ++ ;

#ifdef DEBUG
    printf("\n -----------------------------------------------------------\n");
    printf("New access in set %d, with tag= %llx, subblock= %d, timstm=%lu\n", set, tag, sblk, new_sim_cycle);
#endif  

    for (uint8_t way = 0; way < CACHE_WAYS; way++)
    {
        blk = &(cache_s[set][way]);

        if ( (blk->tag == tag ) && (blk->faulty_sblks[sblk] == 0) )
        {
            found = true;
            break;
        }
        else if ( (blk->tag == tag ) && (blk->faulty_sblks[sblk] == 1) )
        {   
            //false hit scenario                    
            false_hits++;

            blk->timestamp = new_sim_cycle;
            blk->tag=0;
            blk->valid_bit=0;       
            blk->dirty = 0;

            break;
        }
    }

    // Cache hit path
    if (found == true)
    {
        blk->timestamp = new_sim_cycle ;
        hits ++ ;
        
        if (new_cmd == 0) 
        {
             blk->dirty = 1;
        }
        assert (blk->valid_bit == 1) ;
        return;
    }
    
    //Cache miss path
    repl = NULL;
    assert (found == false) ;
    
    misses ++ ; 

    // not found - do global lru          
    timestamp_min = new_sim_cycle + 1;
     
    uint8_t blk_faulty_condition = 0;
    uint8_t way_faulty_condition = 0;
    for (uint8_t way = 0; way < CACHE_WAYS; way++)
    {                                                                                          
        blk = &(cache_s[set][way]);
        
        blk_faulty_condition = 0;
        for (uint8_t i=0; i<subBlocks; i++)
        {
            if (blk->faulty_sblks[i]==1)
            {
                 blk_faulty_condition++;
                 way_faulty_condition++;
            }
        }
        if ((blk->timestamp < timestamp_min) && (blk_faulty_condition < subBlocks)) 
        {                                                                               
            repl = blk; 
            timestamp_min = blk->timestamp;
        }
    }  

    //Full faulty set 
    if (way_faulty_condition == (uint8_t)subBlocks*CACHE_WAYS)
    {       
        return; 
    }

    if (repl->valid_bit == 1)
    {  
        replacements ++ ;
    }

    assert (repl != NULL ) ;     
    repl->timestamp = new_sim_cycle ;
    repl->tag = tag ;
    repl->addr = new_addr;
    repl->pc = new_pc ;
    repl->valid_bit = 1;

    return;
}
    

/// **************** MY_FUNCTIONS **********************************
void my_cache_dump()       
{
    printf("accesses:\t %lu\n", accesses);  
    printf("misses:\t %lu\n", misses);  
    printf("false_hits:\t %lu\n", false_hits);  
    printf("hits:\t %lu\n", hits);
    printf("replacements:\t %lu\n", replacements);  
}
