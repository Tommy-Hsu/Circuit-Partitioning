# Compiler to use
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -std=c++11

# Name of the target executable
TARGET = Lab1

all: $(TARGET) test

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $(TARGET)

test: $(TARGET)
	./$(TARGET) input.txt output.txt

clean:
	rm -f $(TARGET)
