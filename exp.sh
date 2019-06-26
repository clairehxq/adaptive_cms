#!/bin/bash
for i in {3..10}
do
    ./adaptive.o $((1<<i)) $((1<<(10-i))) $((1<<2)) 1 data/ count_k1024_1048576 stream_k1024_1048576
done

for i in {3..10}
do
    ./adaptive.o $((1<<i)) $((1<<(10-i))) $((1<<2)) 888 /mnt/data1/zipf/ count_k1048576_134217728 stream_k1048576_134217728

done
