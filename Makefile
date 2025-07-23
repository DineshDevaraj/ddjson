
CXX := g++
CXXFLAGS := -g -Wall -Wextra

all: demo

demo: demo.cpp source/*.cpp source/*/*.cpp header/*.h
	$(CXX) $(CXXFLAGS) -Iheader -o demo demo.cpp source/*.cpp source/*/*.cpp

clean:
	rm -f *.exe.* *.exe *.gch
