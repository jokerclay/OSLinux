#/bin/bash

set -xe

gcc  process_communicate_shared_memory.c -o process_communicate_shared_memory
sudo ./process_communicate_shared_memory
 
 

