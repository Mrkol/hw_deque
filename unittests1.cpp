#include "gtest/gtest.h"
#include "deque.hpp"
#include <deque>
#include <random>


namespace 
{
	TEST(MainTestCase, ManualDequeTest)
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

	TEST(MainTestCase, ManualIteratorTest)
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
		for (size_t i = 0; i < deq.size(); ++i, ++it)
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
		EXPECT_EQ(*(it - 3), deq[2]);

		EXPECT_EQ(deq.end() - deq.begin(), deq.size());
		EXPECT_EQ(deq.rbegin() - deq.rend(), deq.size());

		const Deque<int> cdeq(deq);
		Deque<int>::const_iterator cit = cdeq.begin();
		for (size_t i = 0; i < deq.size(); ++i, ++cit)
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
		EXPECT_EQ(*(cit - 3), cdeq[2]);	

		EXPECT_EQ(cdeq.cend() - cdeq.cbegin(), cdeq.size());
		EXPECT_EQ(cdeq.crbegin() - cdeq.crend(), cdeq.size());
	}

	TEST(MainTestCase, PairIteratorTest)
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

	template<typename T>
	::testing::AssertionResult MatchOracle(const Deque<T>& deq, const std::deque<T>& oracle)
	{
		bool success = true;
		success &= deq.size() == oracle.size();
		success &= deq.empty() == oracle.empty();
		if (!oracle.empty()) success &= deq.front() == oracle.front();
		if (!oracle.empty()) success &= deq.back() == oracle.back();

		for (size_t i = 0; i < oracle.size(); ++i)
		{
			success &= deq[i] == oracle[i];
		}

		if (success) return ::testing::AssertionSuccess();

		::testing::AssertionResult res = ::testing::AssertionFailure();

		res << std::endl << "Oracle looks like this:" << std::endl;
		for (size_t i = 0; i < oracle.size(); ++i) res << oracle[i] << " ";
		res << "(" << oracle.size() << ")" << std::endl;

		res << "But he deque looks like this:" << std::endl;
		for (size_t i = 0; i < deq.size(); ++i) res << deq[i] << " ";
		res << "(" << deq.size() << ")" << std::endl;

		return res;
	}

	std::default_random_engine engine;
	
	TEST(MainTestCase, RandomizedDequeTest)
	{
		const size_t COUNT = 1000;
		const int RANGE = 10000;

		Deque<short> deq;
		std::deque<short> oracle;

		std::uniform_int_distribution<int> type(0, 4);
		std::uniform_int_distribution<int> value(-RANGE, RANGE);

		for (size_t i = 0; i < COUNT; ++i) 
		{
			switch (type(engine))
			{
				case 0:
					oracle.push_back(value(engine));
					deq.push_back(oracle.back());
					break;

				case 1:
					oracle.push_front(value(engine));
					deq.push_front(oracle.front());
					break;

				case 2:
					if (oracle.empty()) continue;
					oracle.pop_back();
					deq.pop_back();
					break;

				case 3:
					if (oracle.empty()) continue;
					oracle.pop_front();
					deq.pop_front();
					break;

				case 4:
					if (oracle.empty()) continue;
					std::uniform_int_distribution<int> index(0, deq.size() - 1);
					size_t ind = index(engine);
					oracle[ind] = value(engine);
					deq[ind] = oracle[ind];
					break;
			}
			EXPECT_TRUE(MatchOracle(deq, oracle));
		}
	}
}


int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}