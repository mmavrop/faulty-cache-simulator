#ifndef CREATESUBITMAP_H
#define CREATESUBITMAP_H


//void updatesublkbitmap(unsigned int blocksize, unsigned int sets, unsigned int assoc, int sublk);
void updatesublkbitmap(unsigned int cachesize, unsigned int assoc);	//just for 32-block_size and 2-sub_block 

extern int sbitmap[2048][64];

#endif /* CREATEBITMAP_H */
