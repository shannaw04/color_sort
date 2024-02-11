CXX = g++
CXXFLAGS = -Wall
EXE = bin/play_1 bin/play bin/create_1_level bin/create_level_set 

all: $(EXE)

bin/create_1_level: src/create_1_level.cpp src/Generator.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

bin/create_level_set: src/create_level_set.cpp src/Generator.cpp 
	$(CXX) $(CXXFLAGS) -o $@ $^

bin/play_1: src/play_1.cpp src/ColorSort.cpp 
	$(CXX) $(CXXFLAGS) -o $@ $^

bin/play: src/play.cpp src/ColorSort.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f bin/*
