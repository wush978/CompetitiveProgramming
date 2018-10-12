CPP=$(wildcard *.cpp)
DEBUG=$(CPP:.cpp=.debug)
TARGET=$(basename $(CPP))

all : $(TARGET) $(DEBUG)

% : %.cpp
	g++ -O2 -std=c++14 $< -o $@

%.debug : %.cpp
	g++ -g -std=c++14 $< -o $@

debug :
	lldb ./$(shell ls -1rt $(DEBUG) | tail -n 1)
