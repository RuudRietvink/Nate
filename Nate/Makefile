PROJECT_NAME := Nate
TARGET_PATH := $(OUT_DIR)/Nate$(EXE_EXT)

DEFINES := _CONSOLE
INCLUDE_DIRS := ../NateLib/utf8 ../NateLib ../NateLib/parser ../NateLib/src ../NateLib/src/Stats ../NateLib/core/cpp ../MathParser
SOURCES := Nate.cpp
EXTRA_DEPS := $(NATE_LIB) $(MATHPARSER_LIB)
LDLIBS := "$(NATE_LIB)" "$(MATHPARSER_LIB)"

include ../build/common.mk
