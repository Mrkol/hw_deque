#ifndef TESTING_HPP
#define TESTING_HPP

#include <vector>
#include <deque>
#include <random>
#include <functional>
#include "deque.hpp"


enum OpType
{
	PushBack,
	PushFront,
	PopBack,
	PopFront,
	IndexSet
};

struct Operation
{
	OpType type;
	unsigned long long param1;
	long long param2;
};

typedef std::vector<Operation> TestData;

std::ostream& operator<<(std::ostream&, const TestData&);

void GenerateTest(TestData&, std::size_t, int, std::default_random_engine&);

#endif
