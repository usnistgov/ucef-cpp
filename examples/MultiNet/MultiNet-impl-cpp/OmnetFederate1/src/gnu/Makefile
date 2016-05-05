#
# OMNeT++/OMNEST Makefile for MapperEx
#
# This file was generated with the command:
#  opp_makemake -f --deep -O out -I/opt/portico/portico-2.0.2/include/hla13 -I/home/c2wt/Projects/c2wt/generated/core/cpp -I/home/c2wt/Projects/c2wt/generated/core/cpp/include -I/home/c2wt/Projects/c2wt/core/src/cpp/include -I/home/c2wt/Projects/c2wt/generated/MapperEx/cpp/MapperEx/include -I/home/c2wt/Projects/c2wt/generated/MapperEx/cpp/MapperEx -I/home/c2wt/Projects/c2wt/core/src/cpp/SynchronizedFederate -I/home/c2wt/Projects/c2wt/core/src/cpp/OmnetFederate -L/opt/portico/portico-2.0.2/lib/gcc4 -L/home/c2wt/Projects/c2wt/core/src/cpp/SynchronizedFederate/Debug -L/home/c2wt/Projects/c2wt/core/src/cpp/OmnetFederate/Debug -L/home/c2wt/Projects/c2wt/core/src/cpp/OmnetFederate/out/$$\(CONFIGNAME\) -L/opt/inetmanet/inetmanet-2.2/out/$$\(CONFIGNAME\)/src -L/home/c2wt/Projects/c2wt/core/src/cpp/SynchronizedFederate/out/$$\(CONFIGNAME\) -lmysqlcppconn -lboost_system -lboost_regex -lboost_thread -lFedTime -lRTI-NG -linet -lOmnetFederate -lSynchronizedFederate -DINET_IMPORT -KOMNETFEDERATE_PROJ=/home/c2wt/Projects/c2wt/core/src/cpp/OmnetFederate -KINETMANET_2_2_PROJ=/opt/inetmanet/inetmanet-2.2 -KSYNCHRONIZEDFEDERATE_PROJ=/home/c2wt/Projects/c2wt/core/src/cpp/SynchronizedFederate
#

# Name of target to be created (-o option)
TARGET = MultiNet$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I$(PORTICO_PROJ)/include/hla13 \
    -I$(C2WTROOT_PROJ)/generated/core/cpp \
    -I$(C2WTROOT_PROJ)/generated/core/cpp/include \
    -I$(C2WTROOT_PROJ)/core/src/cpp/include \
    -I$(C2WTROOT_PROJ)/generated/MapperEx/cpp/MapperEx/include \
    -I$(C2WTROOT_PROJ)/generated/MapperEx/cpp/MapperEx \
    -I$(SYNCHRONIZEDFEDERATE_PROJ) \
    -I$(OMNETFEDERATE_PROJ) \
    -I$(INET_PROJ)/src/networklayer/common \
    -I$(OMNETFEDERATE_PROJ) \
    -I$(INET_PROJ)/src/networklayer/ipv4 \
    -I$(INET_PROJ)/src/networklayer/ipv6/contract \
    -I$(INET_PROJ)/src/linklayer/radio \
    -I$(INET_PROJ)/src/world/obstacles \
    -I$(INET_PROJ)/src/battery/models \
    -I$(INET_PROJ)/src/linklayer/ieee80211/mac \
    -I$(INET_PROJ)/src/linklayer/ieee80211/radio/errormodel \
    -I$(OMNETFEDERATE_PROJ)/messages \
    -I$(INET_PROJ)/src/linklayer/radio/propagation \
    -I$(INET_PROJ)/src/networklayer/ipv6tunneling \
    -I$(INET_PROJ)/src/linklayer/ieee80211/mgmt \
    -I$(INET_PROJ)/src/world/radio \
    -I$(INET_PROJ)/src/networklayer/icmpv6 \
    -I$(INET_PROJ)/src/networklayer/queue \
    -I$(INET_PROJ)/src/networklayer/arp \
    -I$(INET_PROJ)/src/networklayer/ipv6 \
    -I$(INET_PROJ)/src/linklayer/ethernet \
    -I$(INET_PROJ)/src/networklayer/contract \
    -I$(INET_PROJ)/src/util/headerserializers/tcp \
    -I$(INET_PROJ)/src/transport/udp/contract \
    -I$(INET_PROJ)/src/mobility \
    -I$(INET_PROJ)/src/util/headerserializers/udp \
    -I$(INET_PROJ)/src/transport/sctp \
    -I$(INET_PROJ)/src/applications/dhcp \
    -I$(INET_PROJ)/src/util/headerserializers/sctp \
    -I$(INET_PROJ)/src/applications/pingapp \
    -I$(INET_PROJ)/src/transport/udp \
    -I$(INET_PROJ)/src/world/powercontrol \
    -I$(INET_PROJ)/src/networklayer/manetrouting/base \
    -I$(INET_PROJ)/src/transport/tcp_common \
    -I$(INET_PROJ)/src/linklayer/contract \
    -I$(INET_PROJ)/src/networklayer/ipv4/contract \
    -I$(INET_PROJ)/src/base \
    -I$(INET_PROJ)/src/networklayer/xmipv6 \
    -I$(INET_PROJ)/src/linklayer/ieee80211/radio \
    -I$(INET_PROJ)/src/util \
    -I$(INET_PROJ)/src/util/headerserializers/ipv4 \
    -I$(INET_PROJ)/src/transport/sctp/contract \
    -I$(INET_PROJ)/src/util/headerserializers \
    -I$(INET_PROJ)/src \
    -I. \
    -IOmnetFederate \
    -IOmnetFederate/logs \
    -IOmnetFederate/results

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = -L$(PORTICO_PROJ)/lib/gcc4 -L$(INET_PROJ)/out/$(CONFIGNAME)/src -L$(SYNCHRONIZEDFEDERATE_PROJ) -L$(SYNCHRONIZEDFEDERATE_PROJ) -L$(OMNETFEDERATE_PROJ) -L$(OMNETFEDERATE_PROJ)/out/$(CONFIGNAME) -L$(FILTER_PROJ) -L$(C2WCONSOLELOGGER_PROJ) -lmysqlcppconn -lboost_system -lboost_thread -lboost_regex -lRTI-NG64 -lFedTime64 -linet -lOmnetFederate-$(C2WVERSION) -linet -lSynchronizedFederate-$(C2WVERSION) -lMapperEx-omnet-cpp-$(MODELVERSION) -lC2WConsoleLogger-$(C2WVERSION)
LIBS += -Wl,-rpath,`abspath $(PORTICO_PROJ)/lib/gcc4` -Wl,-rpath,`abspath $(SYNCHRONIZEDFEDERATE_PROJ)` -Wl,-rpath,`abspath $(OMNETFEDERATE_PROJ)` -Wl,-rpath,`abspath $(INET_PROJ)/out/$(CONFIGNAME)/src` -Wl,-rpath,`abspath $(OMNETFEDERATE_PROJ)` -Wl,-rpath,`abspath $(SYNCHRONIZEDFEDERATE_PROJ)` -Wl,-rpath,`abspath $(FILTER_PROJ)` -Wl,-rpath,`abspath $(C2WCONSOLELOGGER_PROJ)`

