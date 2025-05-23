#/bin/bash

set -xe

gcc -g thread_shared_mem.c -o thread_shared_mem

./thread_shared_mem
