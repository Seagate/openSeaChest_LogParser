# Hand Written Makefile (Edit with caution) -Muhammad
# 
# Last Modified: 9/05/18
#
# Seagate Technology 2018
#
UTIL_SRC_DIR=../../utils/src
UTIL_DIR=../../src
CXX = g++
STRIP = strip
TOOL_CHAIN=gcc
LIB_FILE_OUTPUT_DIR=lib
CXXFLAGS = -Wall -std=c++98
CFLAGS = -Wall 
CXXLAGS_I686 = -Wall -m32
ARCHIVE= ar
ARCH=x86_$(shell getconf LONG_BIT)

LFLAGS = \
	../../libjson/libjson.a \
	../../opensea-common/Make/$(TOOL_CHAIN)/$(LIB_FILE_OUTPUT_DIR)/libopensea-common.a \
	../../opensea-parser/Make/$(TOOL_CHAIN)/$(LIB_FILE_OUTPUT_DIR)/libopensea-parser.a

INC_DIR= \
	-I../../utils/include \
	-I../../opensea-common/include \
	-I../../opensea-parser/include \
	-I../../opensea-parser/include/Seagate\
	-I../../libjson 
	

MACHINE := $(shell uname -m)
ifeq ($(MACHINE), x86_64)

endif

OS := $(shell uname)
ifeq ($(OS),Darwin)
# Run MacOS commands
else
LFLAGS += -lrt
endif


#Files for the final binary
SOURCES = $(UTIL_DIR)/openSeaChest_LogParser.cpp\
		 $(UTIL_SRC_DIR)/EULA.cpp\
		 $(UTIL_SRC_DIR)/parser_print_util_options.cpp\
		 $(UTIL_SRC_DIR)/seachest_parser_util_options.cpp\

OUTFILE = openSeaChest_LogParser_$(ARCH)
FARM_OUTFILE = openSeaChest_LogParser_FARM_$(ARCH)
OBJS = $(SOURCES:.cpp=.o)

#add any defines needed for tool release. 
PROJECT_DEFINES = -DSEACHEST\
                  -DDISABLE_NVME_PASSTHROUGH\
                  -DINCLUDE_FARM_LOG\
                  -DINCLUDE_ERROR_LOG\
                  -DINCLUDE_DEVICE_STATISTICS_LOG\
                  -DINCLUDE_EXT_COMPREHENSIVE_LOG\
                  -DINCLUDE_COMMON_EXT_DST_LOG\
                  -DINCLUDE_POWER_CONDITION_LOG\
                  -DINCLUDE_IDENTIFY_LOG\
                  -DINCLUDE_IDENTIFY_DEVICE_DATA_LOG\
                  -DINCLUDE_SCT_TEMP_LOG\
		  -DINCLUDE_NCQ_CMD_ERROR_LOG\
		  -DINCLUDE_SCSI_LOG_PAGES\

.PHONY: all

farm: PROJECT_DEFINES += -DBUILD_FARM_ONLY

debug: CXXFLAGS += -g 
debug: CFLAGS += -g 
debug: PROJECT_DEFINES += -D_DEBUG 
debug: OUTFILE := $(OUTFILE)_dbg
debug: $(OUTFILE)

release: CXXFLAGS += -O3
release: CFLAGS += -O3
release: $(OUTFILE)

export CFLAGS
export CXXFLAGS 
export PROJECT_DEFINES 

all: clean debug

seaclibs:
	$(MAKE) -C ../../opensea-common/Make/$(TOOL_CHAIN)

opensea-parser:
	$(MAKE) -C ../../opensea-parser/Make/$(TOOL_CHAIN)

libjson:
	$(MAKE) -C ../../libjson/ CXXFLAGS=''

$(OUTFILE): libjson seaclibs opensea-parser $(OBJS)
	$(CXX) $(INC_DIR) $(CXXFLAGS) $(PROJECT_DEFINES) $(OBJS) -o $@ $(LFLAGS)

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $(INC_DIR) $(PROJECT_DEFINES) $< -o $@

static:	release 
	$(CXX) $(INC_DIR) $(PROJECT_DEFINES) $(OBJS) -static -o $(OUTFILE)_static $(LFLAGS)

strip: 
	$(STRIP) -s $(OUTFILE) -o $(OUTFILE)__stripped


farm: clean release 
	mv $(OUTFILE) $(FARM_OUTFILE)
clean:
	rm -f *.o *.a openSeaChest_LogParser*
	rm -f $(UTIL_SRC_DIR)/*.o
	rm -f $(UTIL_SRC_DIR)/utils/*.o
	rm -f $(UTIL_DIR)/*.o

clean_all: clean
	$(MAKE) -C ../../opensea-common/Make/$(TOOL_CHAIN) clean
	$(MAKE) -C ../../opensea-parser/Make/$(TOOL_CHAIN) clean
	$(MAKE) -C ../../libjson/ clean CXXFLAGS=''

