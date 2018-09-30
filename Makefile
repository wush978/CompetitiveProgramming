CPP=$(wildcard *.cpp)
TARGET=$(basename $(CPP))

all : $(TARGET)
	echo $(TARGET)

% : %.cpp
	g++ -g -std=c++14 $< -o $@
