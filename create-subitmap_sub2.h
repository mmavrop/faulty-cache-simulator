#ifndef CREATESUBITMAP_SUB2_H
#define CREATESUBITMAP_SUB2_H


//void updatesublkbitmap(unsigned int blocksize, unsigned int sets, unsigned int assoc, int sublk);
void updatesublkbitmap_sub2(unsigned int cachesize, unsigned int assoc);	//just for 32-block_size and 2-sub_block 

extern int sbitmap2[2048][64];

#endif /* CREATEBITMAP_SUB2_H */
