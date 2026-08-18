PROJECT_NAME := NateCompiler
TARGET_PATH := $(OUT_DIR)/NateCompiler$(EXE_EXT)

DEFINES := _CONSOLE
INCLUDE_DIRS := ../utf8 ../Nate ../Nate/Stats ../MathParser
SOURCES := Nate.cpp
EXTRA_DEPS := $(NATE_LIB) $(MATHPARSER_LIB)
LDLIBS := "$(NATE_LIB)" "$(MATHPARSER_LIB)"

include ../build/common.mk
