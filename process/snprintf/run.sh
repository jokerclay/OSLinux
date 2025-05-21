#/bin/bash

set -xe
gcc -ggdb snprintf.c -o snprintf

./snprintf
