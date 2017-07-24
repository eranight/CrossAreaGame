#include "PlaygroundLayer.h"


USING_NS_CC;
using namespace crossareagame;

PlaygroundLayer * PlaygroundLayer::create(const Size & playgroundSize)
{
	auto ret = new (std::nothrow) PlaygroundLayer();
	if (ret != nullptr && ret->init(playgroundSize))
		ret->autorelease();
	else
		CC_SAFE_DELETE(ret);
	return ret;
}

bool PlaygroundLayer::init(const Size & playgroundSize)
{
	if (!Layer::init())
		return false;

	this->playgroundSize = playgroundSize;

	auto playground = DrawNode::create();
	playground->setName("playground");
	playground->drawSolidRect(this->convertToNodeSpace(this->getPosition() - playgroundSize * 0.5f), this->convertToNodeSpace(this->getPosition() + playgroundSize * 0.5f), Color4F(Color3B(244, 255, 249)));
	this->addChild(playground);

	return true;
}

void PlaygroundLayer::stopLayer()
{
	for (auto & circle : circles)
	{
		circle->stopAllActions();
	}
	setTouchEnabled(false);
}

void PlaygroundLayer::circleGoAway(Node * node)
{
	if (missCircle != nullptr)
	{
		missCircle(dynamic_cast<CircleStruct *>(node->getUserObject()));
	}
	circles.eraseObject(dynamic_cast<Sprite *>(node));
}

void PlaygroundLayer::addCircle(CircleStruct * circleStruct)
{
	auto circleSprite = Sprite::createWithSpriteFrameName("circle");
	circleSprite->setScale(circleStruct->scaleFactor);
	circleSprite->setColor(Color3B(circleStruct->color));
	circleSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	circleSprite->setPosition(circleStruct->startPos - playgroundSize * 0.5f);

	circleSprite->runAction(Sequence::create(
		MoveTo::create(circleStruct->crosstime, circleStruct->finishPos - playgroundSize * 0.5f),
		CallFuncN::create(CC_CALLBACK_0(PlaygroundLayer::circleGoAway, this, circleSprite)),
		RemoveSelf::create(),
		nullptr
	));


	circleSprite->setUserObject(circleStruct);
	this->addChild(circleSprite, 1);
	circles.pushBack(circleSprite);
}

Rect PlaygroundLayer::getBoundingBox() const
{
	Rect rect(this->convertToNodeSpace(this->getPosition() - playgroundSize * 0.5f), playgroundSize);
	return rect;
}

bool PlaygroundLayer::onTouchBegan(Touch * touch, Event * event)
{
	bool circleHit = false;
	if (tapCircle != nullptr)
		for (int index = circles.size() - 1; index >= 0; --index)
		{
			Sprite * child = circles.at(index);
			if (child->getBoundingBox().containsPoint(this->convertTouchToNodeSpace(touch)))
			{
				child->stopAllActions();
				tapCircle(dynamic_cast<CircleStruct *>(child->getUserObject()));
				circleHit = true;
				circles.eraseObject(child);
				child->runAction(RemoveSelf::create());
				break;
			}
		}

	if (!circleHit && missTap != nullptr)
		missTap();


	return true;
}

void PlaygroundLayer::onTouchEnded(Touch * touch, Event * event)
{

}

void PlaygroundLayer::onTouchCancelled(Touch * touch, Event * event)
{

}