#!/bin/bash
total=12

#investigate threshold, fix width=512, height=2, row=2. experiment over threshold=2^(1 to 17)
for i in {0..20}
do
    ./adaptive.o $((1<<9)) $((1<<1)) $((1<<1)) $((1<<i)) /mnt/data1/zipf/count_k1048576_134217728 mnt/data1/zipf/stream_k1048576_134217728
done


