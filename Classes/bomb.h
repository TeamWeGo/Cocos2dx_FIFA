#pragma once
#include "cocos2d.h"
using namespace cocos2d;

class Factory :public cocos2d::Ref {
public:
	//获取单例工厂
	static Factory* getInstance();
	//生成炸弹，并存储到容器中管理
	Sprite * createBomb();
	//炸弹定时爆炸
	void exploration();
	//玩家自动引爆某炸弹移除炸弹
	void removeBomb(Sprite*);
	//判断碰撞
	Sprite* collider(Rect rect);
	//初始化爆炸帧动画
	void initSpriteFrame();

private:
	Factory();
	Vector<Sprite*> bomb;
	cocos2d::Vector<SpriteFrame*> explore;
	static Factory* factory;
	int bomb_count;
};