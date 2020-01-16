#include "cache.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <math.h>
#include <time.h>
#include<sys/time.h>

#include <string.h>
#include <ctype.h>
#include <getopt.h>

/* 
 * Cacheline struct. Implements necessary metadata for cacheLine. 
 * Metadata:
 *  validBit - 0 if cacheline is invalid, 1 if cacheLine is valid
 *  tag - integer reflecting which block is stored in cacheLine
 *  timeStamp - a timeval object capturing the system time when 
 *  			the cacheLine was last accessed. This allows us
 *  			to identify the least recently used (LRU) cacheline. 
 */
typedef struct cacheLine{
	int validBit;
	int tag;
	struct timeval timeStamp;
} cacheLine;

/*
 * This method takes as input three parameters:
 * 	s: the number of set bits 
 * 	E: the number of lines per cacheset
 * 	b: the number of offset bits
 *
 * We construct a cache with S = 2^s cache sets, each of 
 * which hold E cacheLines. The cache is represented as a
 * two dimensional matrix of cacheLines.
 */
cacheLine*** makeCache(int s, int E, int b) {
	/* Allocates space for cacheSets */
	cacheLine*** cache = (cacheLine***) malloc(sizeof(cacheLine**)*pow(2,s));
	
	/* For each cache set in our cache */ 
	for(int i = 0; i < pow(2,s); i++){ 

		/* Allocate memory for E cache lines in each set */ 
		cache[i] = (cacheLine**) malloc(sizeof(cacheLine*)*E);

		/* Initialize metadata for each cacheLine */  
		for(int j = 0; j < E; j++){
			cache[i][j] = (cacheLine*) malloc(sizeof(cacheLine));
			cache[i][j]->validBit = 0; // set valid bit to 0 
			cache[i][j]->tag = 0; // initialize tag to 0
			// Set time stamp to current system time
			gettimeofday(&cache[i][j]->timeStamp, NULL);
		}
  	}
	return cache;
}

/*
 * This method takes as an argument an address, s, E, and b
 * and returns the tag for that address. 
 */
int getTagBits(unsigned long long address,int s, int E, int b){
	/* If we have s index bits, b offset bits, and our addresses are 
	 * 64 bits long, then we must have 64 - (b+s) tag bits. Also, 
	 * recall from class that these are the top 64 - (b+s) bits. 
	 * We create a mask of b+s 0's followed by 64-(b+s) 1's. 
	 */
	int mask = ~((~0) << (64 -(b+s)));

	/*  We shift our address to the right by b+s bits, placing the tag bits 
	 *  in the lower 64 - (b+s) bits. Then we mask these away using our tag bits. 
	 */
	return (address >> (b + s)) & mask;
}

/*
 * Given a 64 bit address and s,E,b this method returns
 * the index of the corresponding cache set. 
 */
int getIndexBits(unsigned long long address, int s, int E, int b){
	/* We know our index bits are s long, so our mask is s 1's. */

	/* In order to obtain the index bits, we have to create a mask
	 * that is s bits long */ 	
	int mask = ~((~0) << s);

	/* We shift right to remove the offset bits, and mask away the 
	 * index bits, as desired. */
	return (address >> b) & mask;
}

/*
 * This method takes as input the following parameters:
 * 		address: the address of the block in memory we are caching
 * 		s: # of index bits
 * 		E: # of cache lines per set
 * 		b: # of offset bits 
 *		evict: a pointer to a counter of the evictions so far
 *		hits: a pointer to a counter of the hits so far
 *		misses: a pointer to a counter of the misses so far
 *  	cache: 2d array of cachelines
 *  	accessCacheinfo: a pointer to a string that we edit for verbosity. 
 * 
 * We noticed that reading, writing were equivalent, therefore this 
 * serves as a generic cache access for both. We place the block in 
 * the cache, and update evict, hits, misses counters as well as 
 * accessCacheinfo string accordingly. 
 */ 
void accessCache(unsigned long long address, int s,  int E, int b, 
											int* evict, 
											int* hits, 
											int* misses,
											cacheLine*** cache,
											char** accessCacheInfo){
	// Obtain tag and index from address
	int tag = getTagBits(address, s, E, b);
	int index = getIndexBits(address, s, E, b);
	
	// Grab corresponding cacheset in cache
	cacheLine** set = cache[index];
	
	/* First, we look to see if our data is already in the cache */
	for(int j = 0; j < E; j++){
		/* if tag matches and data is valid */ 
		if((set[j]->tag == tag) && (set[j]->validBit == 1)){
			// set timestamp, increment hits
			gettimeofday(&set[j]->timeStamp, NULL);
			*hits += 1;
			*accessCacheInfo = "hit";
			return;
		}
	}
	
	/* Second, we look to see if there is any invalid data we can overwrite. 
	 * Concurrently, we also locate the least recently used cacheline. */
	int LRUindex = 0;
 
	for(int j = 0; j < E; j++){
		/* If we find invalid data, we replace it with our new block */ 
		if(set[j]->validBit == 0){
			/* set timestamp, new tag, validbit becomes 1, and increment
			 * misses (but not evictions). */
			gettimeofday(&set[j]->timeStamp,NULL);
			set[j]->tag = tag;
			set[j]->validBit = 1;
			*misses += 1;
			*accessCacheInfo = "miss";
			return;
		}
		/* Concurrently, we locate the least recently used block */ 
		if((double)(set[LRUindex]->timeStamp.tv_usec - set[j]->timeStamp.tv_usec) > 0){
			LRUindex = j;
		}
	}
	/* Finally, if our data was not in the cache, and there was no invalid data 
	 * to overwrite, we evict the least recently used data, which we found above. */ 
	set[LRUindex]->tag = tag; // update tag
	gettimeofday(&set[LRUindex]->timeStamp,NULL); // update time
	*evict += 1; // increment eviction counter
	*misses += 1; // increment miss counter

	*accessCacheInfo = "miss eviction";

	return;
}

