.DEFAULT_GOAL := all

PROJECT_NAME := NateLib
TARGET_KIND := lib
TARGET_PATH := $(NATE_LIB)

DEFINES :=
INCLUDE_DIRS := $(REFLEX_INCLUDE) Stats . core/cpp ../MathParser ../utf8
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
	core/cpp/Core.cpp \
	core/cpp/Rational.cpp \
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

lex.yy.cpp lex.yy.h: NateLexer.l NateParser.tab.h location.hpp
ifeq ($(HOST_OS),windows)
	"$(REFLEX)" --full --outfile=lex.reflex.cpp --header-file=lex.reflex.h "$<"
	powershell -NoProfile -Command "(Get-Content 'lex.reflex.cpp' -Raw).Replace('lex.reflex.h','lex.yy.h').Replace('lex.reflex.cpp','lex.yy.cpp') | Set-Content 'lex.yy.cpp'"
	powershell -NoProfile -Command "(Get-Content 'lex.reflex.h' -Raw).Replace('lex.reflex.h','lex.yy.h').Replace('lex.reflex.cpp','lex.yy.cpp') | Set-Content 'lex.yy.h'; Remove-Item 'lex.reflex.cpp','lex.reflex.h' -Force"
else
	"$(REFLEX)" --full --outfile=lex.yy.cpp --header-file=lex.yy.h "$<"
endif

parser.cpp NateParser.tab.h location.hpp: NateParser.y
	"$(BISON)" -Wno-conflicts-sr --output="parser.cpp" --defines="NateParser.tab.h" "$<"

include ../build/common.mk
