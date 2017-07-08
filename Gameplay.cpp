#include "Gameplay.h"

USING_NS_CC;
using namespace crossareagame;

using std::shared_ptr;

Gameplay::Gameplay(CircleGenerator * circleGenerator) :
generator(circleGenerator),
gameOver(false),
readyNextCircleGeneration(false),
couinsChanged(false),
coins(0),
nextGenerationTimes(0.0f, 0.0f),
generatedCirclesNumber(0, 0)
{
	
}

void Gameplay::update(float dt)
{
	if (gameOver)
		return;

	if (!nextGenerationTimes.isFull())
	{
		nextGenerationTimes.addict(dt);
		if (nextGenerationTimes.isFull())
		{
			readyNextCircleGeneration = true;
			nextGenerationTimes.reset();
		}
	}
}

CircleStruct * Gameplay::getNextCircle()
{
	if (!generatedCirclesNumber.isFull())
	{
		generatedCirclesNumber.addict(1);
		if (generatedCirclesNumber.isFull())
		{
			readyNextCircleGeneration = false;
			generatedCirclesNumber.reset();
		}
		return generator->getNextCircle();
	}
	return nullptr;
}

void Gameplay::circleTouched(CircleStruct * circleStruct)
{
	++coins;
	couinsChanged = true;
}

void Gameplay::missTouched()
{

}

void Gameplay::circleMissed(CircleStruct * circleStruct)
{
	/*--coins;
	if (coins < 0)
		gameOver = true;
	else
		couinsChanged = true;*/
}