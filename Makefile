HTSLIB?=../htslib
CFLAGS=-I$(HTSLIB) -c -Wall -g
LDFLAGS=-L$(HTSLIB) -lhts
CC=g++



hts++: $(addsuffix .o, bcfprune htsplusplus RegIdx HtsFile  BcfHeader BcfReader)
	$(CC) -o $@ $^ $(LDFLAGS)
bcfprune.o: bcfprune.cpp programs.hh
	$(CC) -c -o $@ $(CFLAGS) $<

samcount.o: samcount.c
	$(CC) -c -o $@ $(CFLAGS) $<

bcfexludebed.o: bcfexludebed.c
	$(CC) -c -o $@ $(CFLAGS) $<

htsplusplus.o: htsplusplus.cpp  programs.hh
	$(CC) -c -o $@ $(CFLAGS) $<

RegIdx.o: RegIdx.cpp RegIdx.hh
	$(CC) -c -o $@ $(CFLAGS) $<

HtsFile.o: HtsFile.cpp HtsFile.hh
	$(CC) -c -o $@ $(CFLAGS) $<

BcfHeader.o: BcfHeader.cpp BcfHeader.hh
	$(CC) -c -o $@ $(CFLAGS) $<

BcfReader.o: BcfReader.cpp BcfReader.hh
	$(CC) -c -o $@ $(CFLAGS) $<

programs.hh : programs.xml programs2C.xsl  programs.enrich.xsl
	xsltproc programs.enrich.xsl $< |\
		xsltproc programs.enrich.xsl - |\
		xsltproc programs.enrich.xsl - |\
		xsltproc --output $@ programs2C.xsl -

clean:
	rm -f *.o hts++ version.hh
