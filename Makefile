# Compiler to use
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -std=c++11 -g

# Name of the target executable
TARGET = Lab1

OBJS = main.o fm.o

all: $(TARGET) release

# Debug target
debug: CXXFLAGS += -DDEBUG_FLAG=1
debug: $(TARGET)
	./$(TARGET) SolutionChecker/sample.in output_debug.out

# Release target
release: CXXFLAGS += -DDEBUG_FLAG=0
release: $(TARGET)
	./$(TARGET) SolutionChecker/sample.in output_release.out

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

main.o: main.cpp src/fm.h src/Data.h src/BucketList.h
	$(CXX) $(CXXFLAGS) -c main.cpp

fm.o: src/fm.cpp src/fm.h src/Data.h src/BucketList.h src/Debug.h
	$(CXX) $(CXXFLAGS) -c src/fm.cpp -o fm.o

clean:
	rm -f $(OBJS) $(TARGET) output_debug.out output_release.out
