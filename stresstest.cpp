#include <iostream>
#include <cstdlib>
#include <ctime>
#include "deque.hpp"


void Stress(long long operations)
{
	Deque<short> deq;

	for (long long i = 0; i < operations; ++i) 
	{
		switch (std::rand() % 5)
		{
			case 0:
				deq.push_back(i);
				break;

			case 1:
				deq.push_front(i);
				break;

			case 2:
				if (deq.empty()) continue;
				deq.pop_back();
				break;

			case 3:
				if (deq.empty()) continue;
				deq.pop_front();
				break;

			case 4:
				if (deq.empty()) continue;
				deq[std::rand() % deq.size()] = i;
				break;
		}
	}
}



int main()
{
	std::srand(std::time(0));
	Stress(int(1e5));
	return 0;
}