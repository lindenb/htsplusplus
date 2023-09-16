HTSLIB?=../../htslib
CFLAGS=-I$(HTSLIB) -c -Wall -g
LDFLAGS=-L$(HTSLIB)  -lz -lm -lbz2 -llzma -lcurl -lpthread
CC=g++

%.o: %.cpp
	$(CC) -c -o $@ $(CFLAGS) $<


tests: hts++
	./hts++ tests -D ../tests

hts++: $(sort $(addsuffix .o, bcfprune htsplusplus samviewwithmate RegIdx HtsFile  BcfHeader BcfReader SamFileHeader utils tests)) $(HTSLIB)/libhts.a
	$(CC) -o $@ $(filter %.o,$^) $(filter %.a,$^) $(LDFLAGS)



bcfprune.o: bcfprune.cpp programs.hh
	$(CC) -c -o $@ $(CFLAGS) $<

samcount.o: samcount.c
	$(CC) -c -o $@ $(CFLAGS) $<

bcfexludebed.o: bcfexludebed.c
	$(CC) -c -o $@ $(CFLAGS) $<

htsplusplus.o: htsplusplus.cpp  programs.hh
	$(CC) -c -o $@ $(CFLAGS) $<


programs.hh: programs2C.xsl programs.tmp.xml git.hh
		xsltproc --output $@ programs2C.xsl programs.tmp.xml

programs.tmp.xml : programs.xml  programs.enrich.xsl
	xsltproc programs.enrich.xsl $< |\
		xsltproc programs.enrich.xsl - |\
		xsltproc --output "$@" programs.enrich.xsl -

git.hh:
	echo  '#ifndef GIT_VERSION' > $@
	echo -n  '#define GIT_VERSION "' >> $@
	-git rev-parse HEAD | tr -d '\n' >> $@
	echo '"' >> $@
	echo '#endif' >> $@


clean:
	rm -f *.o hts++ git.hh programs.tmp.xml
