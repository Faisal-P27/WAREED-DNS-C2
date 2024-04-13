MAKEFLAGS += -s

COMPILER_x64 = x86_64-w64-mingw32-gcc

# Common flags
CFLAGS =  -fcommon -lws2_32 -lkernel32 -lws2_32 -lmswsock -lsecur32 -ldnsapi -liphlpapi -lshlwapi -pthread -w
CFLAGS += -Wall -Wextra -masm=intel
CFLAGS += -Os -fno-asynchronous-unwind-tables
CFLAGS += -fno-ident -fpack-struct=8 -falign-functions=1
CFLAGS += -s -ffunction-sections -falign-jumps=1 -w -Wl,-s,--no-seh,--enable-stdcall-fixup,--enable-stdcall-fixup

# Source files
SOURCE = Source/*.c Source/Core/*.c Source/Core/Tasks/*.c Source/Core/Tasks/Modules/*.c Source/Cryptography/*.c Source/Packer/*.c Source/Parser/*.c Source/Support/*.c Source/Transport/*.c

# Include directory
INCLUDE_DIR = -I Include

# Build type: exe or dll (set this variable when calling make, e.g., `make BUILD_TYPE=dll`)
BUILD_TYPE ?= exe

ifeq ($(BUILD_TYPE),dll)
# DLL-specific flags
CFLAGS += -DDLL -shared
TARGET = bin/Hams.dll
else ifeq ($(BUILD_TYPE),check)
# check flags
CFLAGS += -D_WIN64 -mwindows
TARGET = bin/Hams.exe
else
# EXE-specific flags
CFLAGS += -D_WIN64 -mwindows
TARGET = bin/Hams.exe
endif

all:
	$(COMPILER_x64) $(SOURCE) $(CFLAGS) $(INCLUDE_DIR) -o $(TARGET)