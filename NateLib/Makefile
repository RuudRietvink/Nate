.DEFAULT_GOAL := all

PROJECT_NAME := NateLib
TARGET_KIND := lib
NATE_LIB ?= $(OUT_DIR)/$(LIB_PREFIX)NateLib$(STATIC_LIB_EXT)
NATE_RUNTIME_LIB ?= $(OUT_DIR)/$(LIB_PREFIX)$(if $(CONFIG_IS_DEBUG),nated,nate)$(STATIC_LIB_EXT)
NATE_COMPILER_EXE ?= ../$(PLATFORM)/$(CONFIG)/Nate$(EXE_EXT)
TARGET_PATH := $(NATE_LIB)

DEFINES :=
INCLUDE_DIRS := $(REFLEX_INCLUDE) parser src src/Stats . core/cpp ../MathParser utf8
RUNTIME_SOURCES := core/cpp/Core.cpp core/cpp/Rational.cpp
RUNTIME_IMPORT_NAMES := $(basename $(notdir $(filter %.ns,$(wildcard core/import/*.ns))))
RUNTIME_IMPORT_NAMES := $(foreach name,$(RUNTIME_IMPORT_NAMES),$(if $(wildcard core/import/$(name).nd),$(name),))
RUNTIME_IMPORT_SOURCES := $(foreach name,$(RUNTIME_IMPORT_NAMES),core/created/$(name).cpp)
RUNTIME_IMPORT_INPUTS := $(foreach name,$(RUNTIME_IMPORT_NAMES),core/import/$(name).nd core/import/$(name).ns)
RUNTIME_IMPORT_STAMP := core/created/runtime-imports-$(CONFIG).stamp
NATE_RUNTIME_PREREQS = $(NATE_RUNTIME_OBJECTS)
ifneq ($(wildcard $(NATE_COMPILER_EXE)),)
NATE_RUNTIME_PREREQS += $(RUNTIME_IMPORT_STAMP) $(RUNTIME_IMPORT_OBJECTS)
endif
SOURCES := \
	$(REFLEX_LIB_ROOT)/convert.cpp \
	$(REFLEX_LIB_ROOT)/debug.cpp \
	$(REFLEX_LIB_ROOT)/error.cpp \
	$(REFLEX_LIB_ROOT)/input.cpp \
	$(REFLEX_LIB_ROOT)/matcher.cpp \
	$(REFLEX_LIB_ROOT)/pattern.cpp \
	$(REFLEX_LIB_ROOT)/posix.cpp \
	$(REFLEX_LIB_ROOT)/unicode.cpp \
	$(REFLEX_LIB_ROOT)/utf8.cpp \
	$(REFLEX_UNICODE_ROOT)/block_scripts.cpp \
	$(REFLEX_UNICODE_ROOT)/language_scripts.cpp \
	$(REFLEX_UNICODE_ROOT)/letter_scripts.cpp \
	src/Code.cpp \
	src/Arg.cpp \
	core/cpp/Core.cpp \
	core/cpp/Rational.cpp \
	src/Define.cpp \
	src/Expr.cpp \
	src/Container.cpp \
	src/Identifier.cpp \
	parser/lex.yy.cpp \
	src/Location.cpp \
	src/NateCode.cpp \
	src/NateFunctions.cpp \
	src/NateParser.cpp \
	src/NateParserMath.cpp \
	src/Object.cpp \
	src/Property.cpp \
	src/Record.cpp \
	src/Scope.cpp \
	parser/parser.cpp \
	src/Stats/Stat.cpp \
	src/Stats/StatAssign.cpp \
	src/Stats/StatCode.cpp \
	src/Stats/StatData.cpp \
	src/Stats/StatDeclareLocal.cpp \
	src/Stats/StatDefine.cpp \
	src/Stats/StatError.cpp \
	src/Stats/StatExpr.cpp \
	src/Stats/StatIfIs.cpp \
	src/Stats/StatIfThen.cpp \
	src/Stats/StatInput.cpp \
	src/Stats/StatLoop.cpp \
	src/Stats/StatObject.cpp \
	src/Stats/StatOutput.cpp \
	src/Stats/StatProgram.cpp \
	src/Stats/StatProperty.cpp \
	src/Stats/StatRead.cpp \
	src/Stats/StatRecord.cpp \
	src/Stats/StatReturn.cpp \
	src/Stats/StatScope.cpp \
	src/Stats/StatWithExpr.cpp \
	src/Stats/StatWrite.cpp \
	src/Type.cpp \
	src/Method.cpp \
	src/WithFlags.cpp

parser/lex.yy.cpp parser/lex.yy.h: parser/NateLexer.l parser/NateParser.tab.h parser/location.hpp
ifeq ($(HOST_OS),windows)
	"$(REFLEX)" --full --outfile=parser/lex.reflex.cpp --header-file=parser/lex.reflex.h "$<"
	powershell -NoProfile -Command "(Get-Content 'parser/lex.reflex.cpp' -Raw).Replace('parser/lex.reflex.h','lex.yy.h').Replace('parser/lex.reflex.cpp','lex.yy.cpp').Replace('src/lex.reflex.h','lex.yy.h').Replace('src/lex.reflex.cpp','lex.yy.cpp').Replace('lex.reflex.h','lex.yy.h').Replace('lex.reflex.cpp','lex.yy.cpp') | Set-Content 'parser/lex.yy.cpp'"
	powershell -NoProfile -Command "(Get-Content 'parser/lex.reflex.h' -Raw).Replace('parser/lex.reflex.h','lex.yy.h').Replace('parser/lex.reflex.cpp','lex.yy.cpp').Replace('src/lex.reflex.h','lex.yy.h').Replace('src/lex.reflex.cpp','lex.yy.cpp').Replace('lex.reflex.h','lex.yy.h').Replace('lex.reflex.cpp','lex.yy.cpp') | Set-Content 'parser/lex.yy.h'; Remove-Item 'parser/lex.reflex.cpp','parser/lex.reflex.h' -Force"
else
	"$(REFLEX)" --full --outfile=parser/lex.yy.cpp --header-file=parser/lex.yy.h "$<"
endif

parser/parser.cpp parser/NateParser.tab.h parser/location.hpp: parser/NateParser.y
	"$(BISON)" -Wno-conflicts-sr --output="parser/parser.cpp" --defines="parser/NateParser.tab.h" "$<"

include ../build/common.mk

NATE_RUNTIME_OBJECTS := $(foreach src,$(RUNTIME_SOURCES),$(call object_path,$(src)))
RUNTIME_IMPORT_OBJECTS := $(foreach src,$(RUNTIME_IMPORT_SOURCES),$(call object_path,$(src)))
RUNTIME_LIBRARY_OBJECTS := $(NATE_RUNTIME_OBJECTS) $(RUNTIME_IMPORT_OBJECTS)

$(foreach src,$(RUNTIME_IMPORT_SOURCES),$(eval $(call compile_rule,$(src))))

all: $(NATE_RUNTIME_LIB)

$(NATE_RUNTIME_LIB): $(NATE_RUNTIME_PREREQS) | $(OUT_DIR)
ifeq ($(COMPILER),msvc)
	$(AR) $(COMMON_ARFLAGS) /OUT:"$@" $(if $(wildcard $(NATE_COMPILER_EXE)),$(RUNTIME_LIBRARY_OBJECTS),$(NATE_RUNTIME_OBJECTS))
else
	$(AR) $(COMMON_ARFLAGS) "$@" $(if $(wildcard $(NATE_COMPILER_EXE)),$(RUNTIME_LIBRARY_OBJECTS),$(NATE_RUNTIME_OBJECTS))
endif

$(RUNTIME_IMPORT_STAMP): $(NATE_COMPILER_EXE) $(RUNTIME_IMPORT_INPUTS)
ifeq ($(HOST_OS),windows)
	pushd .. && "$(subst /,\,$(PLATFORM)/$(CONFIG)/Nate$(EXE_EXT))" --generate-runtime-imports && popd
	@echo generated>"$(subst /,\,$(RUNTIME_IMPORT_STAMP))"
else
	cd .. && "$(PLATFORM)/$(CONFIG)/Nate$(EXE_EXT)" --generate-runtime-imports
	@echo generated > "$(RUNTIME_IMPORT_STAMP)"
endif

$(RUNTIME_IMPORT_OBJECTS): $(RUNTIME_IMPORT_STAMP)

runtime-imports-update: $(NATE_RUNTIME_LIB)

runtime-clean:
ifeq ($(HOST_OS),windows)
	if exist "$(subst /,\,$(NATE_RUNTIME_LIB))" del /Q "$(subst /,\,$(NATE_RUNTIME_LIB))"
	if exist "$(subst /,\,$(RUNTIME_IMPORT_STAMP))" del /Q "$(subst /,\,$(RUNTIME_IMPORT_STAMP))"
else
	rm -f "$(NATE_RUNTIME_LIB)"
	rm -f "$(RUNTIME_IMPORT_STAMP)"
endif

clean: runtime-clean
