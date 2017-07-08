#ifndef  __SMARTRANDOM_H__
#define  __SMARTRANDOM_H__

#include "cocos2d.h"

namespace crossareagame
{
	static std::random_device seed_gen;
	static std::mt19937 engine(seed_gen());

	template <typename T>
	class SmartRandom
	{
	public:
		SmartRandom() {}
		SmartRandom(std::size_t times, const std::vector<T> & vals) : timesToZero(times)
		{
			values.reserve(vals.size());
			for (auto & val : vals)
				values.push_back(std::pair<T, std::size_t>(val, 0));
			distribute();
		}

		void initialize(std::size_t times, const std::vector<T> & vals)
		{
			timesToZero = times;
			values.clear();
			values.reserve(vals.size());
			for (auto & val : vals)
				values.push_back(std::pair<T, std::size_t>(val, 0));
			distribute();
		}

	public:
		T & getRandomValue()
		{
			std::size_t p = getRand(1, getMax());
			for (std::size_t index = 0, bound = 0, inc = 0; index < values.size(); bound += inc, ++index)
			{
				inc = values[index].second;
				if (p > bound && p <= bound + inc)
				{
					recalculate(index);
					return values[index].first;
				}
			}
		}

		void testRandom(const T & val)
		{
			for (std::size_t index = 0; index < values.size(); ++index)
			{
				if (val == values[index].first)
				{
					recalculate(index);
					break;
				}
			}
		}

	private:
		std::vector<std::pair<T, std::size_t>> values;
		std::size_t timesToZero; //how many times propability should be > 0!
		std::size_t decrement;

	private:
		void distribute() //all values take the same propability percent
		{
			decrement = values.size() - 1;
			std::size_t percent = decrement * timesToZero;
			for (auto & p : values)
				p.second = percent;
		}

		void recalculate(int index) //change distribution of all values
		{
			values[index].second -= decrement;
			std::size_t percent = decrement * timesToZero;
			bool distibuteAgainFlag = true; //true if every variant was generated minimum one time
			for (std::size_t ind = 0; ind < values.size(); ++ind)
			if (ind != index && values[ind].second == percent)
			{
				distibuteAgainFlag = false;
				break;
			}
			if (distibuteAgainFlag)
			for (std::size_t ind = 0; ind < values.size(); ++ind)
			if (ind != index && values[ind].second < percent)
				values[ind].second += decrement;
		}

		std::size_t getRand(std::size_t min, std::size_t max)
		{
			std::uniform_int_distribution<std::size_t> dist(min, max);
			return dist(engine);
		}

		std::size_t getMax()
		{
			std::size_t max = 0;
			for (auto & val : values)
				max += val.second;
			return max;
		}
	};

}

#endif //__SMARTRANDOM_H__