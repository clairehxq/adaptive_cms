//#define _GNU_SOURCE
#include "zipf.h"
#include <assert.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

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

static void test1 (long N, long gencount, FILE *count_file, FILE *stream_file) {  
    ZIPFIAN z = create_zipfian(1, N, RFUN);
    int *counts;
    if (1) {
	counts = malloc(N * sizeof(counts));
	for (int i=0; i<N; i++) {
	    counts[i] = 0;
	}
    }

    for (long i=0; i<gencount; i++) {
	long g = zipfian_gen(z);
	assert(0<=g && g<N);
	if (1) {
	    counts[g]++;
	}
    }

    if (1) {
	for (int i=0; i<N; i++) {
	  //printf("%4.1f (%4.1f)\n", counts[0]/(double)counts[i], i/(counts[0]/(double)counts[i]));
	  fprintf(count_file, "%d %d\n", i, counts[i]);
	  for (int j=0; j<counts[i]; j++){
	    fprintf(stream_file, "%d\n", i);
	  }
	}

    }
    destroy_zipfian(z);
}

//int main (int argc __attribute__((__unused__)), char *argv[] __attribute__((__unused__))) {
int main (int argc, char *argv[] ) {
    {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	RSEED(tv.tv_sec + tv.tv_usec);
    }
    FILE *c_file, *s_file;
    if (argc >= 3){
      //int keys_exp = atoi(argv[1]);
      //int gen_counts_exp = atoi(argv[2]);
      u_int32_t keys = atoi(argv[1]); //1 << keys_exp;
      u_int32_t gen_counts = atoi(argv[2]); //1 << gen_counts_exp;
      char *c_filename[20], *s_filename [20];
      strcpy(s_filename, "../data/stream_k");
      strcpy(c_filename, "../data/count_k");
      strcat(c_filename, argv[1]);
      strcat(c_filename, "_");
      strcat(c_filename, argv[2]);
      strcat(s_filename, argv[1]);
      strcat(s_filename, "_");
      strcat(s_filename, argv[2]);
      printf("generating %u keys, %u rows. Output to %s, %s \n", keys, gen_counts, c_filename, s_filename);
      c_file = fopen(c_filename, "w");
      s_file = fopen(s_filename, "w");
      test1(keys, gen_counts, c_file, s_file);
    }
    else{
      printf("generating 2000 keys, 4000000 rows. Output to\n");
      test1(2000, 4000000, c_file, s_file);
    }
    fclose(c_file);
    fclose(s_file);
    return 0;
}
