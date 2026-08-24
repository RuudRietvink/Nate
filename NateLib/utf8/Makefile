TARGET_PATH := $(OUT_DIR)/utf8.stamp

.PHONY: all clean

all: $(TARGET_PATH)

ifeq ($(HOST_OS),windows)
SHELL := cmd.exe
CMD_OUT_DIR := $(subst /,\,$(OUT_DIR))
CMD_TARGET_PATH := $(subst /,\,$(TARGET_PATH))

$(TARGET_PATH): checked.h core.h unchecked.h utf8.h
	if not exist "$(CMD_OUT_DIR)" mkdir "$(CMD_OUT_DIR)"
	type nul > "$(CMD_TARGET_PATH)"

clean:
	if exist "$(CMD_TARGET_PATH)" del /Q "$(CMD_TARGET_PATH)"
else
$(TARGET_PATH): checked.h core.h unchecked.h utf8.h
	mkdir -p "$(OUT_DIR)"
	touch "$@"

clean:
	rm -f "$(TARGET_PATH)"
endif
