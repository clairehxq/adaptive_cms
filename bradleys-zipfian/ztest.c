#define _GNU_SOURCE
#include "zipf.h"
#include <assert.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>

#ifndef  USE_MYRANDOM
#define RFUN random
#define RSEED srandom
#else
#define RFUN myrandom
#define RSEED mysrandom

static unsigned int m_z = 1;
static unsigned int m_w = 1;
static void mysrandom (unsigned int seed) {
    m_z = seed;
    m_w = (seed<<16) + (seed >> 16);
}

static long myrandom()
{
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return ((m_z << 16) + m_w) % 0x7FFFFFFF;
}
#endif

//static float tdiff (struct timeval *start, struct timeval *end) {
//return (end->tv_sec-start->tv_sec) +1e-6*(end->tv_usec - start->tv_usec);
//}

static void test1 (long N, long gencount) {
    struct timeval a,b,c;
    //printf("Generating %ld elements in universe of %ld items with characteristic exponent %f\n",	   gencount, N, s);
    gettimeofday(&a, NULL);
    ZIPFIAN z = create_zipfian(1, N, RFUN);
    int *counts;
    if (1) {
	counts = malloc(N * sizeof(counts));
	for (int i=0; i<N; i++) {
	    counts[i] = 0;
	}
    }
    gettimeofday(&b, NULL);
    //printf("Setup time    = %0.6fs\n", tdiff(&a, &b));
    for (long i=0; i<gencount; i++) {
	long g = zipfian_gen(z);
	assert(0<=g && g<N);
	if (1) {
	    counts[g]++;
	}
    }
    gettimeofday(&c, NULL);
    //double rtime = tdiff(&b, &c);
    //printf("Generate time = %0.6fs (%f per second)\n", rtime, gencount/rtime);
    if (1) {
	for (int i=0; i<N; i++) {
	  //printf("%4.1f (%4.1f)\n", counts[0]/(double)counts[i], i/(counts[0]/(double)counts[i]));
	  printf("%d, %d\n", i, counts[i]);
	}
	printf("\n");
    }
    destroy_zipfian(z);
}

int main (int argc __attribute__((__unused__)), char *argv[] __attribute__((__unused__))) {
    {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	RSEED(tv.tv_sec + tv.tv_usec);
    }
    test1(2000, 4000000);

    //test1(1000, 1000000, 1);

    //test1(100000000L, 1000000L, 1.5);
    return 0;
}
