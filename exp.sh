#!/bin/bash
total=2174626
#for i in {0..5}
#do
    #./adaptive.o $((total/(1<<i))) $((1<<i)) $((1<<2)) 0 /mnt/data1/zipf/ count_k1000000_10000000_1.5 stream_k1000000_10000000_1.5

#done

for i in {3..5}
do
    ./adaptive.o $((1<<i)) $((1<<(10-i))) $((1<<2)) 0 data/ count_k1024_1048576 stream_k1024_1048576
done
