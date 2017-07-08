#include "GameLayer.h"
#include "PlaygroundLayer.h"
#include "GameInformation.h"
#include "CircleGenerator.h"
#include "Script.h"

USING_NS_CC;
using namespace crossareagame;

static const int Z_label = 1;
static const int Z_playground = 0;

Scene* GameLayer::createScene()
{
	auto scene = Scene::create();
	auto layer = GameLayer::create();
	//layer->setScript(std::make_shared<OneSideFastScript>(layer));
	//layer->PushScript(std::make_shared<EasyScript>(layer));
	scene->addChild(layer);
	return scene;
}

bool GameLayer::init()
{
	if (!Layer::init())
		return false;

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Size playgroundSize = visibleSize;

	startPromtVisible = true;
	auto labelPromt = Label::createWithTTF("tap a circle that does not let it cross the screen\n(tap to continue)", "fonts/Marker Felt.ttf", 40, Size(visibleSize.width * 0.5f, 0.0f), TextHAlignment::CENTER);
	labelPromt->setColor(Color3B::BLACK);
	labelPromt->setPosition(origin + visibleSize * 0.5f);
	addChild(labelPromt, Z_label, 99);
	setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	setTouchEnabled(true);
	

	playgroundLayer = PlaygroundLayer::create(playgroundSize);
	playgroundLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	playgroundLayer->setPosition(origin + (visibleSize * 0.5f));

	gameInformation = std::make_shared<GameInformation>();
	circleGenerator = std::make_shared<CircleGenerator>(playgroundSize, 256.0f);

	int fontSize = 32;
	float labelYCoordinate = origin.y + visibleSize.height - fontSize * 0.55f;

	scoreLabel = Label::createWithTTF("score: " + getStringFromInt(0), "fonts/Marker Felt.ttf", fontSize);
	scoreLabel->setPosition(Vec2(origin.x + visibleSize.width * 0.25f, labelYCoordinate));
	scoreLabel->setColor(Color3B::BLACK);

	livesLabel = Label::createWithTTF("lives: " + getStringFromInt(gameInformation->getLives()), "fonts/Marker Felt.ttf", fontSize);
	livesLabel->setPosition(Vec2(origin.x + visibleSize.width * 0.75f, labelYCoordinate));
	livesLabel->setColor(Color3B::BLACK);
	
	this->addChild(playgroundLayer, Z_playground);
	this->addChild(scoreLabel, Z_label);
	this->addChild(livesLabel, Z_label);

	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event * event)
	{
		if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
			Director::getInstance()->end();
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

	commonScript = std::make_shared<EasyScript>(this);
	//secondScript = std::make_shared<EverySideScript>(this);

	return true;
}

void GameLayer::update(float dt)
{
	if (startPromtVisible)
		return;
	if (!scriptsStack.empty())
	{
		scriptsStack.top()->update(dt);
		if (scriptsStack.top()->isFinalized())
			scriptsStack.pop();
	}
	/*if (scriptsStack.top()->isScriptDead())
	{
		scriptsStack.pop();
	}
	if (!scriptsStack.empty())
		scriptsStack.top()->update(dt);*/

	if (!commonScript->isFinalized())
		commonScript->update(dt);
	else
	{
		if (secondScript == nullptr)
		{
			if (RandomHelper::random_int<int>(1, 100) % 2 == 0)
				secondScript = std::make_shared<OneSideFastScript>(this);
			else
				secondScript = std::make_shared<EverySideScript>(this);
			secondScript->refresh();
		}

		if (!secondScript->isFinalized())
			secondScript->update(dt);
		else
		{
			secondScript.reset();
			commonScript->refresh();
		}
			
	}
		

	if (gameInformation->isScoreChanged())
		scoreLabel->setString("score: " + getStringFromInt(gameInformation->getScore()));

	if (gameInformation->isLivesChanged())
		livesLabel->setString("lives: " + getStringFromInt(gameInformation->getLives()));

	if (gameInformation->isGameOver())
	{
		auto visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();
		auto labelGameOver = Label::createWithTTF("GAME OVER", "fonts/Marker Felt.ttf", 40, Size(visibleSize.width * 0.5f, 0.0f), TextHAlignment::CENTER);
		labelGameOver->setColor(Color3B::BLACK);
		labelGameOver->setPosition(origin + visibleSize * 0.5f);
		this->addChild(labelGameOver, 4, Z_label);

		playgroundLayer->stopLayer();
		unscheduleUpdate();
	}
		//Director::getInstance()->end();
}

bool GameLayer::onTouchBegan(Touch * touch, Event * event)
{
	removeChildByTag(99);
	startPromtVisible = false;
	setTouchEnabled(false);
	playgroundLayer->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	playgroundLayer->setTouchEnabled(true);
	scheduleUpdate();
	commonScript->refresh();
	return true;
}

void GameLayer::setScript(const std::shared_ptr<Script> & script)
{
	//this->script = script;
}


//SERVICES MEMBERS

std::string GameLayer::getStringFromInt(int valueToString)
{
	toStringConverter.str("");
	toStringConverter.clear();
	toStringConverter << valueToString;
	return toStringConverter.str();
}

//STACK OF SCRIPTS INTERFACE

void GameLayer::PushScript(const std::shared_ptr<Script> & script)
{
	scriptsStack.push(script);
}

void GameLayer::PopScript()
{
	scriptsStack.pop();
}