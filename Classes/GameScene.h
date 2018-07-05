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

	void moveCharacter1(char);		//�ƶ����1
	void moveCharacter2(char);		//�ƶ����2
	void hurtEvent(Ref*);				//��ը��
	void update(float);	

	void updateBomb(float);				//����ը��
	void createBomb();
	void explorate(float);

	void preloadMusic();				//Ԥ������Ч
	void addSprite();					// ��ӱ����͸��־���
	void addListener();					// ��Ӽ�����

	void onKeyPressed(EventKeyboard::KeyCode code, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode code, Event* event);
	bool onConcactBegin(PhysicsContact & contact);

private:
	PhysicsWorld * m_world;
	cocos2d::Sprite* player1;
	cocos2d::Sprite* player2;
	//cocos2d::Vector<SpriteFrame*> attack1;	//������
	//cocos2d::Vector<SpriteFrame*> dead1;		//��������
	cocos2d::Vector<SpriteFrame*> run_left1;		//�ƶ�����
	cocos2d::Vector<SpriteFrame*> run_right1;
	cocos2d::Vector<SpriteFrame*> run_up1;
	cocos2d::Vector<SpriteFrame*> run_down1;
	//cocos2d::Vector<SpriteFrame*> idle1;		//���ƶ�
	//cocos2d::Vector<SpriteFrame*> attack2;	//������
	//cocos2d::Vector<SpriteFrame*> dead2;		//��������
	cocos2d::Vector<SpriteFrame*> run_left2;		//�ƶ�����
	cocos2d::Vector<SpriteFrame*> run_right2;
	cocos2d::Vector<SpriteFrame*> run_up2;
	cocos2d::Vector<SpriteFrame*> run_down2;
	//cocos2d::Vector<SpriteFrame*> idle2;		//���ƶ�
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
	//cocos2d::Label* time;
	//int dtime;
	cocos2d::ProgressTimer* pT1;				//���1Ѫ��
	cocos2d::ProgressTimer* pT2;				//���2Ѫ��

	//��ҷ���
	int direction1 = 1, direction2 = 3;
	
	//�Ƿ��ƶ�
	bool isMove1 = false, isMove2 = false;
	char moveKey1, moveKey2;

	//�ȷ�
	int score;
	cocos2d::Label* number;

	//�������ʼ״̬
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

	//����
	cocos2d::Sprite* football;
};
