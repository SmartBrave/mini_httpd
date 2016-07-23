#!/bin/bash

for dir in `ls -F | grep '/'`;\
do
    echo $dir
    cd ./$dir;\
    make clean;\
    cd ..
done

git add *
git commit -m "`date +%Y-%m-%d-%H-%M-%S`"
git push origin master
