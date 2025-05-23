#/bin/bash

set -xe

gcc  process_priority.c -o process_priority
sudo ./process_priority
