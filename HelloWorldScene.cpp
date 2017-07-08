#include "HelloWorldScene.h"

USING_NS_CC;
using namespace crossareagame;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	if (!Layer::init())
	{
		return false;
	}

	SpriteFrameCache::getInstance()->addSpriteFrame(SpriteFrame::create("circle.png", Rect(0.0f, 0.0f, 256.0f, 256.0f)), "circle");

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Size playgroundSize = visibleSize;// Size(600.0f, 800.0f);

	playgroundLayer = crossareagame::PlaygroundLayer::create(playgroundSize);
	playgroundLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	playgroundLayer->setPosition(origin + (visibleSize * 0.5f));
	addChild(playgroundLayer);

	gameInfo = std::make_shared<GameInformation>();
	generator = std::make_shared<CircleGenerator>(playgroundSize, 256.0f * Director::getInstance()->getContentScaleFactor());

	script = std::shared_ptr<Script>(new EasyModeScript(nullptr));

	intToStringConverter << gameInfo->getScore();
	labelCoins = Label::createWithTTF(intToStringConverter.str(), "fonts/Marker Felt.ttf", 24);
	labelCoins->setPosition(Vec2(origin.x + visibleSize.width * 0.5f, origin.y + visibleSize.height - labelCoins->getContentSize().height * 0.55f));
	this->addChild(labelCoins);

	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event * event)
	{
		if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
			this->menuCloseCallback(nullptr);
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);
	scheduleUpdate();
    
    return true;
}

void HelloWorld::update(float dt)
{
	script->update(dt);
	if (gameInfo->isScoreChanged())
	{
		intToStringConverter.str("");
		intToStringConverter.clear();
		intToStringConverter << gameInfo->getScore();
		labelCoins->setString(intToStringConverter.str());
	}
	if (gameInfo->isLivesChanged())
	{

	}
	if (gameInfo->isGameOver())
		menuCloseCallback(nullptr);
	/*while (gameplay->isReadyNextCircleGeneration())
	{
		playgroundLayer->addCircle(gameplay->getNextCircle());
	}
	if (gameplay->goal())
	{
		intToStringConverter.str("");
		intToStringConverter.clear();
		intToStringConverter << gameplay->getCoins();
		labelCoins->setString(intToStringConverter.str());
	}*/
		
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
