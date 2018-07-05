#pragma once
#include "cocos2d.h"
#include <stdio.h>
using namespace cocos2d;
using namespace std;

class GameScene : public cocos2d::Scene
{
public:
	static PhysicsWorld* world;
	static cocos2d::Scene* createScene();
	void setPhysicsWorld(PhysicsWorld * world);
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);

	void moveCharacter1(char);		//移动玩家1
	void moveCharacter2(char);		//移动玩家2
	void hurtEvent(Ref*);				//被炸到
	void update(float);	

	void updateBomb(float);				//产生炸弹
	void createBomb();
	void explorate(float);

	void preloadMusic();				//预加载音效
	void addSprite();					// 添加背景和各种精灵
	void addListener();					// 添加监听器

	void onKeyPressed(EventKeyboard::KeyCode code, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode code, Event* event);
	bool onConcactBegin(PhysicsContact & contact);

private:
	PhysicsWorld * m_world;
	cocos2d::Sprite* player1;
	cocos2d::Sprite* player2;
	//cocos2d::Vector<SpriteFrame*> attack1;	//踢球动作
	//cocos2d::Vector<SpriteFrame*> dead1;		//死亡动作
	cocos2d::Vector<SpriteFrame*> run_left1;		//移动动作
	cocos2d::Vector<SpriteFrame*> run_right1;
	cocos2d::Vector<SpriteFrame*> run_up1;
	cocos2d::Vector<SpriteFrame*> run_down1;
	//cocos2d::Vector<SpriteFrame*> idle1;		//不移动
	//cocos2d::Vector<SpriteFrame*> attack2;	//踢球动作
	//cocos2d::Vector<SpriteFrame*> dead2;		//死亡动作
	cocos2d::Vector<SpriteFrame*> run_left2;		//移动动作
	cocos2d::Vector<SpriteFrame*> run_right2;
	cocos2d::Vector<SpriteFrame*> run_up2;
	cocos2d::Vector<SpriteFrame*> run_down2;
	//cocos2d::Vector<SpriteFrame*> idle2;		//不移动
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
	//cocos2d::Label* time;
	//int dtime;
	cocos2d::ProgressTimer* pT1;				//玩家1血条
	cocos2d::ProgressTimer* pT2;				//玩家2血条

	//玩家方向
	int direction1 = 1, direction2 = 3;
	
	//是否移动
	bool isMove1 = false, isMove2 = false;
	char moveKey1, moveKey2;

	//比分
	int score;
	cocos2d::Label* number;

	//各方向初始状态
	cocos2d::SpriteFrame* frame01_right;
	cocos2d::SpriteFrame* frame01_left;
	cocos2d::SpriteFrame* frame01_up;
	cocos2d::SpriteFrame* frame01_down;
	cocos2d::SpriteFrame* frame02_right;
	cocos2d::SpriteFrame* frame02_left;
	cocos2d::SpriteFrame* frame02_up;
	cocos2d::SpriteFrame* frame02_down;

	list<char> keystatep1;
	list<char> keystatep2;

	//足球
	cocos2d::Sprite* football;
};
