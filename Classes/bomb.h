#pragma once
#include "cocos2d.h"
using namespace cocos2d;

class Factory :public cocos2d::Ref {
public:
	//��ȡ��������
	static Factory* getInstance();
	//����ը�������洢�������й���
	Sprite * createBomb();
	//ը����ʱ��ը
	void exploration();
	//����Զ�����ĳը���Ƴ�ը��
	void removeBomb(Sprite*);
	//�ж���ײ
	Sprite* collider(Rect rect);
	//��ʼ����ը֡����
	void initSpriteFrame();

private:
	Factory();
	Vector<Sprite*> bomb;
	cocos2d::Vector<SpriteFrame*> explore;
	static Factory* factory;
	int bomb_count;
};