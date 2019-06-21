#!/bin/bash
total=12

#investigate rows, fix height=2 and memory space. I.e. width = memory/rows
for i in {0..1}
do
    ./adaptive.o $((1<<i)) $((1<<1)) $((1<<(11-i))) 1000 data/count_k1024_1048576 data/stream_k1024_1048576
done

#investigate rows linearly from (1,10)
for i in {1..10}
do
    let width=2048/$i
    ./adaptive.o $width $((1<<1)) $i 1000 data/count_k1024_1048576 data/stream_k1024_1048576
done

