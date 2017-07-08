#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include <sstream>

#include "cocos2d.h"
#include "GameInformation.h"
#include "PlaygroundLayer.h"
#include "Script.h"

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
	virtual void update(float dt) override;
	
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
	
	std::shared_ptr<crossareagame::GameInformation> gameInfo;
	std::shared_ptr<crossareagame::CircleGenerator> generator;
	crossareagame::PlaygroundLayer * playgroundLayer;
	std::shared_ptr<crossareagame::Script> script;

	cocos2d::Label * labelCoins;
	std::ostringstream intToStringConverter;
};

#endif // __HELLOWORLD_SCENE_H__
