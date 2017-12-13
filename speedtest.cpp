#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <deque>
#include <random>
#include <vector>
#include "deque.hpp"


std::default_random_engine engine;

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


void Generate(std::vector<Operation>& testData, std::size_t size)
{
	const int RANGE = 100000;

	std::deque<short> oracle;

	testData.clear();
	testData.reserve(size);

	std::discrete_distribution<int> type({10, 10, 6, 6, 10});
	std::uniform_int_distribution<int> value(-RANGE, RANGE);

	while (testData.size() < size)
	{
		OpType op = (OpType) type(engine);
		if (op == PushBack)
		{
			int val = value(engine);
			oracle.push_back(val);
			testData.push_back({op, 0, val});
			continue;					
		}

		if (op == PushFront)
		{
			int val = value(engine);
			oracle.push_front(val);
			testData.push_back({op, 0, val});
			continue;
		}
		
		if (oracle.empty()) continue;
		
		if (op == PopBack)
		{
			oracle.pop_back();
			testData.push_back({op, 0, 0});
			continue;
		}
		
		if (op == PopFront)
		{
			oracle.pop_front();
			testData.push_back({op, 0, 0});
			continue;
		}

		if (op == IndexSet)
		{
			std::uniform_int_distribution<int> index(0, oracle.size() - 1);
			size_t ind = index(engine);
			int val = value(engine);
			oracle[ind] = val;
			testData.push_back({op, ind, val});
		}
	}
}

long long SpeedTest(const std::vector<Operation>& testData)
{
	using clock_t = std::chrono::high_resolution_clock;
	using timepoint_t = std::chrono::time_point<clock_t>;
	using ms_t = std::chrono::microseconds;

	timepoint_t start(clock_t::now());

	Deque<int> deq;

	for (const auto& op : testData) 
	{
		switch (op.type)
		{
			case PushBack:
				deq.push_back(op.param2);
				break;

			case PushFront:
				deq.push_front(op.param2);
				break;

			case PopBack:
				deq.pop_back();
				break;

			case PopFront:
				deq.pop_front();
				break;

			case IndexSet:
				deq[op.param1] = op.param2;
				break;		
		}
	}
	deq.front(); //hack so that the compiler does not optimise the whole loop out

	timepoint_t end(clock_t::now());

	long long elapsed = std::chrono::duration_cast<ms_t>(end - start).count();

	return elapsed;
}

int main()
{
	long long sizes[] {1000ll, 10000ll, 100000ll, 1000000ll, 10000000ll};
	for (auto size : sizes)
	{
		const size_t AVERAGE_OVER = 10;
		std::cout << "Testing over " << size << " operations..." << std::endl;

		long long average_time = 0;
		for (size_t i = 0; i < AVERAGE_OVER; ++i)
		{
			std::vector<Operation> testData;
			Generate(testData, size);
			average_time += SpeedTest(testData);
		}
		average_time /= AVERAGE_OVER;

		std::cout << "Finished. Average time: " << average_time << "ms" << std::endl;

		std::cout << std::endl;
	}
	return 0;
}
