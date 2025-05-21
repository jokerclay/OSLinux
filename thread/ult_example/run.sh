#/bin/bash

set -xe

gcc -g ult_example.c -o ult_example
./ult_example

set +xe
echo "-------------------------"
set -xe

go build goroutines.go
./goroutines
