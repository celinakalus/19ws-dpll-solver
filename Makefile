# Variables for compilers and tools
# (only set if not already set)
CXX ?= g++
MKDIR ?= mkdir


# Variables for input and output files and directories
INC_DIR ?= ./include
SRC_DIR ?= ./src
OBJ_DIR ?= ./obj
BIN_DIR ?= ./
PROG ?= dpll-solver

INCLUDE_PATHS += $(INC_DIR)


# Inclusion of external headers and libraries
#INCLUDE_PATHS += example/include/path
#LIBRARY_PATHS += example/library/path
#LIBRARY_NAMES += example_library


# generate compiler flags
CPPFLAGS += $(foreach inc,$(INCLUDE_PATHS),-I$(inc))
LIBRARIES += $(foreach lib,$(LIBRARY_PATHS),-L$(lib))
LIBRARIES += $(foreach lib,$(LIBRARY_NAMES),-l$(lib))


# List all source files, corresponding object files, and their directories
CPPSRC := $(shell find $(SRC_DIR) -name \*.cpp)
SRC := $(CPPSRC)

CPPOBJ := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.cpp.o,$(CPPSRC))
OBJ := $(CPPOBJ)

DIRS := $(sort $(dir $(OBJ)) $(dir $(BIN_DIR)/$(PROG)))


# Misc flags


# Targets
.PHONY: default release clean

default: release

release: 
	@$(MAKE) $(MAKEFLAGS) \
		SRC_DIR=$(SRC_DIR) \
		OBJ_DIR=$(OBJ_DIR) \
		BIN_DIR=$(BIN_DIR) \
		PROG=$(PROG) \
		$(BIN_DIR)/$(PROG)

$(BIN_DIR)/$(PROG): $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $+ $(LIBRARIES)

$(OBJ): | $(DIRS)

$(OBJ_DIR)/%.cpp.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CPPFLAGS) -o $@ -c $<

$(DIRS):
	@$(MKDIR) -p $@ 2> /dev/null; true

echo:
	@echo $(SRC)
	@echo $(OBJ)
	@echo $(DIRS)

clean:
	@rm $(OBJ) $(PROG) 2> /dev/null; true

