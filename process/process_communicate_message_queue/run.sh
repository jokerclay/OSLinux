#/bin/bash

set -xe

gcc -ggdb   process_communicate_message_queue.c -o process_communicate_message_queue

./process_communicate_message_queue

