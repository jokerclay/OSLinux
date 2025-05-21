#/bin/bash

set -xe

gcc  process_suspend_and_resume.c -o process_suspend_and_resume
sudo ./process_suspend_and_resume
