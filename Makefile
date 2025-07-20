
CXX := g++
CXXFLAGS := -g -Wall -Wextra

all: demo

demo: *.cpp *.h
	$(CXX) $(CXXFLAGS) -o demo *.cpp

clean:
	rm -f demo
