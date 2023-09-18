HTSLIB?=../../htslib
CFLAGS=-I$(HTSLIB) -c -Wall -g -std=gnu++0x
LDFLAGS=-L$(HTSLIB)  -lz -lm -lbz2 -llzma -lcurl -lpthread
CC=g++

OBJECTS=bcfprune htsplusplus samviewwithmate RegIdx HtsFile  BcfHeader BcfReader SamFileHeader HtsThreadPool utils tests Regex selgt_main selgt.tab selgt.lex.yy \
	SamSequenceDictionary \
	Faidx


%.o: %.cpp
	$(CC) -c -o $@ $(CFLAGS) $<


hts++: $(sort $(addsuffix .o,$(OBJECTS))) $(realpath $(HTSLIB)/libhts.a)
	$(CC) -o $@ $(filter %.o,$^) $(filter %.a,$^) $(LDFLAGS)



samviewwithmate.cpp: SamFileHeader.hh SamFileReader.hh SamFileWriter.hh programs.hh debug.hh
SamFileReader.hh: HtsThreadPool.hh

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

selgt_main : selgt_main.cpp selgt.tab.hpp selgt.hh selgt.lex.yy.hh BcfReader.hh BcfWriter.hh programs.hh debug.hh
	$(CC) -c -o $@ $(CFLAGS) $<

selgt.tab.o: selgt.tab.cpp selgt.tab.hpp selgt.hh selgt.lex.yy.hh
	$(CC) $(CFLAGS) -o $@ $<

selgt.lex.yy.o: selgt.lex.yy.cpp selgt.hh
	$(CC) $(CFLAGS) -o $@ $<

selgt.lex.yy.hh: selgt.lex.yy.cpp
	touch -c $@

selgt.lex.yy.cpp: selgt.l selgt.hh selgt.tab.hpp
	flex  --prefix=selgt --header-file=$(addsuffix .hh,$(basename $@)) --outfile=$@  $<

selgt.tab.hpp: selgt.tab.cpp
	touch -c $@

selgt.tab.cpp :selgt.y selgt.hh
	bison  --xml=bison.xml --verbose --report-file=bison.report.txt --output=$@ -d $<


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
	
tests: hts++
	./hts++ samviewwithmate -B ../tests/viewwithmate.01.bed  ../tests/viewwithmate.01.sam
