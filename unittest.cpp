#include <deque>
#include <algorithm>
#include <random>
#include "gtest/gtest.h"
#include "deque.hpp"


namespace 
{
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

	class MainTestCase : public ::testing::Test
	{
	protected:
		std::default_random_engine engine;
		std::vector<Operation> _testData;
		const std::size_t COUNT = 1000;
		const int RANGE = 10000;

		virtual void SetUp()
		{
			std::deque<short> oracle;

			_testData.clear();
			_testData.reserve(COUNT);

			std::discrete_distribution<int> type({10, 10, 8, 8, 10});
			std::uniform_int_distribution<int> value(-RANGE, RANGE);

			while (_testData.size() < COUNT)
			{
				OpType op = (OpType) type(engine);
				if (op == PushBack)
				{
					int val = value(engine);
					oracle.push_back(val);
					_testData.push_back({op, 0, val});
					continue;					
				}

				if (op == PushFront)
				{
					int val = value(engine);
					oracle.push_front(val);
					_testData.push_back({op, 0, val});
					continue;
				}
				
				if (oracle.empty()) continue;
				
				if (op == PopBack)
				{
					oracle.pop_back();
					_testData.push_back({op, 0, 0});
					continue;
				}
				
				if (op == PopFront)
				{
					oracle.pop_front();
					_testData.push_back({op, 0, 0});
					continue;
				}

				if (op == IndexSet)
				{
					std::uniform_int_distribution<int> index(0, oracle.size() - 1);
					std::size_t ind = index(engine);
					int val = value(engine);
					oracle[ind] = val;
					_testData.push_back({op, ind, val});
				}
			}
		}
	};

	std::ostream& operator<<(std::ostream& out, 
		const std::vector<Operation>& ops)
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


	template<class TDeque1, class TDeque2>
	::testing::AssertionResult Matches(const TDeque1& deq, const TDeque2& oracle)
	{
		bool success = true;
		success &= deq.size() == oracle.size();
		success &= deq.empty() == oracle.empty();
		if (!oracle.empty()) success &= deq.front() == oracle.front();
		if (!oracle.empty()) success &= deq.back() == oracle.back();

		for (std::size_t i = 0; i < oracle.size(); ++i)
		{
			success &= deq[i] == oracle[i];
		}

		if (success) return ::testing::AssertionSuccess();

		::testing::AssertionResult res = ::testing::AssertionFailure();

		res << std::endl << "Oracle looks like this:" << std::endl;
		for (std::size_t i = 0; i < oracle.size(); ++i) res << oracle[i] << " ";
		res << "(" << oracle.size() << ")" << std::endl;

		res << "But he deque looks like this:" << std::endl;
		for (std::size_t i = 0; i < deq.size(); ++i) res << deq[i] << " ";
		res << "(" << deq.size() << ")" << std::endl;

		return res;
	}

	TEST_F(MainTestCase, ManualDequeTest)
	{
		Deque<int> deq;
		deq.push_back(1);
		deq.pop_back();
		EXPECT_TRUE(deq.empty());
		deq.push_back(1);
		deq.push_back(2);
		deq.push_back(3);
		deq.push_back(4);
		EXPECT_EQ(deq.front(), 1);
		EXPECT_EQ(deq.back(), 4);
		EXPECT_EQ(deq.size(), 4);
		deq.pop_front();
		EXPECT_EQ(deq.front(), 2);
		deq.push_front(1);
		deq.push_front(0);
		deq.push_front(-1);
		deq.push_front(-2);
		deq.push_front(-3);
		deq.push_front(-4);
		deq.push_front(-5);
		deq.push_front(-6);
		deq.push_front(-7);
		deq.push_front(-8);
		EXPECT_EQ(deq.front(), -8);
		EXPECT_EQ(deq.back(), 4);
		EXPECT_EQ(deq[3], -5);
	}

