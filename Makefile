CXX = g++

CXXFLAGS = -std=c++11 -g

OPTFLAGS = -O3 -pthread

TARGET = Lab1

OBJS = main.o fm.o

all: $(TARGET) release

debug: CXXFLAGS += -Wall -DDEBUG_FLAG=1
debug: $(TARGET)
	./$(TARGET) SolutionChecker/sample.in output_debug.out

release: CXXFLAGS += -DDEBUG_FLAG=0 $(OPTFLAGS)
release: $(TARGET)
	./$(TARGET) SolutionChecker/case4.in output_release.out

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

main.o: main.cpp src/fm.h src/Data.h src/BucketList.h
	$(CXX) $(CXXFLAGS) -c main.cpp

fm.o: src/fm.cpp src/fm.h src/Data.h src/BucketList.h src/Debug.h
	$(CXX) $(CXXFLAGS) -c src/fm.cpp -o fm.o

clean:
	rm -f $(OBJS) $(TARGET) output_debug.out output_release.out
