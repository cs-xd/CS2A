#!/usr/bin/env python
# encoding: utf-8
import sys

if __name__=='__main__':
    filename=sys.argv[1]
    filename_processed=filename+'.strip'

    fp1=open(filename)
    fp2=open(filename_processed,'w')
    for line in fp1:
        if not line.startswith('>'):
            line=line.strip()
            fp2.write(line)
    fp1.close()
    fp2.close()
