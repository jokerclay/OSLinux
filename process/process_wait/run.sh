#/bin/bash

set -xe

gcc  process_wait.c -o process_wait
sudo ./process_wait
