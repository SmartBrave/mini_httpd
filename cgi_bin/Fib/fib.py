#!/usr/bin/env python
#coding=utf-8

import sys

def fib(x):
    "function fib"
    if(x==0 or x==1):
        return x;
    else:
        return fib(x-1)+fib(x-2)

x=fib(10)
print "<html><h1>"
print "<form action=\"../index.html\">"
print "<input type=\"submit\" value=\"goto home\"/><br/>"
print "</form>"
print x,"</h1></html>"

