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

	public: //get-set interface
		PlaygroundLayer * getPlaygroundLayer() { return playgroundLayer; }
		std::shared_ptr<GameInformation> getGameInformation() { return gameInformation; }
		std::shared_ptr<CircleGenerator> getCircleGenerator() { return circleGenerator; }

		std::shared_ptr<Script> getScript() { return commonScript; }
		void setScript(const std::shared_ptr<Script> & script);

		cocos2d::Label * getScoreLabel() { return scoreLabel; }
		cocos2d::Label * getLivesLabel() { return livesLabel; }

	public:  //central Promt label interface
		void showCentralLabel(const std::string & text, bool touchControl);

	private:
		cocos2d::Label * promtLabel;

	private:
		PlaygroundLayer * playgroundLayer;
		std::shared_ptr<GameInformation> gameInformation;
		std::shared_ptr<CircleGenerator> circleGenerator;

		std::shared_ptr<Script> commonScript;
		std::shared_ptr<Script> secondScript;
		std::shared_ptr<Script> currentScript;
		int secondScriptMode; // =0 if OneSideFastScript, =1 if EverySidesScript

		cocos2d::Label * scoreLabel;
		cocos2d::Label * livesLabel;

	private: //services members
		std::ostringstream toStringConverter;
		std::string getStringFromInt(int valueToString);

	};

}

#endif //__GAMELAYER_H__