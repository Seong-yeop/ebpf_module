#!/bin/bash

make
sudo insmod hello.ko #sym=18446744072266711712
sudo rmmod hello
dmesg
