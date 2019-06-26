#!/bin/bash

bcms_width=2174626
for i in {0..10}
do
    ./adaptive.o $((bcms_width/(1<<i))) 5 $((1<<i)) 0 /mnt/data1/zipf/ count_k1000000_10000000_1.5 stream_k1000000_10000000_1.5
done

for i in {1..10}
do
    let width=bcms_width/$i
    ./adaptive.o $width 5 $i 0 /mnt/data1/zipf/ count_k1000000_10000000_1.5 stream_k1000000_10000000_1.5
done


#investigate rows, fix height=2 and memory space. I.e. width = memory/rows
for i in {0..10}
do
    ./adaptive.o $((1<<i)) $((1<<1)) $((1<<(11-i))) 0  data/ count_k1024_1048576 stream_k1024_1048576
done

for i in {1..10}
do
    let width=2048/$i
    ./adaptive.o $width $((1<<1)) $i 0  data/ count_k1024_1048576 stream_k1024_1048576
done
