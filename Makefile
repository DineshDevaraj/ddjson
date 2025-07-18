
CXX := g++
CXXFLAGS := -Wall -Wextra -O2

all: demo

demo: demo.cpp ddjson.cpp ddjson.h
	$(CXX) $(CXXFLAGS) -o demo demo.cpp ddjson.cpp

clean:
	rm -f demo
