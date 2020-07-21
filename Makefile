HTSLIB?=../htslib
CFLAGS=-I$(HTSLIB) -c -Wall 
LDFLAGS=-L$(HTSLIB) -lhts
CC=gcc


bcfprune: bcfprune.o
	$(CC) -o $@ $< $(LDFLAGS)
bcfprune.o: bcfprune.c
	$(CC) -c -o $@ $(CFLAGS) $<

test: bcfprune test.bcf
	export LD_LIBRARY_PATH=$${PATH}:$(HTSLIB) && ./bcfprune -d 100 test.bcf | grep -w "^3"  | awk '{P=int($$2);if(NR>1 && P-PREV<100){print P-PREV; exit(-1)};PREV=P;}' 
	export LD_LIBRARY_PATH=$${PATH}:$(HTSLIB) && ./bcfprune -d 100 test.bcf | bcftools view - > /dev/null

clean:
	rm -f *.o bcfprune
