#include "cocos2d.h"
#include "Script.h"
#include "GameLayer.h"
#include "PlaygroundLayer.h"
#include "CircleGenerator.h"
#include "GameInformation.h"

USING_NS_CC;
using namespace crossareagame;

static std::vector<cocos2d::Color4B> defaultColors = { Color4B(22, 147, 165, 255), Color4B(251, 184, 41, 255), Color4B(224, 36, 119, 255), Color4B(240, 232, 12, 255) };

Script::Script(GameLayer * gameLayer) :
gameLayer(gameLayer),
finalizeProcessing(false),
finalized(false)
{
}

void Script::runFinalizeProcess()
{
	finalizeProcessing = true;
}


/////////////////////////////////////////////////////////////////////////
///////                         EasyScript                        ///////
/////////////////////////////////////////////////////////////////////////

EasyScript::EasyScript(GameLayer * gameLayer) :
	Script(gameLayer)
{
}

void EasyScript::refresh()
{
	float scalePer = 0.5f * Director::getInstance()->getContentScaleFactor();

	finalized = false;
	finalizeProcessing = false;

	int score = gameLayer->getGameInformation()->getScore();
	int divscore = score / 5;

	minScalePercent = 100.0f - std::min(5.0f * divscore, 50.0f);
	minCrossTimePercent = 0.0f + std::min(3.0f * divscore, 60.0f);
	nextGenerationTimePercent = 0.0f + std::min(10.0f * divscore, 70.0f);

	generationTime.reset();
	generationTime.setCurrect(0.0f);
	generationTime.setMaximum(2.0f - 0.01f * nextGenerationTimePercent);

	cicleCounter.reset();
	cicleCounter.setCurrect(0);
	cicleCounter.setMaximum(10);

	CircleGeneratorConfiguration configuration;
	configuration.scalePercent.first = minScalePercent * scalePer; configuration.scalePercent.second = 100.0f * scalePer;
	configuration.crossTime.first = 4.0f - 0.04f * minCrossTimePercent; configuration.crossTime.second = 5.0f - 0.05f * minCrossTimePercent;
	configuration.colors = defaultColors;
	gameLayer->getCircleGenerator()->setConfiguration(configuration);
	
	gameLayer->getPlaygroundLayer()->tapCircle = [this, scalePer](CircleStruct * circleStruct)
	{
		this->gameLayer->getGameInformation()->addScore();

		int score = this->gameLayer->getGameInformation()->getScore();
		if (score % 5 == 0)
		{
			if (this->minScalePercent > 30.0f)
			{
				this->minScalePercent -= 5.0f;
				this->gameLayer->getCircleGenerator()->setScalePercent(this->minScalePercent * scalePer, 100.0f * scalePer);
			}
			if (this->minCrossTimePercent < 60.0f)
			{
				this->minCrossTimePercent += 3.0f;
				this->gameLayer->getCircleGenerator()->setCrossTime(4.0f - 0.04f * this->minCrossTimePercent, 5.0f - 0.05f * this->minCrossTimePercent);
			}
			if (this->nextGenerationTimePercent < 50)
			{
				this->nextGenerationTimePercent += 10;
				this->generationTime.setMaximum(2.0f - 0.01f * this->nextGenerationTimePercent);
			}

		}
	};

	gameLayer->getPlaygroundLayer()->missCircle = [this](CircleStruct * circleStruct)
	{
		this->gameLayer->getGameInformation()->addLives(-1);
	};
}

void EasyScript::update(float dt)
{
	auto playground = gameLayer->getPlaygroundLayer();
	if (!finalizeProcessing && !finalized)
	{
		generationTime.addict(dt);
		if (generationTime.isFull())
		{
			playground->addCircle(gameLayer->getCircleGenerator()->getNextCircle());
			generationTime.reset();
			cicleCounter.addict(1);
			if (cicleCounter.isFull())
				runFinalizeProcess();
		}
	}
	else if (finalizeProcessing)
	{
		if (playground->circles.empty())
		{
			finalizeProcessing = false;
			finalized = true;
		}
	}
}



