CC = g++
##CPPFLAGS = -g -Wall -Wno-sign-compare -Wno-format
#CPPFLAGS = -g -Wall -Wno-format
CPPFLAGS = -O9 -Wall -Wno-sign-compare -Wno-format
DFLAGS = -DHAVE_PTHREAD
LIBS = -lm -lz -lpthread

main:main.o utils.o csaindex.o csa.o bntseq.o csagap.o alignment.o samout.o bamlite.o readsio.o ksw.o singleend.o kstring.o pairend.o
	$(CC) $(CPPFLAGS) $(DFLAGS) main.o utils.o csaindex.o bntseq.o csa.o csagap.o alignment.o samout.o bamlite.o readsio.o ksw.o singleend.o kstring.o pairend.o csa/csa.a $(LIBS) -L. -o main
main.o:main.cpp
	$(CC) -c $(CPPFLAGS) $(DFLAGS) main.cpp
csa.o:alignment/csa.cpp alignment/csa.h
	$(CC) -c $(CPPFLAGS) $(DFLAGS) alignment/csa.cpp
csaindex.o:index/csaindex.c index/csaindex.h
	$(CC) -c $(CPPFLAGS) $(DFLAGS) index/csaindex.c
bntseq.o:seqio/bntseq.c seqio/bntseq.h
	$(CC) -c $(CPPFLAGS) $(DFLAGS) seqio/bntseq.c
alignment.o:alignment/alignment.c alignment/alignment.h
	$(CC) -c $(CPPFLAGS) $(DFLAGS) alignment/alignment.c
csagap.o:alignment/csagap.c alignment/csagap.h
	$(CC) -c $(CPPFLAGS) $(DFLAGS) alignment/csagap.c
samout.o:sam/samout.c sam/samout.h
	$(CC) -c $(CPPFLAGS) $(DFLAGS) sam/samout.c
bamlite.o:seqio/bamlite.c seqio/bamlite.h
	$(CC) -c $(CPPFLAGS) $(DFLAGS) seqio/bamlite.c
readsio.o:seqio/readsio.c
	$(CC) -c $(CPPFLAGS) $(DFLAGS) seqio/readsio.c
ksw.o:kseq/ksw.c kseq/ksw.h
	$(CC) -c $(CPPFLAGS) $(DFLAGS) kseq/ksw.c
kstring.o:kseq/kstring.c kseq/kstring.h
	$(CC) -c $(CPPFLAGS) $(DFLAGS) kseq/kstring.c
singleend.o:sam/singleend.c sam/singleend.h
	$(CC) -c $(CPPFLAGS) $(DFLAGS) sam/singleend.c
pairend.o:sam/pairend.c
	$(CC) -c $(CPPFLAGS) $(DFLAGS) sam/pairend.c
utils.o:utils.c utils.h
	$(CC) -c $(CPPFLAGS) $(DFLAGS) utils.c

clean:
	rm *.o main
