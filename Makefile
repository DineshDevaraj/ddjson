
CXX := g++
CXXFLAGS := -g -Wall -Wextra

ifeq ($(OS),Windows_NT)
output_name := demo.exe
else
output_name := demo.bin
endif

all: demo

demo: demo.cpp source/*.cpp source/*/*.cpp header/*.h
	$(CXX) $(CXXFLAGS) -Iheader -o $(output_name) demo.cpp source/*.cpp source/*/*.cpp

clean:
	rm -f *.exe.* *.exe *.gch $(output_name)