/////////////////////////////////////////////////////////////////////////
///////                     OneSideFastScript                     ///////
/////////////////////////////////////////////////////////////////////////

OneSideFastScript::OneSideFastScript(GameLayer * gameLayer) :
	Script(gameLayer)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	warningLabel = Label::createWithTTF("! ! !", "fonts/Marker Felt.ttf", 50, Size(visibleSize.width * 0.5f, 0.0f), TextHAlignment::CENTER);
	warningLabel->setColor(Color3B::RED);
	warningLabel->setPosition(Vec2(origin.x + visibleSize.width * 0.5f, origin.y + warningLabel->getContentSize().height * 0.5f));
	gameLayer->addChild(warningLabel, 4);
	warningLabel->setVisible(false);
}

OneSideFastScript::~OneSideFastScript()
{
	gameLayer->removeChild(warningLabel);
}

void OneSideFastScript::refresh()
{
	float scalePer = 0.5f * Director::getInstance()->getContentScaleFactor();

	finalized = false;
	finalizeProcessing = false;

	int score = gameLayer->getGameInformation()->getScore();

	float genTime = 1.0f - std::min(0.5f, (score / 20) * 0.1f);
	generationTime.reset();
	generationTime.setCurrect(genTime);
	generationTime.setMaximum(genTime);

	cicleCounter.reset();
	cicleCounter.setCurrect(0);
	cicleCounter.setMaximum(10);

	side = PlaygroundSide::Bottom;
	crossTime = 2.5f - std::min(1.0f, (score / 10) * 0.1f);

	CircleGeneratorConfiguration configuration;
	configuration.scalePercent.first = (100.0f - std::min(5.0f * (score / 5), 70.0f)) * scalePer; configuration.scalePercent.second = 100.0f * scalePer;
	configuration.crossTime.first = crossTime; configuration.crossTime.second = crossTime;
	configuration.colors = defaultColors;
	gameLayer->getCircleGenerator()->setConfiguration(configuration);

	warningLabel->runAction(warningLabelEvent.getActionClone());

	gameLayer->getPlaygroundLayer()->tapCircle = [this](CircleStruct * circleStruct)
	{
		this->gameLayer->getGameInformation()->addScore();
	};

	gameLayer->getPlaygroundLayer()->missCircle = [this](CircleStruct * circleStruct)
	{
		this->gameLayer->getGameInformation()->addLives(-1);
	};
}

void OneSideFastScript::update(float dt)
{
	auto playground = gameLayer->getPlaygroundLayer();
	if (!finalizeProcessing && !finalized)
	{
		if (warningLabel->getNumberOfRunningActions() == 0)
		{
			generationTime.addict(dt);
			if (generationTime.isFull())
			{
				CircleStruct * circleStruct = gameLayer->getCircleGenerator()->getNextCircleWithSide(side);
				playground->addCircle(circleStruct);
				cicleCounter.addict(1);
				if (cicleCounter.isFull())
					runFinalizeProcess();
				generationTime.reset();
			}
		}
	}
	else if (finalizeProcessing)
	{
		if (playground->circles.empty())
		{
			finalizeProcessing = false;
			finalized = true;
		}
	}
}

void OneSideFastScript::runFinalizeProcess()
{
	Script::runFinalizeProcess();

	warningLabel->stopAllActions();
	warningLabel->setVisible(false);
}


/////////////////////////////////////////////////////////////////////////
///////                      EverySideScript                      ///////
/////////////////////////////////////////////////////////////////////////

