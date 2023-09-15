HTSLIB?=../htslib
CFLAGS=-I$(HTSLIB) -c -Wall -g
LDFLAGS=-L$(HTSLIB) -lhts
CC=g++

%.o: %.cpp
	$(CC) -c -o $@ $(CFLAGS) $<


hts++: $(sort $(addsuffix .o, bcfprune htsplusplus samviewwithmate RegIdx HtsFile  BcfHeader BcfReader SamFileHeader ))
	$(CC) -o $@ $^ $(LDFLAGS)

bcfprune.o: bcfprune.cpp programs.hh
	$(CC) -c -o $@ $(CFLAGS) $<

samcount.o: samcount.c
	$(CC) -c -o $@ $(CFLAGS) $<

bcfexludebed.o: bcfexludebed.c
	$(CC) -c -o $@ $(CFLAGS) $<

htsplusplus.o: htsplusplus.cpp  programs.hh
	$(CC) -c -o $@ $(CFLAGS) $<


programs.hh : programs.xml programs2C.xsl  programs.enrich.xsl
	xsltproc programs.enrich.xsl $< |\
		xsltproc programs.enrich.xsl - |\
		xsltproc programs.enrich.xsl - |\
		xsltproc --output $@ programs2C.xsl -

clean:
	rm -f *.o hts++ version.hh
