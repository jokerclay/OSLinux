#/bin/bash

set -xe

gcc -g cpu_affinity.c -o cpu_affinity

./cpu_affinity


