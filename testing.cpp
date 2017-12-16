#include "testing.hpp"


std::ostream& operator<<(std::ostream& out, 
	const TestData& ops)
{
	out << "{" << std::endl;
	for (auto op : ops)
	{
		out << "\t";
		out << "(" << op.type << ", " << op.param1 << ", " << op.param2 << ")";
		out << std::endl;
	}
	out << "}";
	return out;
}

void GenerateTest(TestData& testData, std::size_t size, int VALUE_RANGE, 
	std::default_random_engine& engine)
{
	std::deque<short> oracle;

	testData.clear();
	testData.reserve(size);

	std::discrete_distribution<int> type({10, 10, 6, 6, 10});
	std::uniform_int_distribution<int> value(-VALUE_RANGE, VALUE_RANGE);

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
