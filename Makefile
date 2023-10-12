# Compiler to use
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -std=c++11 -g

# Name of the target executable
TARGET = Lab1

all: $(TARGET) test

$(TARGET): main.cpp src/fm.cpp src/fm.h src/Data.h
	$(CXX) $(CXXFLAGS) main.cpp src/fm.cpp src/Data.h -o $(TARGET)

test: $(TARGET)
	./$(TARGET) SolutionChecker/sample.in output.out
clean:
	rm -f $(TARGET)

