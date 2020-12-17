#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "cache.h"

FILE *fi;
main(int argc, char *argv[])
{
    char in_filename [1024]; 

    unsigned long long int new_sim_cycle, new_pc, new_addr;
    unsigned int new_cmd;
    
    strcpy (in_filename, argv[1] );
    fi = fopen(in_filename,"r"); 
    if ( fi == NULL)
    {
        exit(0);
    }
 
    //Initialize cache parameters
    cache_init();
#ifdef FAULTY_BEHAVIOR
    update_fault_map(BLOCK_GRANULARITY);
#endif

    while( 4 == fscanf(fi,"%llx %llx %u %llx\n", &new_addr, &new_pc, &new_cmd, &new_sim_cycle)) 
    {
	    cache_access(new_addr, new_pc, new_cmd, new_sim_cycle, BLOCK_GRANULARITY) ; 
    }

    my_cache_dump(); 
    fclose(fi); 
}