# Output directory
PROJECT_OUTPUT_DIR = ../target/bin
PROJECTRELATIVE_PATH =
O = $(PROJECT_OUTPUT_DIR)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = 
#$O/OmnetFederate/MapperExFilterInit.o

# Message files
MSGFILES =

# Other makefile variables (-K)
INET_PROJ=$(INETMANET_HOME)
FILTER_PROJ=$(PROJECTTARGET)/nar/MultiNet-omnet-cpp-$(MODELVERSION)-$(PROJECTAOL)-static/lib/$(PROJECTAOL)/static/
OMNETFEDERATE_PROJ=$(PROJECTTARGET)/nar/OmnetFederate-$(C2WVERSION)-$(PROJECTAOL)-shared/lib/$(PROJECTAOL)/shared/
SYNCHRONIZEDFEDERATE_PROJ=$(PROJECTTARGET)/nar/SynchronizedFederate-$(C2WVERSION)-$(PROJECTAOL)-shared/lib/$(PROJECTAOL)/shared/
C2WCONSOLELOGGER_PROJ=$(PROJECTTARGET)/nar/C2WConsoleLogger-$(C2WVERSION)-$(PROJECTAOL)-shared/lib/$(PROJECTAOL)/shared/
PORTICO_PROJ=$(RTI_HOME)
C2WTROOT_PROJ=$(C2WTROOT)

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc or configuser.vc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIB_SUBDIR = $(OMNETPP_LIB_DIR)/$(TOOLCHAIN_NAME)
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" -loppmain$D $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS) -DINET_IMPORT $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE and have object
# files depend on it (except when "make depend" was called)
COPTS_FILE = $O/.last-copts
ifneq ($(MAKECMDGOALS),depend)
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
$(shell $(MKPATH) "$O" && echo "$(COPTS)" >$(COPTS_FILE))
endif
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $O/$(TARGET)
	$(Q)$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	@echo Creating executable: $@
	$(Q)$(CXX) $(LDFLAGS) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

install: all

.PHONY: all clean cleanall depend msgheaders

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE)
	@$(MKPATH) $(dir $@)
	$(qecho) "$<"
	$(Q)$(CXX) -c $(CXXFLAGS) $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(qecho) MSGC: $<
	$(Q)$(MSGC) -s _m.cc $(MSGCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

clean:
	$(qecho) Cleaning...
	$(Q)-rm -rf $O
	$(Q)-rm -f MapperEx MapperEx.exe libMapperEx.so libMapperEx.a libMapperEx.dll libMapperEx.dylib
	$(Q)-rm -f ./*_m.cc ./*_m.h
	$(Q)-rm -f OmnetFederate/*_m.cc OmnetFederate/*_m.h
	$(Q)-rm -f OmnetFederate/logs/*_m.cc OmnetFederate/logs/*_m.h
	$(Q)-rm -f OmnetFederate/results/*_m.cc OmnetFederate/results/*_m.h

cleanall: clean
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(qecho) Creating dependencies...
	$(Q)$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc OmnetFederate/*.cc OmnetFederate/logs/*.cc OmnetFederate/results/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
#$O/OmnetFederate/MapperExFilterInit.o: OmnetFederate/MapperExFilterInit.cc \
#	OmnetFederate/MapperExFilterInit.h \
#	$(OMNETFEDERATE_PROJ)/OmnetFOMInteractions.h

