CPP=$(wildcard *.cpp)
TARGET=$(basename $(CPP))

all : $(TARGET)
	echo $(TARGET)

% : %.cpp
	g++ -std=c++14 $< -o $@
