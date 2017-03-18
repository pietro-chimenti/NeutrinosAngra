# Simple Makefile to use with a shared lib
# 
# Author: Pietro Chimenti, based on R.Brun template 16/10/2014

RC     := root-config
ifeq ($(shell which $(RC) 2>&1 | sed -ne "s@.*/$(RC)@$(RC)@p"),$(RC))
MKARCH := $(wildcard $(shell $(RC) --etcdir)/Makefile.arch)
RCONFIG := $(wildcard $(shell $(RC) --incdir)/RConfigure.h)
endif
ifneq ($(MKARCH),)
include $(MKARCH)
else
ifeq ($(ROOTSYS),)
ROOTSYS = ~/root
endif
include $(ROOTSYS)/etc/Makefile.arch
endif
#------------------------------------------------------------------------------

PULSEO        = AngraMCPulse.$(ObjSuf) AngraMCPulseDict.$(ObjSuf)
PULSES        = AngraMCPulse.$(SrcSuf) AngraMCPulseDict.$(SrcSuf)
PULSESO       = libAngraMCPulse.$(DllSuf)
PULSE         = AngraMCPulse$(ExeSuf)
ifeq ($(PLATFORM),win32)
PULSELIB      = libAngraMCPulse.lib
else
PULSELIB      = $(shell pwd)/$(PULSESO)
endif

PULSETREEO    = PulseTree.$(ObjSuf) 
PULSETREES    = PulseTree.$(SrcSuf)

MIXERTIMERO   = AngraMixerTimer.$(ObjSuf)
MIXERTIMERS   = AngraMixerTimer.$(SrcSuf)

ANGRAMIXERO   = AngraMCMixer.$(ObjSuf)
ANGRAMIXERS   = AngraMCMixer.$(SrcSuf)

TESTTIMERO    = TestTimer.$(ObjSuf)
TESTTIMERS    = TestTimer.$(SrcSuf)

TESTTIMER     = TestTimer$(ExeSuf)

MIXERO    = Mixer.$(ObjSuf)
MIXERS    = Mixer.$(SrcSuf)

TESTLISTO = TestList.$(ObjSuf)
TESTLISTS = TestList.$(SrcSuf)

TESTLIST  = TestList$(ExeSuf)

OBJS          = $(PULSEO) $(PULSETREEO) $(MIXERO) $(TESTLISTO) $(MIXERTIMERO) $(TESTTIMERO) $(ANGRAMIXERO)

PROGRAMS      = $(PULSE) $(TESTLIST) $(TESTTIMER)

ifeq ($(ARCH),aix5)
MAKESHARED    = /usr/vacpp/bin/makeC++SharedLib
endif

#------------------------------------------------------------------------------

.SUFFIXES: .$(SrcSuf) .$(ObjSuf) .$(DllSuf)
.PHONY:    

all:            $(PROGRAMS)

$(PULSESO):     $(PULSEO)
ifeq ($(ARCH),aix5)
		$(MAKESHARED) $(OutPutOpt) $@ $(LIBS) -p 0 $^
else
ifeq ($(PLATFORM),macosx)
# We need to make both the .dylib and the .so
		$(LD) $(SOFLAGS)$@ $(LDFLAGS) $^ $(OutPutOpt) $@ $(EXPLLINKLIBS)
ifneq ($(subst $(MACOSX_MINOR),,1234),1234)
ifeq ($(MACOSX_MINOR),4)
		ln -sf $@ $(subst .$(DllSuf),.so,$@)
endif
endif
else
ifeq ($(PLATFORM),win32)
		bindexplib $* $^ > $*.def
		lib -nologo -MACHINE:IX86 $^ -def:$*.def \
		   $(OutPutOpt)$(EVENTLIB)
		$(LD) $(SOFLAGS) $(LDFLAGS) $^ $*.exp $(LIBS) \
		   $(OutPutOpt)$@
		$(MT_DLL)
else
		$(LD) $(SOFLAGS) $(LDFLAGS) $^ $(OutPutOpt) $@ $(EXPLLINKLIBS)
endif
endif
endif
		@echo "$@ done"

$(PULSE):       $(PULSESO) $(MIXERO) $(PULSETREEO) $(ANGRAMIXERO) $(MIXERTIMERO)
		$(LD) $(LDFLAGS) $(MIXERO) $(PULSEO) $(PULSETREEO) $(MIXERTIMERO) $(ANGRAMIXERO) $(LIBS) $(OutPutOpt)$@
		$(MT_EXE)
		@echo "$@ done"

$(TESTLIST):    $(PULSESO) $(TESTLISTO)
		$(LD) $(LDFLAGS) $(TESTLISTO) $(PULSEO) $(LIBS) $(OutPutOpt)$@
		$(MT_EXE)
		@echo "$@ done"

$(TESTTIMER):   $(MIXERTIMERO) $(TESTTIMERO)
		$(LD) $(LDFLAGS) $(MIXERTIMERO) $(TESTTIMERO) $(LIBS) $(OutPutOpt)$@
		$(MT_EXE)
		@echo "$@ done"


ifeq ($(shell $(RC) --has-mathmore),yes)
ifeq ($(PLATFORM),win32)
EXTRAROOFITLIBS = '$(ROOTSYS)/lib/libMathMore.lib'
else
EXTRAROOFITLIBS = -lMathMore
endif
endif

clean:
		@rm -f $(OBJS) $(TRACKMATHSRC) core *Dict.*

distclean:      clean
		@rm -f $(PROGRAMS) $(PULSESO) $(PULSELIB) *Dict.* *.def *.exp \
		   *.root *.ps *.so *.lib *.dll *.d *.log .def so_locations \
		   files/* 
		@rm -rf cxx_repository

.SUFFIXES: .$(SrcSuf)

###

AngraMCPulse.$(ObjSuf): AngraMCPulse.h
PulseTree.$(ObjSuf): PulseTree.h
Mixer.$(ObjSuf): AngraMCPulse.h
TestList.$(ObjSuf): AngraMCPulse.h
AngraMCMixer.$(ObjSuf): AngraMCMixer.h
AngraMCPulseDict.$(SrcSuf): AngraMCPulse.h AngraMCPulseLinkDef.h
	@echo "Generating dictionary $@..."
	$(ROOTCINT) -f $@ -c $^


.$(SrcSuf).$(ObjSuf):
	$(CXX)  $(CXXFLAGS) -c $<
