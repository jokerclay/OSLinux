#/bin/bash

set -xe

gcc  process_sync_with_pipe.c -o process_sync_with_pipe
sudo ./process_sync_with_pipe
 
 

