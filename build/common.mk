ifeq ($(strip $(ROOT_DIR)),)
ROOT_DIR := $(abspath ..)
endif

ifeq ($(strip $(CONFIG)),)
CONFIG := Debug
endif

HOST_OS := windows
SHELL := cmd.exe
WINDOWS_ARCH := $(if $(PROCESSOR_ARCHITEW6432),$(PROCESSOR_ARCHITEW6432),$(PROCESSOR_ARCHITECTURE))
DETECTED_PLATFORM := $(if $(filter AMD64,$(WINDOWS_ARCH)),x64,$(if $(filter ARM64,$(WINDOWS_ARCH)),arm64,x86))
PLATFORM ?= $(DETECTED_PLATFORM)
ROOT_OUT_DIR := $(ROOT_DIR)/$(PLATFORM)/$(CONFIG)
OUT_DIR := $(ROOT_OUT_DIR)
BIN_DIR := $(ROOT_OUT_DIR)/bin
LIB_DIR := $(ROOT_OUT_DIR)/lib
INCLUDE_DIR := $(ROOT_OUT_DIR)/include
GENERATED_INCLUDE_DIR := $(INCLUDE_DIR)/created
OBJ_ROOT := $(ROOT_DIR)/.make/$(PLATFORM)/$(CONFIG)

ifeq ($(origin CXX),default)
CXX := cl
endif

ifeq ($(origin AR),default)
AR := lib
endif

ifeq ($(strip $(CXX_STANDARD)),)
CXX_STANDARD := c++23
endif

COMMON_CXXFLAGS := /nologo /EHsc /W3 /permissive- /FS
COMMON_ARFLAGS := /nologo
COMMON_LDFLAGS := /nologo
ifeq ($(CONFIG),Debug)
COMMON_CXXFLAGS += /MDd /Od /Zi
COMMON_DEFINES := _DEBUG
else
COMMON_CXXFLAGS += /MD /O2
COMMON_DEFINES := NDEBUG
endif

COMMON_DEFINES += OS_WIN WIN32

PROJECT_DIR := $(CURDIR)
PROJECT_OBJ_DIR := $(OBJ_ROOT)/$(PROJECT_NAME)
TARGET_DIR := $(patsubst %/,%,$(dir $(TARGET_PATH)))

OBJ_EXT := .obj
ifeq ($(strip $(CXX_STANDARD)),)
CXX_STANDARD := c++23
endif
INCLUDE_FLAGS := $(foreach dir,$(INCLUDE_DIRS),/I"$(dir)")
DEFINE_FLAGS := $(foreach def,$(COMMON_DEFINES) $(DEFINES),/D$(def))
LIBPATH_FLAGS := $(foreach dir,$(LIB_DIRS),/LIBPATH:"$(dir)")
STANDARD_FLAG := /std:$(CXX_STANDARD)
ifeq ($(CXX_STANDARD),c++23)
STANDARD_FLAG := /std:c++latest
endif
PDB_FLAG := /Fd"$(PROJECT_OBJ_DIR)/$(PROJECT_NAME).pdb"
CXXFLAGS := $(COMMON_CXXFLAGS) $(PDB_FLAG) $(PROJECT_CXXFLAGS) $(STANDARD_FLAG) $(DEFINE_FLAGS) $(INCLUDE_FLAGS)
LDFLAGS := $(COMMON_LDFLAGS) $(PROJECT_LDFLAGS) $(LIBPATH_FLAGS)

object_path = $(PROJECT_OBJ_DIR)/$(notdir $(basename $(1)))$(OBJ_EXT)
OBJECTS := $(foreach src,$(SOURCES),$(call object_path,$(src)))

.PHONY: all clean

all: $(TARGET_PATH)

SHELL := cmd.exe
CMD_TARGET_DIR := $(subst /,\,$(TARGET_DIR))
CMD_PROJECT_OBJ_DIR := $(subst /,\,$(PROJECT_OBJ_DIR))
CMD_TARGET_PATH := $(subst /,\,$(TARGET_PATH))

$(TARGET_DIR):
	if not exist "$(CMD_TARGET_DIR)" mkdir "$(CMD_TARGET_DIR)"

$(PROJECT_OBJ_DIR):
	if not exist "$(CMD_PROJECT_OBJ_DIR)" mkdir "$(CMD_PROJECT_OBJ_DIR)"

define compile_rule
$(call object_path,$(1)): $(1) | $(PROJECT_OBJ_DIR)
	$(CXX) /c /TP $(CXXFLAGS) /Fo"$$@" "$$<"
endef

$(foreach src,$(SOURCES),$(eval $(call compile_rule,$(src))))

ifeq ($(TARGET_KIND),lib)
$(TARGET_PATH): $(OBJECTS) | $(TARGET_DIR)
	$(AR) $(COMMON_ARFLAGS) /OUT:"$@" $(OBJECTS)
else
$(TARGET_PATH): $(OBJECTS) $(EXTRA_DEPS) | $(TARGET_DIR)
	$(CXX) $(OBJECTS) $(LDLIBS) /Fe"$@" /link $(LDFLAGS)
endif

clean:
	if exist "$(CMD_TARGET_PATH)" del /Q "$(CMD_TARGET_PATH)"
	if exist "$(CMD_PROJECT_OBJ_DIR)" rmdir /S /Q "$(CMD_PROJECT_OBJ_DIR)"
