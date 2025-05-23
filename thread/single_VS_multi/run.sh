#/bin/bash

set -xe

gcc -g single_thread.c -o single_thread
gcc -g muti_thread.c   -o muti_thread


./single_thread
./muti_thread

