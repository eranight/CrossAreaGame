#include "GameLayer.h"
#include "PlaygroundLayer.h"
#include "GameInformation.h"
#include "CircleGenerator.h"
#include "Script.h"

USING_NS_CC;
using namespace crossareagame;

static const int Z_label = 4;
static const int Z_playground = 0;

Scene* GameLayer::createScene()
{
	auto scene = Scene::create();
	auto layer = GameLayer::create();
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

	promtLabel = Label::createWithTTF("", "fonts/Marker Felt.ttf", 40, Size(visibleSize.width * 0.5f, 0.0f), TextHAlignment::CENTER);
	promtLabel->setColor(Color3B::BLACK);
	promtLabel->setPosition(origin + visibleSize * 0.5f);
	promtLabel->setVisible(false);
	addChild(promtLabel, Z_label);

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
	currentScript = commonScript;
	secondScriptMode = 0;

	showCentralLabel("tap a circle that does not let it cross the screen\n(tap to continue)", true, ScriptEvent::PREV_START);

	return true;
}

void GameLayer::update(float dt)
{
	if (promtLabel->isVisible())
		return;

	if (!commonScript->isFinalized())
		commonScript->update(dt);
	else
	{
		if (secondScript == nullptr)
		{
			int scriptMode = secondScriptMode % 3;
			switch (scriptMode)
			{
			case 0:
				secondScript = std::make_shared<OneSideFastScript>(this);
				break;
			case 1:
				secondScript = std::make_shared<EverySideScript>(this);
				break;
			case 2:
				secondScript = std::make_shared<OnlyBoardColor>(this, secondScriptMode == 2);
				break;
			default:
				break;
			}
			secondScript->refresh();
			secondScriptMode++;
			currentScript = secondScript;
		}

		if (!secondScript->isFinalized())
			secondScript->update(dt);
		else
		{
			commonScript->refresh();
			secondScript.reset();

			currentScript = commonScript;
		}
			
	}

	if (gameInformation->isScoreChanged())
		scoreLabel->setString("score: " + getStringFromInt(gameInformation->getScore()));

	if (gameInformation->isLivesChanged())
		livesLabel->setString("lives: " + getStringFromInt(gameInformation->getLives()));

	if (gameInformation->isGameOver())
	{
		showCentralLabel("GAME OVER", false, ScriptEvent::POST_FINISH);

		playgroundLayer->stopLayer();
	}
		//Director::getInstance()->end();
}

bool GameLayer::onTouchBegan(Touch * touch, Event * event)
{
	promtLabel->setVisible(false);
	setTouchEnabled(false);
	playgroundLayer->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	playgroundLayer->setTouchEnabled(true);
	scheduleUpdate();
	if (refreshCurrentScript)
		currentScript->refresh();
	return true;
}

//CENTRAL LABEL INTERFACE

void GameLayer::showCentralLabel(const std::string & text, bool touchControl, const crossareagame::ScriptEvent & scriptEvent)
{
	if (touchControl)
	{
		playgroundLayer->setTouchEnabled(false);
		setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
		setTouchEnabled(true);
	}
	
	if (scriptEvent == ScriptEvent::PREV_START)
		refreshCurrentScript = true;
	else if (scriptEvent == ScriptEvent::POST_FINISH)
		refreshCurrentScript = false;

	promtLabel->setString(text);
	promtLabel->setVisible(true);

	unscheduleUpdate();
}

//SERVICES MEMBERS

std::string GameLayer::getStringFromInt(int valueToString)
{
	toStringConverter.str("");
	toStringConverter.clear();
	toStringConverter << valueToString;
	return toStringConverter.str();
}