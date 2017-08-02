#ifndef __UTILS_H__
#define __UTILS_H__

#include "cocos2d.h"

//git test

namespace crossareagame
{
	enum class PlaygroundSide
	{
		Bottom,
		Left,
		Top,
		Right,
		COUNT,  //do not use it enum-value (only for random or loop)
		NONE    //use it to show playgroundSide is not defined
	};

	enum class ScriptEvent
	{
		PREV_START = 0,
		POST_FINISH
	};

	struct CircleStruct : public cocos2d::Ref
	{
		PlaygroundSide startSide, finishSide;
		cocos2d::Vec2 startPos, finishPos;
		cocos2d::Color4B color;
		float scaleFactor;
		float crosstime;
	};

	template<typename T>
	class TimesCounter
	{
	public:
		TimesCounter() : maximum(0), current(0) {}
		TimesCounter(T max, T curr) : maximum(max), current(curr) {}

		T getMaximum() { return maximum; }
		void setMaximum(T max) { maximum = max; }

		T getCurrent() { return current; }
		void setCurrect(T curr) { current = curr; }

		void reset() { current = 0; }
		void addict(T num) { current += num; }
		bool isFull() { return current >= maximum; }

	private:
		T maximum;
		T current;
	};

}

#endif //__UTILS_H__