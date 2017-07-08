#ifndef __GAMEINFORMATION_H__
#define __GAMEINFORMATION_H__

#include "cocos2d.h"

namespace crossareagame
{
	class GameInformation
	{
	public:
		GameInformation(int score = 0, int lives = 3);

	public:
		int getScore() { return score; }
		void setScore(int score) { this->score = score; scoreChanged = true; }
		void addScore(int add = 1) { score += add; scoreChanged = true; }

		int getLives() { return lives; }
		void setLives(int lives) { this->lives = lives; livesChanged = true; }
		void addLives(int add = 1);

		bool isScoreChanged();
		bool isLivesChanged();

		bool isGameOver() { return gameOver; }

	private:
		int score;
		int lives;
		bool gameOver;
		bool scoreChanged;
		bool livesChanged;
	};

}

#endif //__GAMEINFORMATION_H__