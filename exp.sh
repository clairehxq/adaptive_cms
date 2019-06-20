#!/bin/bash
total=12

# investigate the best width/height configuration using the same memory space
./adaptive.o $((1<<10)) $((1<<0)) $((1<<2)) 1000 data/count_k1024_1048576 data/stream_k1024_1048576
./adaptive.o $((1<<9)) $((1<<1)) $((1<<2)) 1000 data/count_k1024_1048576 data/stream_k1024_1048576
./adaptive.o $((1<<8)) $((1<<2)) $((1<<2)) 1000 data/count_k1024_1048576 data/stream_k1024_1048576
./adaptive.o $((1<<7)) $((1<<3)) $((1<<2)) 1000 data/count_k1024_1048576 data/stream_k1024_1048576
./adaptive.o $((1<<6)) $((1<<4)) $((1<<2)) 1000 data/count_k1024_1048576 data/stream_k1024_1048576
./adaptive.o $((1<<5)) $((1<<5)) $((1<<2)) 1000 data/count_k1024_1048576 data/stream_k1024_1048576
./adaptive.o $((1<<4)) $((1<<6)) $((1<<2)) 1000 data/count_k1024_1048576 data/stream_k1024_1048576
./adaptive.o $((1<<3)) $((1<<7)) $((1<<2)) 1000 data/count_k1024_1048576 data/stream_k1024_1048576
./adaptive.o $((1<<2)) $((1<<8)) $((1<<2)) 1000 data/count_k1024_1048576 data/stream_k1024_1048576
./adaptive.o $((1<<1)) $((1<<9)) $((1<<2)) 1000 data/count_k1024_1048576 data/stream_k1024_1048576
./adaptive.o $((1<<0)) $((1<<10)) $((1<<2)) 1000 data/count_k1024_1048576 data/stream_k1024_1048576
#investigate 
./adaptive.o $((1<<9)) $((1<<1)) $((1<<2)) 1000 data/count_k1024_1048576 data/stream_k1024_1048576
