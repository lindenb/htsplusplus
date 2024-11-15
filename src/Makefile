HTSDIR?=../../htslib
HTSLIB_CPPFLAGS?= -I$(HTSDIR)
CFLAGS=-I. $(HTSLIB_CPPFLAGS) -c -Wall -g -std=c++11
EXTRA_CFLAGS_PIC = -fpic
LDFLAGS=$(HTSLIB_LDFLAGS)  -lz -lm -lbz2 -llzma -lcurl -lpthread
LEX_YACC_OBJ=_main .tab .lex.yy
CC=g++


define LEXYACC


$(1).tab.o: grammars/$(1)/$(1).tab.cpp grammars/$(1)/$(1).tab.hpp grammars/$(1)/$(1).hh grammars/$(1)/$(1).lex.yy.hh
	$(CC) $(CFLAGS) -o $$@ $$<

$(1).lex.yy.o: grammars/$(1)/$(1).lex.yy.cpp grammars/$(1)/$(1).hh
	$(CC) $(CFLAGS) -o $$@ $$<

grammars/$(1)/$(1).lex.yy.hh: grammars/$(1)/$(1).lex.yy.cpp
	touch -c $$@

grammars/$(1)/$(1).lex.yy.cpp: grammars/$(1)/$(1).l grammars/$(1)/$(1).hh grammars/$(1)/$(1).tab.hpp
	flex  --prefix=$(1) --header-file=$$(addsuffix .hh,$$(basename $$@)) --outfile=$$@  $$<

grammars/$(1)/$(1).tab.hpp: grammars/$(1)/$(1).tab.cpp
	touch -c $$@

grammars/$(1)/$(1).tab.cpp :grammars/$(1)/$(1).y grammars/$(1)/$(1).hh
	bison  --xml=bison.xml --verbose --report-file=bison.report.txt --output=$$@ -d $$<


endef


API_OBJECTS=SamFileReaderFactory SamFileReader HtsFile SamFileHeader HtsIdx Path KString Regex  SamRecordIterator Interval

OBJECTS=bcfprune htsplusplus samviewwithmate RegIdx HtsFile  BcfHeader BcfReader SamFileHeader HtsThreadPool utils tests Regex \
	$(addprefix selgt,$(LEX_YACC_OBJ)) \
	$(addprefix distance_parser,$(LEX_YACC_OBJ)) \
	SamSequenceDictionary \
	SamFileReader \
	Interval \
	Cigar \
	Faidx \
	bcfhead \
    ContigPos \
	SequenceOntology \
	bcfconcat \
	samrmdupnames \
	bcf2intervals \
	bcfstreamuntil \
	fastqsplitnfiles


%.o: %.cpp
	$(CC) -c -o $@ $(CFLAGS) $<
%.pico:%.cpp
	$(CC) -c -o $@ $(CFLAGS) $(EXTRA_CFLAGS_PIC) $<

libhts++.so: $(sort $(addsuffix .pico,$(API_OBJECTS)))
	gcc -shared -fvisibility=hidden -o $@ $^ $(LDFLAGS) -lhts

all: ../bin/hts++ ../README.md tests2

../bin/hts++: $(sort $(addsuffix .o,$(OBJECTS))) $(HTSDIR)/libhts.a
	mkdir -p $(dir $@)
	$(CC) -o $@ $(filter %.o,$^) $(filter %.a,$^) $(LDFLAGS)

tests.o: tests.cpp SamFilterApp.hh programs.hh debug.hh utils.hh
	$(CC) -c -o $@ $(CFLAGS) $<

samviewwithmate.o: samviewwithmate.cpp SamFilterApp.hh programs.hh debug.hh SamRecord.hh
	$(CC) -c -o $@ $(CFLAGS) $<

samrmdupnames.o: samrmdupnames.cpp SamFilterApp.hh programs.hh debug.hh SamRecord.hh
	$(CC) -c -o $@ $(CFLAGS) $<

SamFilterApp.hh: SamFileReader.hh SamFileWriter.hh HtsThreadPool.hh

SamFileWriter.hh: HtsThreadPool.hh

SamFileReader.o : SamFileReader.cpp SamFileReader.hh HtsThreadPool.hh SamRecord.hh
	$(CC) -c -o $@ $(CFLAGS) $<

SamFileReaderFactory.o : SamFileReaderFactory.cpp SamFileReaderFactory.hh SamFileReader.hh
	$(CC) -c -o $@ $(CFLAGS) $<

HtsFile.o:  HtsFile.cpp HtsFile.hh
	$(CC) -c -o $@ $(CFLAGS) $<


Cigar.o: Cigar.cpp Cigar.hh
	$(CC) -c -o $@ $(CFLAGS) $<

Regex.o : Regex.cpp Regex.hh debug.hh utils.hh
	$(CC) -c -o $@ $(CFLAGS) $<

ContigPos.o: ContigPos.cpp ContigPos.hh Locatable.hh Comparable.hh
	$(CC) -c -o $@ $(CFLAGS) $<

Interval.o: Interval.cpp Interval.hh Locatable.hh Comparable.hh
	$(CC) -c -o $@ $(CFLAGS) $<

bcfprune.o: bcfprune.cpp programs.hh
	$(CC) -c -o $@ $(CFLAGS) $<

bcfhead.o: bcfhead.cpp programs.hh
	$(CC) -c -o $@ $(CFLAGS) $<


samcount.o: samcount.c
	$(CC) -c -o $@ $(CFLAGS) $<

bcf2intervals.o: bcf2intervals.cpp programs.hh  BcfReader.hh BcfRecord.hh
	$(CC) -c -o $@ $(CFLAGS) $<

bcfstreamuntil.o: bcfstreamuntil.cpp programs.hh  BcfReader.hh BcfRecord.hh
	$(CC) -c -o $@ $(CFLAGS) $<

bcfconcat.o: bcfconcat.cpp programs.hh BcfWriter.hh BcfHeader.hh BcfReader.hh BcfRecord.hh
	$(CC) -c -o $@ $(CFLAGS) $<

fastqsplitnfiles.o: fastqsplitnfiles.cpp programs.hh KSeq.hh
	$(CC) -c -o $@ $(CFLAGS) $<

selgt_main : selgt_main.cpp grammars/selgt/selgt.tab.hpp grammars/selgt/selgt.hh selgt.lex.yy.hh BcfReader.hh BcfWriter.hh programs.hh debug.hh
	$(CC) -c -o $@ $(CFLAGS) $<

bcfexludebed.o: bcfexludebed.c
	$(CC) -c -o $@ $(CFLAGS) $<

htsplusplus.o: htsplusplus.cpp  programs.hh
	$(CC) -c -o $@ $(CFLAGS) $<


programs.hh: programs2C.xsl programs.tmp.xml git.hh
		xsltproc --output $@ programs2C.xsl programs.tmp.xml



$(eval $(call LEXYACC,selgt))
$(eval $(call LEXYACC,distance_parser))


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
	../bin/hts++ bcfprune --distance 100kb ../tests/variants.01.bcf > /dev/null
	#$< samrmdupnames 

clean-all: clean
	rm -f so-simple.owl SequenceOntology.cpp
clean:
	rm -f *.o ../bin/hts++ git.hh programs.tmp.xml
