#ifndef __CIRCLE_GENERATOR_H__
#define __CIRCLE_GENERATOR_H__

#include "cocos2d.h"
#include "Utils.h"
#include "SmartRandom.h"

namespace crossareagame
{
	class CircleGeneratorConfiguration
	{
	public:
		std::pair<float, float> scalePercent; //min - max
		std::pair<float, float> crossTime; //min - max
		std::vector<cocos2d::Color4B> colors;

	};

	class CircleGenerator
	{
	public:
		CircleGenerator(const cocos2d::Size & playgroundSize, float normalCircleDiameter);
		CircleGenerator(const cocos2d::Size & playgroundSize, float normalCircleDiameter, const CircleGeneratorConfiguration & configuration);
		virtual ~CircleGenerator() = default;

	public:
		void setConfiguration(CircleGeneratorConfiguration & configuration);
		void setScalePercent(float minScalePercent, float maxScalePercent);
		void setCrossTime(float minCrossTime, float maxCrossTime);

	public:
		virtual CircleStruct * getNextCircle();
		CircleStruct * getNextCircleWithScale(float scale);
		CircleStruct * getNextCircleWithCrossTime(float crossTime);
		CircleStruct * getNextCircleWithSide(const PlaygroundSide & side);
		CircleStruct * getNextCircleWithColor(const cocos2d::Color4B & color);

		/*
			if configuration.scalePercent.first == -1.0f - using fields minScalePercent & maxScalePercent to generate scale
			if configuration.crossTime.first == -1.0f - using fields minCrossTime & maxCrossTime
			if configuration.colors.isEmpty == true - using field colors
			if side == PlaygroundSide.NONE - using random side
			if halfOfSide == -1.0f - using random halfOfSide
		*/
		CircleStruct * getNextCircleWithConfiguration(const CircleGeneratorConfiguration & configuration, const PlaygroundSide & side, int halfOfSide);

	public:
		const cocos2d::Size & getPlaygroungSize() { return playgroundSize; }

		void getCrossTime(float & min, float & max) { min = minCrossTime; max = maxCrossTime; }

	protected:

		cocos2d::Size playgroundSize;
		float normalCircleDiameter;
		float minScalePercent, maxScalePercent;
		float minCrossTime, maxCrossTime;
		std::vector<cocos2d::Color4B> colors;

		SmartRandom<PlaygroundSide> sidesRandomize;
		SmartRandom<cocos2d::Color4B> colorsRandomize;


		void putColor(CircleStruct * circleStruct, const cocos2d::Color4B & color);
		void putScale(CircleStruct * circleStruct, float minScale, float maxScale);
		void putCrossTime(CircleStruct * circleStruct, float minCrossTime, float maxCrossTime);
		void putStartFinishPos(CircleStruct * circleStruct, const PlaygroundSide & side = PlaygroundSide::NONE);

	private:

		void calculatePoints(CircleStruct * circleStruct, float & d1, float & d2, float & s1, float & s2, float size1, float size2);

		CircleStruct * getCircleStruct(float minScale, float maxScale, float minCrossTime, float maxCrossTime, const std::vector<cocos2d::Color4B> & colors, const PlaygroundSide & side);
	};

}

#endif //__CIRCLE_GENERATOR_H__