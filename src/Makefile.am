ACLOCAL_AMFLAGS = -I ../m4

# Define the target name. 
# NOTE: 'hts++' is not valid here because it contains '+'.
# Use 'hts' or 'htspp' instead.
bin_PROGRAMS = htspp

# Define the source file
htspp_SOURCES = HtsPlusPlus.cpp Option.cpp OptionFlag.cpp GetOpt.cpp HtsFile.cpp

# Add the flags defined by configure (HTSLIB_*)
htspp_CPPFLAGS = @HTSLIB_CPPFLAGS@
htspp_LDFLAGS = @HTSLIB_LDFLAGS@
htspp_LDADD = @HTSLIB_LIBS@
