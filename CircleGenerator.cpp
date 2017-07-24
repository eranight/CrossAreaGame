#include "CircleGenerator.h"

using cocos2d::Size;
using cocos2d::Vec2;
using cocos2d::Color4B;
using cocos2d::RandomHelper;
using namespace crossareagame;

CircleGenerator::CircleGenerator(const Size & playgroundSize, float normalCircleDiameter) :
	playgroundSize(playgroundSize),
	normalCircleDiameter(normalCircleDiameter),
	sidesRandomize(1, { PlaygroundSide::Bottom, PlaygroundSide::Left, PlaygroundSide::Top, PlaygroundSide::Right })
{

}

CircleGenerator::CircleGenerator(const Size & playgroundSize, float normalCircleDiameter, const CircleGeneratorConfiguration & configuration) :
	playgroundSize(playgroundSize),
	normalCircleDiameter(normalCircleDiameter),
	minScalePercent(configuration.scalePercent.first),
	maxScalePercent(configuration.scalePercent.second),
	minCrossTime(configuration.crossTime.first),
	maxCrossTime(configuration.crossTime.second),
	colors(configuration.colors)
{

}

void CircleGenerator::setConfiguration(CircleGeneratorConfiguration & configuration)
{
	minScalePercent = configuration.scalePercent.first;
	maxScalePercent = configuration.scalePercent.second;
	minCrossTime = configuration.crossTime.first;
	maxCrossTime = configuration.crossTime.second;
	colors = configuration.colors;
	colorsRandomize.initialize(1, colors);
}

CircleStruct * CircleGenerator::getCircleStruct(float minScale, float maxScale, float minCrossTime, float maxCrossTime, const std::vector<cocos2d::Color4B> & colors, const PlaygroundSide & side)
{
	auto circleStruct = new (std::nothrow) CircleStruct();
	if (circleStruct != nullptr)
	{
		putColor(circleStruct, colorsRandomize.getRandomValue());
		putScale(circleStruct, minScale, maxScale);
		putStartFinishPos(circleStruct, side);
		putCrossTime(circleStruct, minCrossTime, maxCrossTime);
	}

	return circleStruct;
}

void CircleGenerator::setScalePercent(float minScalePercent, float maxScalePercent)
{
	this->minScalePercent = minScalePercent;
	this->maxScalePercent = maxScalePercent;
}

void CircleGenerator::setCrossTime(float minCrossTime, float maxCrossTime)
{
	this->minCrossTime = minCrossTime;
	this->maxCrossTime = maxCrossTime;
}

CircleStruct * CircleGenerator::getNextCircle()
{
	return getCircleStruct(minScalePercent, maxScalePercent, minCrossTime, maxCrossTime, colors, PlaygroundSide::NONE);
}

CircleStruct * CircleGenerator::getNextCircleWithScale(float scale)
{
	return getCircleStruct(scale, scale, minCrossTime, maxCrossTime, colors, PlaygroundSide::NONE);
}

CircleStruct * CircleGenerator::getNextCircleWithCrossTime(float crossTime)
{
	return getCircleStruct(minScalePercent, maxScalePercent, crossTime, crossTime, colors, PlaygroundSide::NONE);
}

CircleStruct * CircleGenerator::getNextCircleWithSide(const PlaygroundSide & side)
{
	return getCircleStruct(minScalePercent, maxScalePercent, minCrossTime, maxCrossTime, colors, side);
}

CircleStruct * CircleGenerator::getNextCircleWithColor(const cocos2d::Color4B & color)
{
	return getCircleStruct(minScalePercent, maxScalePercent, minCrossTime, maxCrossTime, { color }, PlaygroundSide::NONE);
}

CircleStruct * CircleGenerator::getNextCircleWithConfiguration(const CircleGeneratorConfiguration & configuration, const PlaygroundSide & side, int halfOfSide)
{
	auto circleStruct = new (std::nothrow) CircleStruct();
	if (circleStruct != nullptr)
	{
		putColor(circleStruct, configuration.colors.empty() ? colorsRandomize.getRandomValue() : configuration.colors[RandomHelper::random_int<int>(0, colors.size() - 1)]);
		
		if (configuration.scalePercent.first == -1.0f)
			putScale(circleStruct, minScalePercent, maxScalePercent);
		else
			putScale(circleStruct, configuration.scalePercent.first, configuration.scalePercent.second);

		putStartFinishPos(circleStruct, side);

		if (configuration.crossTime.first == -1.0f)
			putCrossTime(circleStruct, minCrossTime, maxCrossTime);
		else
			putCrossTime(circleStruct, configuration.crossTime.first, configuration.crossTime.second);
	}

	return circleStruct;
}

void CircleGenerator::putColor(CircleStruct * circleStruct, const cocos2d::Color4B & color)
{
	circleStruct->color = color;
}

void CircleGenerator::putScale(CircleStruct * circleStruct, float minScale, float maxScale)
{
	circleStruct->scaleFactor = RandomHelper::random_real<float>(minScale, maxScale) / 100.0f;
}

void CircleGenerator::putCrossTime(CircleStruct * circleStruct, float minCrossTime, float maxCrossTime)
{
	circleStruct->crosstime = RandomHelper::random_real<float>(minCrossTime, maxCrossTime);
}

void CircleGenerator::putStartFinishPos(CircleStruct * circleStruct, const PlaygroundSide & side)
{
	int startSideAsInt = side == PlaygroundSide::NONE ? static_cast<int>(sidesRandomize.getRandomValue()) : static_cast<int>(side);

	circleStruct->startSide = static_cast<PlaygroundSide>(startSideAsInt);
	circleStruct->finishSide = static_cast<PlaygroundSide>((startSideAsInt + 2) % 4);

	switch (circleStruct->startSide)
	{
	case PlaygroundSide::Bottom:
		calculatePoints(circleStruct, circleStruct->startPos.y, circleStruct->finishPos.y, circleStruct->startPos.x, circleStruct->finishPos.x, playgroundSize.height, playgroundSize.width);
		break;
	case PlaygroundSide::Top:
		calculatePoints(circleStruct, circleStruct->finishPos.y, circleStruct->startPos.y, circleStruct->startPos.x, circleStruct->finishPos.x, playgroundSize.height, playgroundSize.width);
		break;
	case PlaygroundSide::Left:
		calculatePoints(circleStruct, circleStruct->startPos.x, circleStruct->finishPos.x, circleStruct->startPos.y, circleStruct->finishPos.y, playgroundSize.width, playgroundSize.height);
		break;
	case PlaygroundSide::Right:
		calculatePoints(circleStruct, circleStruct->finishPos.x, circleStruct->startPos.x, circleStruct->startPos.y, circleStruct->finishPos.y, playgroundSize.width, playgroundSize.height);
		break;
	default:
		break;
	}
}

void CircleGenerator::calculatePoints(CircleStruct * circleStruct, float & d1, float & d2, float & s1, float & s2, float size1, float size2)
{
	float radius = normalCircleDiameter * 0.5f * circleStruct->scaleFactor;

	d1 = -radius;
	d2 = size1 + radius;

	s1 = RandomHelper::random_real<float>(radius, size2 - radius);
	s2 = RandomHelper::random_real<float>(radius, size2 - radius);

}