#
# Do NOT modify or remove this copyright and license
#
# Copyright (c) 2014 - 2020 Seagate Technology LLC and/or its Affiliates, All Rights Reserved
#
# This software is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# ******************************************************************************************
# 
# Hand Written Makefile (Edit with caution) -Muhammad
#
NAME=openSeaChest_LogParser
#Change the Major version when major interface changes are made.
MAJOR=0
#Change the Minor version when new features are added. 
MINOR=2
#Change the patch version when only bug fixes are made.
PATCH=4


UTIL_SRC_DIR=../../utils/src
UTIL_DIR=../../src
CXX ?= g++
STRIP ?= strip
TOOL_CHAIN=gcc
LIB_FILE_OUTPUT_DIR=lib
CXXFLAGS = -Wall -Wextra -Werror=old-style-cast -Wshadow -Wmaybe-uninitialized -Wvla -Wfloat-equal \
    -Wlogical-op -Wdouble-promotion -Wformat-security -std=c++11
CFLAGS ?= -Wall -Wextra
ARCHIVE ?= ar
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

#Add more warnings for the GCC versions that support them
#gcc 5.5 and up : -Wlogical-not-parentheses
#    6.5 and up : -Wnull-dereference -Wduplicated-cond -Wshift-overflow=2
ifeq ($(UNAME),Linux)
	GCC_VERSION_STRING = $(shell $(CC) -dumpversion)
	GCC_VER = $(subst ., ,$(GCC_VERSION_STRING))
	GCC_MAJOR = $(word 1,$(GCC_VER))
	GCC_MINOR = $(word 2,$(GCC_VER))
	GCC_SUBMINOR = $(word 3,$(GCC_VER))
	ifeq ($(GCC_MINOR),)
		GCC_MINOR = 0
	endif 
	ifeq ($(GCC_SUBMINOR),)
		GCC_SUBMINOR = 0
	endif
	ifeq ($(shell test $(GCC_MAJOR) -gt 4; echo $$?),0)
		ifeq ($(shell test $(GCC_MINOR) -gt 4; echo $$?),0)
			CXXFLAGS += -Wlogical-not-parentheses
		endif
	endif
	ifeq ($(shell test $(GCC_MAJOR) -gt 5; echo $$?),0)
		ifeq ($(shell test $(GCC_MINOR) -gt 4; echo $$?),0)
			CXXFLAGS += -Wnull-dereference -Wduplicated-cond -Wshift-overflow=2
		endif
	endif
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

$(OUTFILE): clean libjson seaclibs opensea-parser $(OBJS) done
	$(CXX) $(INC_DIR) $(CXXFLAGS) $(PROJECT_DEFINES) $(OBJS) -o $@ $(LFLAGS)

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $(INC_DIR) $(PROJECT_DEFINES) $< -o $@

static:	release 
	$(CXX) $(INC_DIR) $(PROJECT_DEFINES) $(OBJS) -static -o $(OUTFILE)_static $(LFLAGS)

strip: 
	$(STRIP) -s $(OUTFILE) -o $(OUTFILE)__stripped


farm: clean release 
	mv $(OUTFILE) $(FARM_OUTFILE)
clean: banner
	rm -f *.o *.a openSeaChest_LogParser*
	rm -f $(UTIL_SRC_DIR)/*.o
	rm -f $(UTIL_SRC_DIR)/utils/*.o
	rm -f $(UTIL_DIR)/*.o
	@echo "============================================================"
	@echo "       CLEANED"
	@echo "============================================================"

clean_all: clean
	$(MAKE) -C ../../opensea-common/Make/$(TOOL_CHAIN) clean
	$(MAKE) -C ../../opensea-parser/Make/$(TOOL_CHAIN) clean
	$(MAKE) -C ../../libjson/ clean CXXFLAGS=''

banner:
	@echo "============================================================"
	@echo "openSeaChest_LogParser Makefile version: "$(MAJOR).$(MINOR).$(PATCH)  "OS: "$(OS)
	@echo "============================================================"

done:
	@echo "============================================================"
	@echo "Build of openSeaChest-LogParser has completed "
	@echo "============================================================"

