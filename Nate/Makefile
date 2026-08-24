PROJECT_NAME := Nate
TARGET_PATH := $(BIN_DIR)/Nate$(EXE_EXT)

DEFINES := _CONSOLE
INCLUDE_DIRS := ../NateLib/utf8 ../NateLib ../NateLib/parser ../NateLib/src ../NateLib/src/Stats ../NateLib/core/cpp ../MathParser
SOURCES := Nate.cpp
EXTRA_DEPS := $(NATE_LIB) $(MATHPARSER_LIB)
LDLIBS := "$(NATE_LIB)" "$(MATHPARSER_LIB)"
RUNTIME_IMPORT_INPUTS := $(wildcard ../NateLib/core/import/*.nd ../NateLib/core/import/*.ns)
RUNTIME_IMPORT_STAMP := $(GENERATED_INCLUDE_DIR)/runtime-imports-$(CONFIG).stamp

include ../build/common.mk

all: $(RUNTIME_IMPORT_STAMP)

$(RUNTIME_IMPORT_STAMP): $(TARGET_PATH) $(RUNTIME_IMPORT_INPUTS)
	$(MAKE) -C ../NateLib runtime-imports-update
