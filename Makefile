# Compiler to use
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -std=c++11 -g

# Name of the target executable
TARGET = Lab1

OBJS = main.o fm.o

all: $(TARGET) test

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

main.o: main.cpp src/fm.h src/Data.h src/BucketList.h
	$(CXX) $(CXXFLAGS) -c main.cpp

fm.o: src/fm.cpp src/fm.h src/Data.h src/BucketList.h
	$(CXX) $(CXXFLAGS) -c src/fm.cpp -o fm.o

test: $(TARGET)
	./$(TARGET) SolutionChecker/sample.in output.out

clean:
	rm -f $(OBJS) $(TARGET)
