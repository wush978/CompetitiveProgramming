CPP=$(wildcard *.cpp)
TARGET=$(basename $(CPP))

all : $(TARGET)

% : %.cpp
	g++ -g -std=c++14 $< -o $@