/*
 * This method frees all allocated space for the cache.  
 */
void freeCache(cacheLine*** cache, int s, int E){
	for(int i = 0; i < pow(2,s); i++){
		for(int j = 0; j < E; j++){
			free(cache[i][j]); // free cacheline
		}
		free(cache[i]); // free each cache set
	}
	free(cache); // free cache pointer
}



/*
 * This method takes as arguments s,E,b, the cache, and a traceFile, 
 * and sets counters evicts, hits, and misses to reflect the 
 * evictions, hits, and misses when we run our cache on the traceFile.   
 */
void runCache(int s, int E, int b, char* traceFile, cacheLine*** cache, 
										int* evicts, int* hits, int* misses, 
																int verbose){ 
	
	/* We create a stream to read our tracefile line by line */ 	
	FILE *stream;
	
	stream = fopen(traceFile, "r");
	if(stream == NULL){
		printf("Read failed");
		exit(EXIT_FAILURE);
	}
	
	/* 
	 * Parses file line by line and calls corresponding cache operation.  
	 */
	char type = 0;
	unsigned long long address = 0;
	int size = 0;
	
	/* While we have not reached end of our file, we parse */
	while(fscanf(stream, " %c %llx,%x", &type, &address, &size) != EOF){
		/* We initialize our two documentation strings to be empty */ 
		char* accessCacheInfo = "";	
		char* modifyInfo = "";

		/* Based on parsed type, we perform corresponding operation */ 
		switch(type){
			/* If we are modifying cache, we will read and then write. We note that
			 * we always get a cachehit on the write. Therefore we increment the hit
			 * and perform one cache access. */ 
			case 'M':
				*hits += 1;
				modifyInfo = "hit";
			/* Cases S and L are equivalent */ 
			case 'S':
			case 'L':
				// Access the cache, updating counters and accessCacheInfo
				accessCache(address, s, E, b, evicts, hits, misses, cache, &accessCacheInfo);
				/* If we our verbose flag is set to 1, we print additional information about
				 * each instruction. Namely, the sequence of hits, misses, or evictions. 
				 * This information is stored in accessCacheInfo as well as modifyInfo strings. 
				 * If we are modifying, then modifyInfo adds an additional hit at the end, to account
				 * for the second operation in modify, which is a write. 
				 */
				if(verbose == 1){
					printf("%c %llx,%x %s %s\n", type, address, size, accessCacheInfo, modifyInfo);
				}

				break;
			/* Note that we are definitely taking advantage of "fall through" in our switch statement.*/ 
			default:
				break;	
		}
	}
	
	fclose(stream);
	return;
}

/*
 * This method takes in flagged command line arguments:
 * -s: # of index bits
 * -E: # of lines per set
 * -b: # of offset bits 
 * -t: tracefile
 * -h: optional flag which prints help information
 * -v: optional flag for more verbose output
 *
 * It creates the cache, runs the trace file, and outputs the results
 * to printSummary. 
 */ 
int main(int argc, char** argv){
	/* Set default values for v and h, as well as initializing
	 * s,E,b, and the traceFile string */
	int v = 0;
	int h = 0;
	int s,E,b;

	char *traceFile;
	char c;

	/* We use some code provided by professor to parse flagged
	 * arguments */
	while ((c = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
		switch (c) {
		case 'h':
			h = 1;
			break;
		case 'v':
			v = 1;
			break;
		case 's':
			s = atoi(optarg); //convert to int
			break;
		case 'E':
			E = atoi(optarg); // convert to int
			break;
		case 'b':
			b = atoi(optarg);
			break;
		case 't':
			traceFile = optarg;
			break;
		default:
		//If we get an unexpected flag, print error message and exit. 
		printf("incorrect arguments");
			exit(1);
		}	
	}
	
	/* Helper documentation if helper flag triggered */ 
	if(h == 1){
		printf("This function is a cache simulator. \n\
	It takes the following flagged command line arguments:\n\
		-s: # of index bits \n\
 		-E: # of lines per set\n\
 		-b: # of offset bits\n\
 		-t: tracefile\n\
 		-h: optional flag which prints help information\n\
 		-v: optional flag for more verbose output\n\
	Example usage includes: cachesim -s 1 -E 4 -b 10 -t t1.trace");
	}

	// make the cache 
	cacheLine*** cache = makeCache(s,E,b);
	
	// create counters for hits, misses, evicts
	int evicts = 0;
	int hits = 0;
	int misses = 0;
	// run cache simulator
	runCache(s, E, b, traceFile, cache, &evicts, &hits, &misses, v);
	
	// free up allocated space for cache
	freeCache(cache, s, E);
		
	// pass data to print summary
	printSummary(hits, misses, evicts);
	return 0;	
}
