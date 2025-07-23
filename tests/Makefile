
CXX := g++
CXXFLAGS := -g -Wall -Wextra
LIBS := -lCatch2Main -lCatch2

ifeq ($(OS),Windows_NT)
TEST_BIN := tests.exe
else
TEST_BIN := tests.bin
endif

TEST_SRC := $(wildcard tests/*.cpp)
TEST_SRC += $(wildcard source/*.cpp)
TEST_SRC += $(wildcard source/*/*.cpp)

TEST_INCLUDES := -Iheader

all: $(TEST_SRC)
	$(CXX) $(CXXFLAGS) $(TEST_INCLUDES) -o $(TEST_BIN) $(TEST_SRC) $(LIBS)

clean:
	rm -f $(TEST_BIN)
