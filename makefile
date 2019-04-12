CC = gcc

BUILD_DIR = bin
SRC_DIR = src

TARGET = testSocket.exe

MKDIR_P_BUILD = if not exist "$(BUILD_DIR)" mkdir $(BUILD_DIR)

CINCLUDE = -I.\$(SRC_DIR)\ -I\msys64\mingw64\include
CLIBS = -L\msys64\mingw64\lib -lws2_32 -lmingw32 -lSDL2

SOURCES = $(wildcard $(SRC_DIR)/*.c)

$(TARGET): $(SOURCES)
	$(shell $(MKDIR_P_BUILD))
	$(CC) -o $(BUILD_DIR)/$@ $(CINCLUDE) $^ $(CLIBS)	