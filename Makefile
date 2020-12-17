CC = g++

CFLAGS = -g -pg -O0

.cc.o:
	${CC} ${CFLAGS} -c $*.c 


statpred:	        main.o cache.o create-subitmap.o create-subitmap_sub2.o create-subitmap_sub4.o create-subitmap_sub8.o
	${CC} ${CFLAGS} main.o cache.o create-subitmap.o create-subitmap_sub2.o create-subitmap_sub4.o create-subitmap_sub8.o -o my_cache 

main.o : cache.h create-subitmap.h create-subitmap_sub2.h create-subitmap_sub4.h create-subitmap_sub8.h 
statcache.o : cache.h 

all: 	my_cache  

clean:
	rm -f *.o; rm -f *~; rm -f my_cache ;

