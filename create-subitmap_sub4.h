#ifndef CREATESUBITMAP_SUB4_H
#define CREATESUBITMAP_SUB4_H


//void updatesublkbitmap(unsigned int blocksize, unsigned int sets, unsigned int assoc, int sublk);
void updatesublkbitmap_sub4(unsigned int cachesize, unsigned int assoc);	//just for 32-block_size and 2-sub_block 

extern int sbitmap4[2048][64];

#endif /* CREATEBITMAP_SUB4_H */
