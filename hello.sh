#!/bin/bash

make
sudo insmod hello.ko
dmesg
sudo rmmod hello
