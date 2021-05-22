#!/bin/bash

while :
do
    kill -9 `ps a | grep httpd | awk '{print 1;}'`
    ./httpd
    sleep 300
done
