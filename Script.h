#ifndef __SCRIPT_H__
#define __SCRIPT_H__

#include <memory>
#include "Utils.h"

namespace crossareagame
{
	class GameLayer;
	class PlaygroundLayer;
	class CircleGenerator;
	class GameInformation;
	class CircleStruct;

	class Script
	{
	public:
		Script(GameLayer * gameLayer);
		virtual ~Script() = default;

	public:
		//call refresh to set start configuration
		virtual void refresh() = 0;
		virtual void update(float dt) = 0;

	public: //finalize script interface
		virtual void runFinalizeProcess(); //set finalizeProcessing flag
		bool isFinalizeProcess() { return finalizeProcessing; }
		bool isFinalized() { return finalized; }

	protected:
		bool finalized; //true if Script do not generate circle anymore and all previously generated circles are destroyed;
		bool finalizeProcessing; //true if Script is in finalize process, but is not finalized yet;

	protected:
		GameLayer * gameLayer;

	};

	class EasyScript : public Script
	{
	public:
		EasyScript(GameLayer * gameLayer);

	public:
		void refresh() override;
		void update(float dt) override;

	private:
		crossareagame::TimesCounter<float> generationTime;
		crossareagame::TimesCounter<int>   cicleCounter;

		float nextGenerationTimePercent;
		float minScalePercent;
		float minCrossTimePercent;
	};

	class WarningLabelEvent
	{
	public:
		WarningLabelEvent();
		~WarningLabelEvent();

	public:
		cocos2d::Action * getActionClone();

	private:
		cocos2d::Sequence * warningLabelAction;

	};

	class OneSideFastScript : public Script
	{
	public:
		OneSideFastScript(GameLayer * gameLayer);
		~OneSideFastScript();

	public:
		void refresh() override;
		void update(float dt) override;
		void runFinalizeProcess() override;

	private:
		crossareagame::PlaygroundSide side;
		float crossTime;
		crossareagame::TimesCounter<float> generationTime;
		crossareagame::TimesCounter<int>   cicleCounter;

		cocos2d::Label * warningLabel;
		WarningLabelEvent warningLabelEvent;
	};


	class EverySideScript : public Script
	{
	public:
		EverySideScript(GameLayer * gameLayer);
		~EverySideScript();

	public:
		void refresh() override;
		void update(float dt) override;
		void runFinalizeProcess() override;

	private:
		cocos2d::Vector<cocos2d::Label *> warningLabels;
		WarningLabelEvent warningLabelEvent;
		float crossTime;

	};
}

#endif //__SCRIPT_H__