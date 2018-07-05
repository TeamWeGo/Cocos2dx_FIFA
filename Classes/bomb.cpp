#include"bomb.h"
USING_NS_CC;

Factory* Factory::factory = NULL;
Factory::Factory() {
	initSpriteFrame();
}
Factory* Factory::getInstance() {
	if (factory == NULL) {
		factory = new Factory();
	}
	return factory;
}
void Factory::initSpriteFrame() {
	auto texture = Director::getInstance()->getTextureCache()->addImage("explore.png");
	explore.reserve(8);
	for (int i = 0; i < 5; i++)
	{
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(193 * i + 1, 0, 142, 142)));
		explore.pushBack(frame);
	}
	for (int i = 0; i < 3; i++)
	{
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(193 * i + 1, 193, 142, 142)));
		explore.pushBack(frame);
	}
}

Sprite * Factory::createBomb() {
	Sprite* bo = Sprite::create("bomb.png");
	bomb.pushBack(bo);
	return bo;
}
void Factory::exploration() {
	for (auto i : bomb) {
		i->stopAllActions();
		Animation* anim = Animation::createWithSpriteFrames(explore, 0.05f, 1);
		Animate* ani = Animate::create(anim);
		Sequence* seq = Sequence::create(ScaleBy::create(1.f, 1.5f),ani ,CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, i)), NULL);
		i->runAction(seq);
	}
	bomb.clear();
}
void Factory::removeBomb(Sprite* sp) {
	Animation* anim = Animation::createWithSpriteFrames(explore, 0.2);
	Animate* ani = Animate::create(anim);
	Sequence* seq = Sequence::create(ani, CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, sp)), NULL);
	sp->runAction(seq);
	bomb.eraseObject(sp);
}

Sprite* Factory::collider(Rect rect) {
	/*for (auto i : monster) {
		if (rect.containsPoint(i->getPosition())) {
			return i;
		}
	}*/
	return NULL;
}

