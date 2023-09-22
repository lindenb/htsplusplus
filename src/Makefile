HTSLIB?=../../htslib
CFLAGS=-I$(HTSLIB) -c -Wall -g -std=gnu++0x
LDFLAGS=-L$(HTSLIB)  -lz -lm -lbz2 -llzma -lcurl -lpthread
CC=g++

OBJECTS=bcfprune htsplusplus samviewwithmate RegIdx HtsFile  BcfHeader BcfReader SamFileHeader HtsThreadPool utils tests Regex selgt_main selgt.tab selgt.lex.yy \
	SamSequenceDictionary \
	Faidx \
	SequenceOntology \
	bcfconcat


%.o: %.cpp
	$(CC) -c -o $@ $(CFLAGS) $<


all: ../bin/hts++ ../README.md tests2

../bin/hts++: $(sort $(addsuffix .o,$(OBJECTS))) $(realpath $(HTSLIB)/libhts.a)
	mkdir -p $(dir $@)
	$(CC) -o $@ $(filter %.o,$^) $(filter %.a,$^) $(LDFLAGS)

tests.o: tests.cpp SamFilterApp.hh programs.hh debug.hh utils.hh
	$(CC) -c -o $@ $(CFLAGS) $<

samviewwithmate.o: samviewwithmate.cpp SamFilterApp.hh programs.hh debug.hh
	$(CC) -c -o $@ $(CFLAGS) $<

SamFilterApp.hh: SamFileReader.hh SamFileWriter.hh HtsThreadPool.hh

SamFileWriter.hh: HtsThreadPool.hh

SamFileReader.o : SamFileReader.hh HtsThreadPool.hh
	$(CC) -c -o $@ $(CFLAGS) $<

Regex.o : Regex.cpp Regex.hh debug.hh utils.hh
	$(CC) -c -o $@ $(CFLAGS) $<

bcfprune.o: bcfprune.cpp programs.hh
	$(CC) -c -o $@ $(CFLAGS) $<

samcount.o: samcount.c
	$(CC) -c -o $@ $(CFLAGS) $<

bcfconcat.o: bcfconcat.cpp programs.hh BcfWriter.hh BcfHeader.hh BcfReader.hh BcfRecord.hh
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

../README.md : programs2md.xsl programs.tmp.xml
	xsltproc --output "$@" $^

programs.tmp.xml : programs.xml  programs.enrich.xsl
	xsltproc programs.enrich.xsl $< |\
		xsltproc programs.enrich.xsl - |\
		xsltproc --output "$@" programs.enrich.xsl -


SequenceOntology.cpp : so2c.xsl
	wget -O so-simple.owl "https://raw.githubusercontent.com/The-Sequence-Ontology/SO-Ontologies/master/Ontology_Files/so-simple.owl"
	xsltproc so2c.xsl so-simple.owl |  sed  's%,"http://purl.obolibrary.org/obo/%,PURL "%g'  > $@
	rm so-simple.owl

git.hh:
	echo  '#ifndef GIT_VERSION' > $@
	echo -n  '#define GIT_VERSION "' >> $@
	-git rev-parse HEAD | tr -d '\n' >> $@
	echo '"' >> $@
	echo '#endif' >> $@


tests: ../bin/hts++
	 ../bin/hts++ tests -D ../tests
	
tests2 : ../bin/hts++
	@echo "## RUNNING TESTS ##"
	../bin/hts++ bcfconcat ../tests/variants.01.bcf ../tests/variants.01.bcf ../tests/variants.01.bcf > /dev/null

clean:
	rm -f *.o ../bin/hts++ git.hh programs.tmp.xml \
			SequenceOntology.cpp so-simple.owl
