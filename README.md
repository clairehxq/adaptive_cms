
1. generate zipf data
```
cd brad
gcc -c syn_zipf.c zipf.c -lm
```
run syn_zipf.o to generate distinct keys=10^6, stream size=10^7, k=1.5 zipf data. Save two files to /mnt/data1/zipf/ (or change save location in syn_zipf.c
```
./syn_zipf.o 1000000 10000000 1.5
```

2. Make executables
```
g++ -c adaptive.cpp
chmod u+x *.sh
```

3. run experiment 
```
./exp.sh
./exp_row.sh
./exp_threshold.sh
```