	TEST_F(MainTestCase, ManualIteratorTest)
	{
		Deque<int> deq;
		deq.push_back(-5);
		deq.push_back(-4);
		deq.push_back(-3);
		deq.push_back(-2);
		deq.push_back(-1);
		deq.push_back(0);
		deq.push_back(1);
		deq.push_back(2);
		deq.push_back(3);
		deq.push_back(4);
		deq.push_back(5);
		Deque<int>::iterator it = deq.begin();
		for (std::size_t i = 0; i < deq.size(); ++i, ++it)
		{
			EXPECT_EQ(*it, deq[i]);
		}
		it = deq.begin();
		it += 7;
		EXPECT_EQ(*it, deq[7]);
		it -= 3;
		EXPECT_EQ(*it, deq[4]);
		it += 1;
		EXPECT_EQ(*it, deq[5]);

		EXPECT_EQ(*(it + -2), deq[3]);
		EXPECT_EQ(*(2 + it), deq[7]);
		EXPECT_EQ(*(it - 3), deq[2]);

		EXPECT_EQ(deq.end() - deq.begin(), deq.size());
		EXPECT_EQ(deq.rend() - deq.rbegin(), deq.size());

		int ind = 0;
		for (auto& el : deq) EXPECT_EQ(el, deq[ind++]);

		Deque<int>::reverse_iterator rit = deq.rbegin();
		for (std::size_t i = deq.size() - 1; rit != deq.rend(); --i, ++rit)
		{
			EXPECT_EQ(*rit, deq[i]);
		}

		const Deque<int> cdeq(deq);
		Deque<int>::const_iterator cit = cdeq.begin();
		for (std::size_t i = 0; i < deq.size(); ++i, ++cit)
		{
			EXPECT_EQ(*cit, cdeq[i]);
		}
		cit = cdeq.begin();
		cit += 7;
		EXPECT_EQ(*cit, cdeq[7]);
		cit -= 3;
		EXPECT_EQ(*cit, cdeq[4]);
		cit += 1;
		EXPECT_EQ(*cit, cdeq[5]);

		EXPECT_EQ(*(cit + -2), cdeq[3]);
		EXPECT_EQ(*(2 + cit), cdeq[7]);
		EXPECT_EQ(*(cit - 3), cdeq[2]);	

		EXPECT_EQ(cdeq.cend() - cdeq.cbegin(), cdeq.size());
		EXPECT_EQ(cdeq.crend() - cdeq.crbegin(), cdeq.size());
	}

	TEST_F(MainTestCase, PairIteratorTest)
	{
		Deque<std::pair<int, int>> deq;
		deq.push_back(std::make_pair(0, 1));
		deq.push_back(std::make_pair(2, 3));
		deq.push_back(std::make_pair(4, 5));
		deq.push_back(std::make_pair(6, 7));
		deq.push_back(std::make_pair(8, 9));
		Deque<std::pair<int, int>>::iterator it = deq.begin();
		EXPECT_EQ((it + 1)->second, deq[1].second);
		EXPECT_EQ((it + 4)->second, deq[4].second);
	}

	TEST_F(MainTestCase, StlIteratorTest)
	{
		Deque<int> deq1;
		Deque<int> deq2;

		for (std::size_t i = 0; i < COUNT; ++i)
		{
			deq1.push_back(i);
			deq2.push_back(i);
		}

		std::random_shuffle(deq1.begin(), deq1.end());
		ASSERT_FALSE(Matches(deq1, deq2));
		std::sort(deq1.begin(), deq1.end());

		ASSERT_TRUE(Matches(deq1, deq2));
	}

	TEST_F(MainTestCase, LargeSizeTest)
	{
		Deque<int> deq1;
		std::deque<int> oracle;

		ASSERT_TRUE(Matches(deq1, oracle));
		
		for (std::size_t i = 0; i < COUNT; ++i)
		{
			deq1.push_back(i);
			oracle.push_back(i);
		}
		
		ASSERT_TRUE(Matches(deq1, oracle));

		for (std::size_t i = 0; i < COUNT; ++i)
		{
			deq1.pop_front();
			oracle.pop_front();
		}
		
		ASSERT_TRUE(Matches(deq1, oracle));
	}

	std::default_random_engine engine;
	
	TEST_F(MainTestCase, RandomizedDequeTest)
	{
		Deque<short> deq;
		std::deque<short> oracle;

		for (auto op : _testData) 
		{
			switch (op.type)
			{
				case PushBack:
					oracle.push_back(op.param2);
					deq.push_back(op.param2);
					break;

				case PushFront:
					oracle.push_front(op.param2);
					deq.push_front(op.param2);
					break;

				case PopBack:
					oracle.pop_back();
					deq.pop_back();
					break;

				case PopFront:
					oracle.pop_front();
					deq.pop_front();
					break;

				case IndexSet:
					oracle[op.param1] = op.param2;
					deq[op.param1] = op.param2;
					break;
			}
			ASSERT_TRUE(Matches(deq, oracle)) << _testData;
		}
	}
}


int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
