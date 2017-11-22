GTEST_DIR = /usr/src/gtest
USER_DIR = .
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

CXX = clang++

CXXFLAGS += -Wall -Wextra -std=c++14 -pthread
CPPFLAGS += -isystem $(GTEST_DIR)/include

DEBUG_FLAGS = -O0 -g -fno-omit-frame-pointer -fno-optimize-sibling-calls

TESTS = unittests1

BIN_DIR = ./bin

GTEST_HEADERS = /usr/include/gtest/*.h \
                /usr/include/gtest/internal/*.h

gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) $(DEBUG_FLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) $(DEBUG_FLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^


#################

all_tests: $(TESTS)

unittests1.o: $(USER_DIR)/unittests1.cpp $(USER_DIR)/deque.hpp $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(DEBUG_FLAGS) -c $(USER_DIR)/unittests1.cpp

unittests1: unittests1.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(DEBUG_FLAGS) -lpthread $^ -o $(BIN_DIR)/$@

result:
	$(CXX) -E deque.hpp -o deque.h

clean:
	rm -f $(BIN_DIR)/* gtest.a gtest_main.a *.o