#!/bin/bash
rm -f *.o test_svd master_svd

gcc -g -O3 -c svd.c
gcc -g -O3 -c disp_mat.c
gcc -g -O3 -c trans_mat.c
gcc -g -O3 -c mul_mat.c

echo "Compiling master_svd"
gcc -g -O3 master_svd.c svd.o disp_mat.o trans_mat.o mul_mat.o -lpthread -lm -o master_svd

