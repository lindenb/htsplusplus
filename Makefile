HTSLIB?=../htslib
CFLAGS=-I$(HTSLIB) -c -Wall -std=c11
LDFLAGS=-L$(HTSLIB) -lhts
CC=g++



hts++: bcfprune.o htsplusplus.o RegIdx.o
	$(CC) -o $@ $^ $(LDFLAGS)
bcfprune.o: bcfprune.c
	$(CC) -c -o $@ $(CFLAGS) $<

samcount.o: samcount.c
	$(CC) -c -o $@ $(CFLAGS) $<

bcfexludebed.o: bcfexludebed.c
	$(CC) -c -o $@ $(CFLAGS) $<

htsplusplus.o: htsplusplus.cpp  programs.hh
	$(CC) -c -o $@ $(CFLAGS) $<

RegIdx.o: RegIdx.cpp RegIdx.hh
	$(CC) -c -o $@ $(CFLAGS) $<

programs.hh : programs.xml programs2C.xsl  programs.enrich.xsl
	xsltproc programs.enrich.xsl $< |\
		xsltproc programs.enrich.xsl - |\
		xsltproc programs.enrich.xsl - |\
		xsltproc --output $@ programs2C.xsl -


test: hts++ test.bcf
	export LD_LIBRARY_PATH=$${PATH}:$(HTSLIB) && ./hts++ bcfprune -d 100 test.bcf | grep -w "^3"  | awk '{P=int($$2);if(NR>1 && P-PREV<100){print P-PREV; exit(-1)};PREV=P;}' 
	export LD_LIBRARY_PATH=$${PATH}:$(HTSLIB) && ./hts++ bcfprune -d 100 test.bcf | bcftools view - > /dev/null
	export LD_LIBRARY_PATH=$${PATH}:$(HTSLIB) && ./hts++ bcfprune -d 100 < test.bcf | bcftools view - > /dev/null

clean:
	rm -f *.o hts++ version.hh
