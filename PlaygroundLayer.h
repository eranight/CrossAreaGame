#ifndef __PLAYGROUND_LAYER_H__
#define __PLAYGROUND_LAYER_H__

#include "cocos2d.h"
#include "CircleGenerator.h"
#include "Gameplay.h"
#include <memory>

namespace crossareagame
{

	class PlaygroundLayer : public cocos2d::Layer
	{
	public:

		bool init(const cocos2d::Size & playgroundSize);

		static PlaygroundLayer * create(const cocos2d::Size & playgroundSize);

		bool onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event) override;
		void onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event) override;
		void onTouchCancelled(cocos2d::Touch * touch, cocos2d::Event * event) override;

		void stopLayer();

		cocos2d::Rect getBoundingBox() const override;
		cocos2d::Size getPlaygroundSize() { return playgroundSize; }

	public:
		void addCircle(CircleStruct * circleStruct);
		void circleGoAway(cocos2d::Node * node);

		using cirstrfunc = std::function<void(CircleStruct *)>;
		cirstrfunc tapCircle;
		cirstrfunc missCircle;
		std::function<void()> missTap;

		cocos2d::Vector<cocos2d::Sprite *> circles;

	private:

		cocos2d::Size playgroundSize;
	};

}

#endif //__PLAYGROUND_LAYER_H__