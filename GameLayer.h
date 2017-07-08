#ifndef __GAMELAYER_H__
#define __GAMELAYER_H__

#include "cocos2d.h"

namespace crossareagame
{

	class PlaygroundLayer;
	class GameInformation;
	class CircleGenerator;
	class Script;

	class GameLayer : public cocos2d::Layer
	{
	public:
		static cocos2d::Scene* createScene();
		bool init() override;
		void update(float dt) override;
		CREATE_FUNC(GameLayer);
		bool onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event) override;

	public: //stack of scripts interface
		void PushScript(const std::shared_ptr<Script> & script);
		void PopScript();
		std::shared_ptr<Script> TopScript() { return scriptsStack.top(); }

	public: //get-set interface
		PlaygroundLayer * getPlaygroundLayer() { return playgroundLayer; }
		std::shared_ptr<GameInformation> getGameInformation() { return gameInformation; }
		std::shared_ptr<CircleGenerator> getCircleGenerator() { return circleGenerator; }

		std::shared_ptr<Script> getScript() { return commonScript; }
		void setScript(const std::shared_ptr<Script> & script);

		cocos2d::Label * getScoreLabel() { return scoreLabel; }
		cocos2d::Label * getLivesLabel() { return livesLabel; }

	private:
		bool startPromtVisible;

	private:
		PlaygroundLayer * playgroundLayer;
		std::shared_ptr<GameInformation> gameInformation;
		std::shared_ptr<CircleGenerator> circleGenerator;

		std::shared_ptr<Script> commonScript;
		std::shared_ptr<Script> secondScript;
		bool switchScript;

		cocos2d::Label * scoreLabel;
		cocos2d::Label * livesLabel;

		std::stack<std::shared_ptr<Script>> scriptsStack;

	private: //services members
		std::ostringstream toStringConverter;
		std::string getStringFromInt(int valueToString);

	};

}

#endif //__GAMELAYER_H__