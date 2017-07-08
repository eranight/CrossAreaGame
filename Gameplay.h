#ifndef __GAMEPLAY_H__
#define __GAMEPLAY_H__

#include "Utils.h"
#include "CircleGenerator.h"
#include <memory>

namespace crossareagame
{
	class Gameplay
	{
	public:
		Gameplay(CircleGenerator * circleGenerator);
		~Gameplay() = default;

	public:
		virtual void circleTouched(CircleStruct * circleStruct);
		virtual void missTouched();
		virtual void circleMissed(CircleStruct * circleStruct);

		virtual void update(float dt);

		bool isGameOver() { return gameOver; }
		bool isReadyNextCircleGeneration() { return readyNextCircleGeneration; } //returns true while method getNextCircle was called less that numberOfGeneratedCircles times
		CircleStruct * getNextCircle();

	public: //generator interface
		std::shared_ptr<CircleGenerator> getGenerator() { return generator; }

	public: //generation time interface
		TimesCounter<float> nextGenerationTimes;

	public: //number of circles interface
		TimesCounter<int> generatedCirclesNumber;

	public: //coins interface
		int getCoins() { return coins; }
		void setCoins(int coins) { this->coins = coins; } //do not use the method
		bool goal() { bool icc = couinsChanged; couinsChanged = false; return icc; } //return true if the coins changed 

	protected:
		bool gameOver;
		bool readyNextCircleGeneration;
		bool couinsChanged;

		int coins;

		std::shared_ptr<CircleGenerator> generator;
	};
}

#endif //__GAMEPLAY_H__