#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <deque>
#include <random>
#include <vector>
#include "deque.hpp"
#include "testing.hpp"


long long SpeedTest(const TestData& testData)
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
	std::default_random_engine engine;
	long long sizes[] {100ll, 1000ll, 10000ll, 100000ll, 1000000ll};
	for (auto size : sizes)
	{
		const size_t AVERAGE_OVER = 10;
		std::cout << "Testing over " << size << " operations..." << std::endl;

		long long average_time = 0;
		for (size_t i = 0; i < AVERAGE_OVER; ++i)
		{
			TestData testData;
			GenerateTest(testData, size, 10000, engine);
			average_time += SpeedTest(testData);
		}
		average_time /= AVERAGE_OVER;

		std::cout << "Finished. Average time: " << average_time << "ms" << std::endl;

		std::cout << std::endl;
	}
	return 0;
}
