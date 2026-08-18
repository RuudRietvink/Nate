.DEFAULT_GOAL := all

PROJECT_NAME := Nate
TARGET_KIND := lib
TARGET_PATH := $(NATE_LIB)

DEFINES :=
INCLUDE_DIRS := $(REFLEX_INCLUDE) Stats . ../MathParser ../utf8
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
	Code.cpp \
	Arg.cpp \
	core/Core.cpp \
	core/Rational.cpp \
	Define.cpp \
	Expr.cpp \
	Container.cpp \
	Identifier.cpp \
	lex.yy.cpp \
	Location.cpp \
	NateCode.cpp \
	NateFunctions.cpp \
	NateParser.cpp \
	NateParserMath.cpp \
	Object.cpp \
	Property.cpp \
	Record.cpp \
	Scope.cpp \
	parser.cpp \
	Stats/Stat.cpp \
	Stats/StatAssign.cpp \
	Stats/StatCode.cpp \
	Stats/StatData.cpp \
	Stats/StatDeclareLocal.cpp \
	Stats/StatDefine.cpp \
	Stats/StatError.cpp \
	Stats/StatExpr.cpp \
	Stats/StatIfIs.cpp \
	Stats/StatIfThen.cpp \
	Stats/StatInput.cpp \
	Stats/StatLoop.cpp \
	Stats/StatObject.cpp \
	Stats/StatOutput.cpp \
	Stats/StatProgram.cpp \
	Stats/StatProperty.cpp \
	Stats/StatRead.cpp \
	Stats/StatRecord.cpp \
	Stats/StatReturn.cpp \
	Stats/StatScope.cpp \
	Stats/StatWithExpr.cpp \
	Stats/StatWrite.cpp \
	Type.cpp \
	Method.cpp \
	WithFlags.cpp

lex.yy.cpp lex.yy.h: NateLexer.l NateParser.tab.h location.hpp NateParser.h NateFunctions.h core/Core.h core/Complex.h
	"$(REFLEX)" --header-file "$<"

parser.cpp NateParser.tab.h location.hpp: NateParser.y
	"$(BISON)" --output="parser.cpp" --defines="NateParser.tab.h" "$<"

include ../build/common.mk
