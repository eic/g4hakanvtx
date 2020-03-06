AUTOMAKE_OPTIONS = foreign

# List of shared libraries to produce
lib_LTLIBRARIES = \
    libg4hakanvtx.la

AM_CPPFLAGS = \
    -I$(includedir) \
    -I$(OFFLINE_MAIN)/include \
    -I$(ROOTSYS)/include \
    -I$(G4_MAIN)/include \
    -I$(OPT_SPHENIX)/include

AM_LDFLAGS = \
    -L$(libdir) \
    -L$(OFFLINE_MAIN)/lib

libg4hakanvtx_la_LIBADD = \
  -lSubsysReco \
  -lg4detectors \
  -lg4testbench 

pkginclude_HEADERS = \
  G4HakanVtxSubsystem.h \
  SimpleNtuple.h

libg4hakanvtx_la_SOURCES = \
  G4HakanVtxDetector.cc \
  G4HakanVtxDisplayAction.cc \
  G4HakanVtxSubsystem.cc \
  G4HakanVtxSteppingAction.cc \
  SimpleNtuple.cc

################################################
# linking tests

BUILT_SOURCES = testexternals.cc

noinst_PROGRAMS = \
  testexternals_g4hakanvtx


testexternals_g4hakanvtx_SOURCES = testexternals.cc
testexternals_g4hakanvtx_LDADD = libg4hakanvtx.la


testexternals.cc:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@

clean-local:
	rm -f *Dict* $(BUILT_SOURCES) *.pcm