EverySideScript::EverySideScript(GameLayer * gameLayer) :
Script(gameLayer)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	for (int index = 0; index < static_cast<int>(PlaygroundSide::COUNT); ++index)
	{
		auto warningLabel = Label::createWithTTF("! ! !", "fonts/Marker Felt.ttf", 50, Size(visibleSize.width * 0.5f, 0.0f), TextHAlignment::CENTER);
		warningLabel->setColor(Color3B::RED);
		gameLayer->addChild(warningLabel, 4);
		warningLabel->setVisible(false);
		warningLabels.pushBack(warningLabel);

		float wheight = warningLabel->getContentSize().height * 0.5f;
		switch (static_cast<PlaygroundSide>(index))
		{
		case PlaygroundSide::Bottom:
			warningLabel->setPosition(Vec2(origin.x + visibleSize.width * 0.5f, origin.y + wheight));
			break;
		case PlaygroundSide::Left:
		{
									 warningLabel->setPosition(Vec2(origin.x + wheight, origin.y + visibleSize.height * 0.5f));
									 warningLabel->setRotation(90.0f);
		}
			break;
		case PlaygroundSide::Top:
		{
									warningLabel->setPosition(Vec2(origin.x + visibleSize.width * 0.5f, origin.y + visibleSize.height - 2.0f * wheight));
									warningLabel->setRotation(180.0f);
		}
			break;
		case PlaygroundSide::Right:
		{
									  warningLabel->setPosition(Vec2(origin.x + visibleSize.width - wheight, origin.y + visibleSize.height * 0.5f));
									  warningLabel->setRotation(-90.0f);
		}
			break;
		default:
			break;
		}
	}
}

EverySideScript::~EverySideScript()
{
	for (auto & label : warningLabels)
		gameLayer->removeChild(label);
	warningLabels.clear();
}

void EverySideScript::refresh()
{
	float scalePer = 0.5f * Director::getInstance()->getContentScaleFactor();

	finalized = false;
	finalizeProcessing = false;

	int score = gameLayer->getGameInformation()->getScore();

	crossTime = 3.0f - std::min(1.0f, (score / 10) * 0.1f);

	CircleGeneratorConfiguration configuration;
	configuration.scalePercent.first = (100.0f - std::min(5.0f * (score / 5), 70.0f)) * scalePer; configuration.scalePercent.second = 100.0f * scalePer;
	configuration.crossTime.first = crossTime; configuration.crossTime.second = crossTime;
	configuration.colors = defaultColors;
	gameLayer->getCircleGenerator()->setConfiguration(configuration);

	for (auto & label : warningLabels)
		label->runAction(warningLabelEvent.getActionClone());

	gameLayer->getPlaygroundLayer()->tapCircle = [this](CircleStruct * circleStruct)
	{
		this->gameLayer->getGameInformation()->addScore();
	};

	gameLayer->getPlaygroundLayer()->missCircle = [this](CircleStruct * circleStruct)
	{
		this->gameLayer->getGameInformation()->addLives(-1);
	};
}

void EverySideScript::update(float dt)
{
	auto playground = gameLayer->getPlaygroundLayer();
	if (!finalizeProcessing && !finalized)
	{
		int cnt = 0;
		for (auto & label : warningLabels)
			cnt += label->getNumberOfRunningActions();
		if (cnt == 0)
		{
			auto circleGenerator = gameLayer->getCircleGenerator();
			for (int index = 0; index < static_cast<int>(PlaygroundSide::COUNT); ++index)
				playground->addCircle(circleGenerator->getNextCircleWithSide(static_cast<PlaygroundSide>(index)));
			runFinalizeProcess();
		}
	}
	else if (finalizeProcessing)
	{
		if (playground->circles.empty())
		{
			finalizeProcessing = false;
			finalized = true;
		}
	}
}

void EverySideScript::runFinalizeProcess()
{
	Script::runFinalizeProcess();

	for (auto & label : warningLabels)
	{
		label->stopAllActions();
		label->setVisible(false);
	}
}


/////////////////////////////////////////////////////////////////////////
///////                      OnlyBoardColor                       ///////
/////////////////////////////////////////////////////////////////////////

