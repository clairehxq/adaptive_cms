#!/bin/bash
total=12

#investigate threshold, fix width=512, height=2, row=2. experiment over threshold=2^(1 to 17)
for i in {0..20}
do
    ./adaptive.o $((1<<9)) $((1<<1)) $((1<<1)) $((1<<i)) data/ count_k1024_1048576 stream_k1024_1048576
done

bcms_width=2174625
#investigate threshold, fix width=512, height=2, row=2. experiment over threshold=2^(1 to 17)
for i in {0..20}
do
    let width=bcms_width/4
    ./adaptive.o $width 5 $((1<<2)) $((1<<i)) /mnt/data1/zipf/ count_k1000000_10000000_1.5 stream_k1000000_10000000_1.5
done

