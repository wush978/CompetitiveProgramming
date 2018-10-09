CPP=$(wildcard *.cpp)
TARGET=$(basename $(CPP))

all : $(TARGET)

% : %.cpp
	g++ -g -O2 -std=c++14 $< -o $@
