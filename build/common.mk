PROJECT_DIR := $(CURDIR)
PROJECT_OBJ_DIR := $(OBJ_ROOT)/$(PROJECT_NAME)
TARGET_DIR := $(patsubst %/,%,$(dir $(TARGET_PATH)))

ifeq ($(COMPILER),msvc)
OBJ_EXT := .obj
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
else
OBJ_EXT := .o
INCLUDE_FLAGS := $(foreach dir,$(INCLUDE_DIRS),-I"$(dir)")
DEFINE_FLAGS := $(foreach def,$(COMMON_DEFINES) $(DEFINES),-D$(def))
LIBPATH_FLAGS := $(foreach dir,$(LIB_DIRS),-L"$(dir)")
STANDARD_FLAG := -std=$(CXX_STANDARD)
CXXFLAGS := $(COMMON_CXXFLAGS) $(PROJECT_CXXFLAGS) $(STANDARD_FLAG) $(DEFINE_FLAGS) $(INCLUDE_FLAGS)
LDFLAGS := $(COMMON_LDFLAGS) $(PROJECT_LDFLAGS) $(LIBPATH_FLAGS)
endif

object_path = $(PROJECT_OBJ_DIR)/$(notdir $(basename $(1)))$(OBJ_EXT)
OBJECTS := $(foreach src,$(SOURCES),$(call object_path,$(src)))

.PHONY: all clean

all: $(TARGET_PATH)

ifeq ($(HOST_OS),windows)
SHELL := cmd.exe
CMD_TARGET_DIR := $(subst /,\,$(TARGET_DIR))
CMD_PROJECT_OBJ_DIR := $(subst /,\,$(PROJECT_OBJ_DIR))
CMD_TARGET_PATH := $(subst /,\,$(TARGET_PATH))

$(TARGET_DIR):
	if not exist "$(CMD_TARGET_DIR)" mkdir "$(CMD_TARGET_DIR)"

$(PROJECT_OBJ_DIR):
	if not exist "$(CMD_PROJECT_OBJ_DIR)" mkdir "$(CMD_PROJECT_OBJ_DIR)"
else
$(TARGET_DIR):
	mkdir -p "$@"

$(PROJECT_OBJ_DIR):
	mkdir -p "$@"
endif

ifeq ($(COMPILER),msvc)
define compile_rule
$(call object_path,$(1)): $(1) | $(PROJECT_OBJ_DIR)
	$(CXX) /c /TP $(CXXFLAGS) /Fo"$$@" "$$<"
endef
else
define compile_rule
$(call object_path,$(1)): $(1) | $(PROJECT_OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c "$$<" -o "$$@"
endef
endif

$(foreach src,$(SOURCES),$(eval $(call compile_rule,$(src))))

ifeq ($(TARGET_KIND),lib)
$(TARGET_PATH): $(OBJECTS) | $(TARGET_DIR)
ifeq ($(COMPILER),msvc)
	$(AR) $(COMMON_ARFLAGS) /OUT:"$@" $(OBJECTS)
else
	$(AR) $(COMMON_ARFLAGS) "$@" $(OBJECTS)
endif
else
$(TARGET_PATH): $(OBJECTS) $(EXTRA_DEPS) | $(TARGET_DIR)
ifeq ($(COMPILER),msvc)
	$(CXX) $(OBJECTS) $(LDLIBS) /Fe"$@" /link $(LDFLAGS)
else
	$(CXX) $(LDFLAGS) -o "$@" $(OBJECTS) $(LDLIBS)
endif
endif

clean:
ifeq ($(HOST_OS),windows)
	if exist "$(CMD_TARGET_PATH)" del /Q "$(CMD_TARGET_PATH)"
	if exist "$(CMD_PROJECT_OBJ_DIR)" rmdir /S /Q "$(CMD_PROJECT_OBJ_DIR)"
else
	rm -f "$(TARGET_PATH)"
	rm -rf "$(PROJECT_OBJ_DIR)"
endif
