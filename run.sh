#!/bin/bash

#Compile, insert and remove the module
make
sudo insmod $1.ko processid=$2
sudo rmmod $1

#output the log file
tail -15 /var/log/kern.log

#clean files
rm .$1*
rm Module.symvers
rm modules.order
rm $1.ko
rm $1.mod.c
rm $1.mod.o
rm $1.o
rm -r .tmp_versions
