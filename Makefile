# Compiler to use
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -std=c++11

# Name of the target executable
TARGET = Lab1

all: $(TARGET) test

$(TARGET): main.cpp src/fm.cpp src/fm.h
	$(CXX) $(CXXFLAGS) main.cpp src/fm.cpp -o $(TARGET)

test: $(TARGET)
	./$(TARGET) input.dat output.dat

clean:
	rm -f $(TARGET)

