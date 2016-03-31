#!/usr/bin/env python
# encoding: utf-8

if __name__=="__main__":
    f=open('alb-se.sam')
    f2=open('result','w')
    for line in f.readlines():
        s=line.split()
        f2.write(s[0])
        f2.write(' ')
        f2.write(s[3])
        f2.write(' ')
        f2.write(s[4])
        f2.write('\n')

