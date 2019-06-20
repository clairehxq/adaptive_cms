#!/bin/bash
total=12

#investigate threshold, fix width=512, height=2, row=4. experiment over threshold=2^(1 to 17)
for i in {0..17}
do
    ./adaptive.o $((1<<9)) $((1<<1)) $((1<<2)) $((1<<i)) data/count_k1024_1048576 data/stream_k1024_1048576
done


