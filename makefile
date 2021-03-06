GTEST_DIR = /usr/src/gtest
USER_DIR = .
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

CXX = clang++

CXXFLAGS += -Wall -Wextra -std=c++14
GTESTFLAGS += -isystem $(GTEST_DIR)/include -pthread

DEBUG_FLAGS = -O0 -g -fno-omit-frame-pointer -fno-optimize-sibling-calls
RELEASE_FLAGS = -O3

BIN_DIR = ./bin
OBJ_DIR = ./obj
PROF_DIR = ./profiling

GTEST_HEADERS = /usr/include/gtest/*.h \
                /usr/include/gtest/internal/*.h

all: all_tests

gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) $(DEBUG_FLAGS) -I$(GTEST_DIR) $(CXXFLAGS) \
		-c $(GTEST_DIR)/src/gtest-all.cc -o $(OBJ_DIR)/$@

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) $(DEBUG_FLAGS) -I$(GTEST_DIR) $(CXXFLAGS) \
		-c $(GTEST_DIR)/src/gtest_main.cc -o $(OBJ_DIR)/$@

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $(OBJ_DIR)/$@ $(^:%=$(OBJ_DIR)/%)

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $(OBJ_DIR)/$@ $(^:%=$(OBJ_DIR)/%)

#################

all_tests: dirs unittest speedtest
	./bin/unittest
	./bin/speedtest > speed_testing_results.txt

testing.o: $(USER_DIR)/testing.cpp
	$(CXX) $(CXXFLAGS) $(RELEASE_FLAGS) \
		-c $< -o $(OBJ_DIR)/$@

unittest.o: $(USER_DIR)/unittest.cpp $(GTEST_HEADERS)
	$(CXX) $(GTESTFLAGS) $(CXXFLAGS) $(DEBUG_FLAGS) \
		-c $< -o $(OBJ_DIR)/$@

unittest: unittest.o testing.o gtest_main.a
	$(CXX) $(GTESTFLAGS) $(CXXFLAGS) $(DEBUG_FLAGS) -lpthread \
		$(^:%=$(OBJ_DIR)/%) -o $(BIN_DIR)/$@

speedtest.o: $(USER_DIR)/speedtest.cpp
	$(CXX) $(CXXFLAGS) $(RELEASE_FLAGS) \
		-c $< -o $(OBJ_DIR)/$@

speedtest: speedtest.o testing.o
	$(CXX) $(CXXFLAGS) $(RELEASE_FLAGS) \
		$(^:%=$(OBJ_DIR)/%) -o $(BIN_DIR)/$@

profile: dirs speedtest
	cd $(PROF_DIR);\
	valgrind --tool=callgrind ./../$(BIN_DIR)/speedtest

yacontest:
	head -n -3 deque.hpp > deque.h
	tail -n +2 deque.tpp >> deque.h
	tail -n 2 deque.hpp >> deque.h

dirs:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)
	mkdir -p $(PROF_DIR)

clean:
	rm -f $(BIN_DIR)/* $(OBJ_DIR)/*
