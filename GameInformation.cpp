#include "GameInformation.h"

USING_NS_CC;
using namespace crossareagame;

GameInformation::GameInformation(int score, int lives) :
score(score), lives(lives), gameOver(false), scoreChanged(false), livesChanged(false)
{

}

void GameInformation::addLives(int add)
{
	lives += add;
	if (lives <= 0)
	{
		lives = 0;
		gameOver = true;
	}
	livesChanged = true;
}

bool GameInformation::isScoreChanged()
{
	bool flagValue = scoreChanged;
	scoreChanged = false;
	return flagValue;
}

bool GameInformation::isLivesChanged()
{
	bool flagValue = livesChanged;
	livesChanged = false;
	return flagValue;
}