
CXX := g++
CXXFLAGS := -g -Wall -Wextra

all: demo

demo: demo.cpp ddjson.cpp ddjson.h
	$(CXX) $(CXXFLAGS) -o demo demo.cpp ddjson.cpp

clean:
	rm -f demo