OnlyBoardColor::OnlyBoardColor(GameLayer * gameLayer, bool showAttempt) :
Script(gameLayer), showAttempt(showAttempt)
{
	
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	choosingColor = Color4F(defaultColors[RandomHelper::random_int<int>(0, defaultColors.size() - 1)]);

	int wdht = 20;
	float w_2 = wdht * 0.5f;
	auto frame = DrawNode::create();
	frame->setLineWidth(wdht);
	Vec2 points[] = {
		origin + Vec2(w_2, w_2),
		Vec2(origin.x + w_2, origin.y + visibleSize.height - w_2),
		origin + visibleSize - Vec2(w_2, w_2),
		Vec2(origin.x + visibleSize.width - w_2, origin.y + w_2),
		origin + Vec2(w_2, w_2)
	};
	frame->drawPolygon(points, 5, Color4F(0, 0, 0, 0), wdht, choosingColor);
	gameLayer->addChild(frame, 2, 99);

	if (showAttempt)
	{
		gameLayer->showCentralLabel("tap a circle wich has color like the frame\n(tap to continue)", true);
	}
}

OnlyBoardColor::~OnlyBoardColor()
{
	gameLayer->removeChildByTag(99);
}

void OnlyBoardColor::refresh()
{
	float scalePer = 0.5f * Director::getInstance()->getContentScaleFactor();

	finalized = false;
	finalizeProcessing = false;

	int score = gameLayer->getGameInformation()->getScore();
	int divscore = score / 5;

	float minScalePercent = 100.0f - std::min(5.0f * divscore, 50.0f);
	float minCrossTimePercent = 0.0f + std::min(3.0f * divscore, 60.0f);
	float nextGenerationTimePercent = 0.0f + std::min(10.0f * divscore, 70.0f);

	generationTime.reset();
	generationTime.setCurrect(0.0f);
	generationTime.setMaximum(2.0f - 0.01f * nextGenerationTimePercent);

	cicleCounter.reset();
	cicleCounter.setCurrect(0);
	cicleCounter.setMaximum(10);

	CircleGeneratorConfiguration configuration;
	configuration.scalePercent.first = minScalePercent * scalePer; configuration.scalePercent.second = 100.0f * scalePer;
	configuration.crossTime.first = 4.0f - 0.04f * minCrossTimePercent; configuration.crossTime.second = 5.0f - 0.05f * minCrossTimePercent;
	configuration.colors = defaultColors;
	gameLayer->getCircleGenerator()->setConfiguration(configuration);

	gameLayer->getPlaygroundLayer()->tapCircle = [this, scalePer](CircleStruct * circleStruct)
	{
		if (circleStruct->color == this->choosingColor)
			this->gameLayer->getGameInformation()->addScore();
		else
			this->gameLayer->getGameInformation()->addLives(-1);
	};

	gameLayer->getPlaygroundLayer()->missCircle = [this](CircleStruct * circleStruct)
	{
		if (circleStruct->color == this->choosingColor)
			this->gameLayer->getGameInformation()->addLives(-1);
	};
}

void OnlyBoardColor::update(float dt)
{
	auto playground = gameLayer->getPlaygroundLayer();
	if (!finalizeProcessing && !finalized)
	{
		generationTime.addict(dt);
		if (generationTime.isFull())
		{
			playground->addCircle(gameLayer->getCircleGenerator()->getNextCircle());
			playground->addCircle(gameLayer->getCircleGenerator()->getNextCircle());
			generationTime.reset();
			cicleCounter.addict(1);
			if (cicleCounter.isFull())
				runFinalizeProcess();
		}
	}
	else if (finalizeProcessing)
	{
		if (playground->circles.empty())
		{
			finalizeProcessing = false;
			finalized = true;
		}
	}
}

/////////////////////////////////////////////////////////////////////////
///////                     NO SCRIPT CLASS!                      ///////
///////                       WarningLabel                        ///////
/////////////////////////////////////////////////////////////////////////
WarningLabelEvent::WarningLabelEvent()
{
	warningLabelAction = Sequence::createWithTwoActions(
		Blink::create(3.0f, 3),
		CallFuncN::create([](Node * node)
		{
			node->setVisible(false);
		})
	);
	warningLabelAction->retain();
}

WarningLabelEvent::~WarningLabelEvent()
{
	warningLabelAction->release();
}

Action * WarningLabelEvent::getActionClone()
{
	return warningLabelAction->clone();
}