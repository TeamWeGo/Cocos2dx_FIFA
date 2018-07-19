#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "bomb.h"

#pragma execution_character_set("utf-8")

#define database UserDefault::getInstance()

USING_NS_CC;

using namespace CocosDenshion;

void GameScene::setPhysicsWorld(PhysicsWorld* world) { m_world = world; }

Scene* GameScene::createScene()
{
	//return GameScene::create();
	srand((unsigned)time(NULL));
	auto scene = Scene::createWithPhysics();

	scene->getPhysicsWorld()->setAutoStep(true);

	// Debug 模式
	// scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2(0, -300.0f));
	auto layer = GameScene::create();
	layer->setPhysicsWorld(scene->getPhysicsWorld());
	scene->addChild(layer);
	return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	auto edgeSp = Sprite::create();  //创建一个精灵
	auto boundBody = PhysicsBody::createEdgeBox(visibleSize * 0.96f, PhysicsMaterial(0.0f, 1.0f, 0.0f), 3);  //edgebox是不受刚体碰撞影响的一种刚体，我们用它来设置物理世界的边界
	edgeSp->setPosition(visibleSize.width / 2, visibleSize.height / 2);  //位置设置在屏幕中央
	edgeSp->setPhysicsBody(boundBody);
	addChild(edgeSp);

	preloadMusic();
	addSprite();
	addListener();
	
	schedule(schedule_selector(GameScene::update), 0.07f, kRepeatForever, 0);
	schedule(schedule_selector(GameScene::updateBomb), 10.0f, kRepeatForever, 0);
	return true;
}

void GameScene::preloadMusic()
{
	auto sae = SimpleAudioEngine::getInstance();
	sae->preloadEffect("gameover.mp3");
	sae->preloadBackgroundMusic("bgm.mp3");
	//sae->playBackgroundMusic("bgm.mp3", true);
}

void GameScene::addSprite(){

	auto texture = Director::getInstance()->getTextureCache()->addImage("player1.png");
	auto texture2 = Director::getInstance()->getTextureCache()->addImage("player2.png");
	//玩家各方向的初始状态
	frame01_right = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 200, 100, 100)));
	frame01_left = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 100, 100, 100)));
	frame01_up = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 300, 100, 100)));
	frame01_down = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 100, 100)));

	frame02_right = SpriteFrame::createWithTexture(texture2, CC_RECT_PIXELS_TO_POINTS(Rect(0, 200, 100, 100)));
	frame02_left = SpriteFrame::createWithTexture(texture2, CC_RECT_PIXELS_TO_POINTS(Rect(0, 100, 100, 100)));
	frame02_up = SpriteFrame::createWithTexture(texture2, CC_RECT_PIXELS_TO_POINTS(Rect(0, 300, 100, 100)));
	frame02_down = SpriteFrame::createWithTexture(texture2, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 100, 100)));
	
	//添加足球
	football = Sprite::create("ball.png");
	football->setPosition(visibleSize / 2);
	football->setScale(0.2f, 0.2f);

	//添加足球刚体
	auto ballbody = PhysicsBody::createCircle(football->getContentSize().width/2, PhysicsMaterial(1.0f, 0.0f, 0.0f));
	ballbody->setDynamic(true);
	ballbody->setRotationEnable(false);
	ballbody->setGravityEnable(false);
	ballbody->setLinearDamping(0.5f);
	football->setPhysicsBody(ballbody);
	football->setName("football");

	addChild(football, 3);
	
	//添加玩家1
	player1 = Sprite::createWithSpriteFrame(frame01_right);
	player1->setPosition(Vec2(origin.x + visibleSize.width / 2 - 100,
		origin.y + visibleSize.height / 2));
	player1->setScale(2.0f, 2.0f);

	//添加玩家1刚体
	auto playerbody = PhysicsBody::createBox(player1->getContentSize()*0.6f, PhysicsMaterial(1000.0f, 0.0f, 50.0f));
	playerbody->setDynamic(true);
	playerbody->setRotationEnable(false);
	playerbody->setGravityEnable(false);
	playerbody->setLinearDamping(0.0f);
	player1->setPhysicsBody(playerbody);
	player1->setName("player");

	addChild(player1, 3);


	//添加玩家2
	player2 = Sprite::createWithSpriteFrame(frame02_left);
	player2->setPosition(Vec2(origin.x + visibleSize.width / 2 + 100,
		origin.y + visibleSize.height / 2));
	player2->setScale(2.0f, 2.0f);

	//添加玩家2刚体
	playerbody = PhysicsBody::createBox(player2->getContentSize() * 0.8f, PhysicsMaterial(1000.0f, 0.0f, 50.0f));
	playerbody->setDynamic(true);
	playerbody->setRotationEnable(false);
	playerbody->setGravityEnable(false);
	playerbody->setLinearDamping(0.0f);
	player2->setPhysicsBody(playerbody);
	player2->setName("player");

	addChild(player2, 3);

	//血条
	Sprite* sp1 = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 320, 420, 47)));
	Sprite* sp2 = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 320, 420, 47)));

	pT1 = ProgressTimer::create(sp1);
	pT1->setScaleX(90);
	pT1->setAnchorPoint(Vec2(0, 0));
	pT1->setType(ProgressTimerType::BAR);
	pT1->setBarChangeRate(Point(1, 0));
	pT1->setMidpoint(Point(0, 1));
	pT1->setPercentage(100);
	pT1->setPosition(Vec2(origin.x + 14 * pT1->getContentSize().width, origin.y + visibleSize.height - 2 * pT1->getContentSize().height));
	addChild(pT1, 2);
	sp1->setAnchorPoint(Vec2(0, 0));
	sp1->setPosition(Vec2(origin.x + pT1->getContentSize().width, origin.y + visibleSize.height - sp1->getContentSize().height));
	addChild(sp1, 1);

	pT2 = ProgressTimer::create(sp2);
	pT2->setScaleX(90);
	pT2->setAnchorPoint(Vec2(0, 0));
	pT2->setType(ProgressTimerType::BAR);
	pT2->setBarChangeRate(Point(1, 0));
	pT2->setMidpoint(Point(0, 1));
	pT2->setPercentage(100);
	pT2->setPosition(Vec2(origin.x + 14 * pT2->getContentSize().width + 500, origin.y + visibleSize.height - 2 * pT2->getContentSize().height));
	addChild(pT2, 2);
	sp2->setAnchorPoint(Vec2(0, 0));
	sp2->setPosition(Vec2(origin.x + pT2->getContentSize().width + 500, origin.y + visibleSize.height - sp2->getContentSize().height));
	addChild(sp2, 1);


	run_right1.reserve(5);
	for (int i = 0; i < 4; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(i * 100, 200, 100, 100)));
		run_right1.pushBack(frame);
	}
	run_right1.pushBack(frame01_right);

	run_left1.reserve(5);
	for (int i = 0; i < 4; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(i * 100, 100, 100, 100)));
		run_left1.pushBack(frame);
	}
	run_left1.pushBack(frame01_left);

	run_up1.reserve(5);
	for (int i = 0; i < 4; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(i * 100, 300, 100, 100)));
		run_up1.pushBack(frame);
	}
	run_up1.pushBack(frame01_up);

	run_down1.reserve(5);
	for (int i = 0; i < 4; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(i * 100, 0, 100, 100)));
		run_down1.pushBack(frame);
	}
	run_down1.pushBack(frame01_down);

	run_right2.reserve(5);
	for (int i = 0; i < 4; i++) {
		auto frame = SpriteFrame::createWithTexture(texture2, CC_RECT_PIXELS_TO_POINTS(Rect(i * 100, 200, 100, 100)));
		run_right2.pushBack(frame);
	}
	run_right2.pushBack(frame02_right);

	run_left2.reserve(5);
	for (int i = 0; i < 4; i++) {
		auto frame = SpriteFrame::createWithTexture(texture2, CC_RECT_PIXELS_TO_POINTS(Rect(i * 100, 100, 100, 100)));
		run_left2.pushBack(frame);
	}
	run_left2.pushBack(frame02_left);

	run_up1.reserve(5);
	for (int i = 0; i < 4; i++) {
		auto frame = SpriteFrame::createWithTexture(texture2, CC_RECT_PIXELS_TO_POINTS(Rect(i * 100, 300, 100, 100)));
		run_up2.pushBack(frame);
	}
	run_up2.pushBack(frame02_up);

	run_down2.reserve(5);
	for (int i = 0; i < 4; i++) {
		auto frame = SpriteFrame::createWithTexture(texture2, CC_RECT_PIXELS_TO_POINTS(Rect(i * 100, 0, 100, 100)));
		run_down2.pushBack(frame);
	}
	run_down2.pushBack(frame02_down);

	TMXTiledMap* tmx = TMXTiledMap::create("map.tmx");
	tmx->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	tmx->setAnchorPoint(Vec2(0.5, 0.5));
	tmx->setScale(visibleSize.height / tmx->getContentSize().height);
	this->addChild(tmx, 0);
}

void GameScene::addListener()
{

	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onConcactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

void GameScene::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {

	int distance;

	switch (code) {
	case EventKeyboard::KeyCode::KEY_CAPITAL_A:
	case EventKeyboard::KeyCode::KEY_A:
		moveKey1 = 'A';
		keystatep1.push_front('A');
		break;
	case EventKeyboard::KeyCode::KEY_CAPITAL_D:
	case EventKeyboard::KeyCode::KEY_D:
		moveKey1 = 'D';
		keystatep1.push_front('D');
		break;
	case EventKeyboard::KeyCode::KEY_CAPITAL_S:
	case EventKeyboard::KeyCode::KEY_S:
		moveKey1 = 'S';
		keystatep1.push_front('S');
		break;
	case EventKeyboard::KeyCode::KEY_CAPITAL_W:
	case EventKeyboard::KeyCode::KEY_W:
		moveKey1 = 'W';
		keystatep1.push_front('W');
		break;


	case EventKeyboard::KeyCode::KEY_CAPITAL_J:
	case EventKeyboard::KeyCode::KEY_J:
		moveKey2 = 'J';
		keystatep2.push_front('J');
		break;
	case EventKeyboard::KeyCode::KEY_CAPITAL_I:
	case EventKeyboard::KeyCode::KEY_I:
		moveKey2 = 'I';
		keystatep2.push_front('I');
		break;
	case EventKeyboard::KeyCode::KEY_CAPITAL_K:
	case EventKeyboard::KeyCode::KEY_K:
		moveKey2 = 'K';
		keystatep2.push_front('K');
		break;
	case EventKeyboard::KeyCode::KEY_CAPITAL_L:
	case EventKeyboard::KeyCode::KEY_L:
		moveKey2 = 'L';
		keystatep2.push_front('L');
		break;

	default:
		break;
	}
	moveCharacter1(moveKey1);
	moveCharacter2(moveKey2);
}

// 释放按键
void GameScene::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
	char key = 'N';
	char * moveKey = NULL;
	list<char>*keystate = NULL;
	switch (code) {
	case EventKeyboard::KeyCode::KEY_CAPITAL_A:
	case EventKeyboard::KeyCode::KEY_A:
		key = 'A';
		break;
	case EventKeyboard::KeyCode::KEY_CAPITAL_D:
	case EventKeyboard::KeyCode::KEY_D:
		key = 'D';
		break;
	case EventKeyboard::KeyCode::KEY_CAPITAL_S:
	case EventKeyboard::KeyCode::KEY_S:
		key = 'S';
		break;
	case EventKeyboard::KeyCode::KEY_CAPITAL_W:
	case EventKeyboard::KeyCode::KEY_W:
		key = 'W';
		break;

	case EventKeyboard::KeyCode::KEY_CAPITAL_I:
	case EventKeyboard::KeyCode::KEY_I:
		key = 'I';
		break;
	case EventKeyboard::KeyCode::KEY_CAPITAL_K:
	case EventKeyboard::KeyCode::KEY_K:
		key = 'K';
		break;
	case EventKeyboard::KeyCode::KEY_CAPITAL_J:
	case EventKeyboard::KeyCode::KEY_J:
		key = 'J';
		break;
	case EventKeyboard::KeyCode::KEY_CAPITAL_L:
	case EventKeyboard::KeyCode::KEY_L:
		key = 'L';
		break;

	default:
		break;
	}
	if (key == 'N')
	{
		return;
	}
	else if (key == 'A' || key == 'S' || key == 'D' || key == 'W')
	{
		keystate = &keystatep1;
		moveKey = &moveKey1;
	}
	else
	{
		keystate = &keystatep2;
		moveKey = &moveKey2;
	}
	for (auto it1 = keystate->begin(); it1 != keystate->end(); it1++)
	{
		if (*it1 == key)
		{
			it1 = keystate->erase(it1);
			break;
		}
	}
	if (!keystate->empty())
	{
		*moveKey = keystate->front();
	}
	else
	{
		*moveKey = 'N';
	}
	moveCharacter1(moveKey1);
	moveCharacter2(moveKey2);
}

bool GameScene::onConcactBegin(PhysicsContact & contact) {
	auto c1 = contact.getShapeA(), c2 = contact.getShapeB();
	auto body1 = c1->getBody(), body2 = c2->getBody();
	auto sp1 = (Sprite*)body1->getNode(), sp2 = (Sprite*)body2->getNode();

	if (sp1 && sp2) {

		if (sp1->getName() == sp2->getName())
			return true;

		else if ((sp1->getName() == "player" && sp2->getName() == "football") || (sp1->getName() == "football" && sp2->getName() == "player")) {
			
		}
	}

	return true;
}

void GameScene::moveCharacter1(char c) {
	auto animate1 = Animate::create(Animation::createWithSpriteFrames(run_up1, 0.1f, 1));
	auto animate2 = Animate::create(Animation::createWithSpriteFrames(run_down1, 0.1f, 1));
	auto animate3 = Animate::create(Animation::createWithSpriteFrames(run_left1, 0.1f, 1));
	auto animate4 = Animate::create(Animation::createWithSpriteFrames(run_right1, 0.1f, 1));
	auto speed = 250;
	switch (c) {
	case 'W':
		if (player1->getPosition().y < visibleSize.height)
		{
			direction1 = 2;
			auto move = CCRepeatForever::create(animate1);
			player1->stopAllActions();
			player1->runAction(move);
			player1->getPhysicsBody()->setVelocity(Vec2(0, speed));
		}
		else
		{
			player1->getPhysicsBody()->setVelocity(Vec2(0, 0));
		}
		break;
	case 'S':
		if (player1->getPosition().y > 0)
		{
			direction1 = 4;
			auto move = CCRepeatForever::create(animate2);
			player1->stopAllActions();
			player1->runAction(move);
			player1->getPhysicsBody()->setVelocity(Vec2(0, -speed));
		}
		else
		{
			player1->getPhysicsBody()->setVelocity(Vec2(0, 0));
		}
		break;
	case 'A':
		if (player1->getPosition().x > 0)
		{
			direction1 = 3;
			auto move = CCRepeatForever::create(animate3);
			player1->stopAllActions();
			player1->runAction(move);
			player1->getPhysicsBody()->setVelocity(Vec2(-speed, 0));
		}
		else
		{
			player1->getPhysicsBody()->setVelocity(Vec2(0, 0));
		}
		break;
	case 'D':
		if (player1->getPosition().x < visibleSize.width)
		{
			direction1 = 1;
			auto move = CCRepeatForever::create(animate4);
			player1->stopAllActions();
			player1->runAction(move);
			player1->getPhysicsBody()->setVelocity(Vec2(speed, 0));
		}
		else
		{
			player1->getPhysicsBody()->setVelocity(Vec2(0, 0));
		}
		break;
	case 'N':
		player1->stopAllActions();
		player1->getPhysicsBody()->setVelocity(Vec2(0, 0));
		break;
	}
}

void GameScene::moveCharacter2(char c) {
	auto animate1 = Animate::create(Animation::createWithSpriteFrames(run_up2, 0.1f, 1));
	auto animate2 = Animate::create(Animation::createWithSpriteFrames(run_down2, 0.1f, 1));
	auto animate3 = Animate::create(Animation::createWithSpriteFrames(run_left2, 0.1f, 1));
	auto animate4 = Animate::create(Animation::createWithSpriteFrames(run_right2, 0.1f, 1));
	auto speed = 250;
	switch (c) {
	case 'I':
		if (player2->getPosition().y < visibleSize.height)
		{
			direction1 = 2;
			auto move = CCRepeatForever::create(animate1);
			player2->stopAllActions();
			player2->runAction(move);
			player2->getPhysicsBody()->setVelocity(Vec2(0, speed));
		}
		else
		{
			player2->getPhysicsBody()->setVelocity(Vec2(0, 0));
		}
		break;
	case 'K':
		if (player2->getPosition().y < visibleSize.height)
		{
			direction1 = 4;
			auto move = CCRepeatForever::create(animate2);
			player2->stopAllActions();
			player2->runAction(move);
			player2->getPhysicsBody()->setVelocity(Vec2(0, -speed));
		}
		else
		{
			player2->getPhysicsBody()->setVelocity(Vec2(0, 0));
		}
		break;
	case 'J':
		if (player2->getPosition().y < visibleSize.height)
		{
			direction1 = 3;
			auto move = CCRepeatForever::create(animate3);
			player2->stopAllActions();
			player2->runAction(move);
			player2->getPhysicsBody()->setVelocity(Vec2(-speed, 0));
		}
		else
		{
			player2->getPhysicsBody()->setVelocity(Vec2(0, 0));
		}
		break;
	case 'L':
		if (player2->getPosition().y < visibleSize.height)
		{
			direction1 = 1;
			auto move = CCRepeatForever::create(animate4);
			player2->stopAllActions();
			player2->runAction(move);
			player2->getPhysicsBody()->setVelocity(Vec2(speed, 0));
		}
		else
		{
			player2->getPhysicsBody()->setVelocity(Vec2(0, 0));
		}
		break;
	case 'N':
		player2->stopAllActions();
		player2->getPhysicsBody()->setVelocity(Vec2(0, 0));
		break;
	}
}



void GameScene::update(float) {
	TTFConfig ttfConfig;
	ttfConfig.fontFilePath = "fonts/arial.ttf";
	ttfConfig.fontSize = 36;
	if (score == 0)
		number = Label::createWithTTF(ttfConfig, "0:0");
	else if (score == 1)
		number = Label::createWithTTF(ttfConfig, "1:0");
	else if (score == -1)
		number = Label::createWithTTF(ttfConfig, "0:1");
	else if (score == 2)
		number = Label::createWithTTF(ttfConfig, "Player1 Win!");
	else
		number = Label::createWithTTF(ttfConfig, "Player2 Win!");
	number->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - number->getContentSize().height));
	this->addChild(number, 1);
}

void GameScene::createBomb() {
	auto mybomb = Factory::getInstance()->createBomb();
	Factory::getInstance()->initSpriteFrame();
	for (int i = 0; i < visibleSize.height; i+= mybomb->getContentSize().height*0.2)
	{
		for (int j = 0; j < visibleSize.width; j+= mybomb->getContentSize().width*0.2)
		{
			auto lucky = random(0, 99);
			if (lucky % 25 == 0)
			{
				auto bomb = Factory::getInstance()->createBomb();
				bomb->setScale(0.2f, 0.2f);
				auto bombbody = PhysicsBody::createCircle(bomb->getContentSize().width/2, PhysicsMaterial(1.0f, 1.0f, 1.0f));
				bombbody->setDynamic(false);
				bombbody->setRotationEnable(false);
				bombbody->setGravityEnable(false);
				bombbody->setCategoryBitmask(0xFFFFFFFF);
				bombbody->setCollisionBitmask(0xFFFFFFFF);
				bombbody->setContactTestBitmask(0xFFFFFFFF);
				bomb->setPhysicsBody(bombbody);

				bomb->setPosition(j, visibleSize.height);
				GameScene::addChild(bomb, 1);
				auto seq = Sequence::create(MoveTo::create(0.5f, Vec2(j, i)), CCDelayTime::create(2), CallFunc::create([=] {
					Factory::getInstance()->exploration();
				}), NULL);
				bomb->runAction(seq);
			}
		}
	}
}

void GameScene::updateBomb(float)
{
	createBomb();
}

void GameScene::explorate(float)
{
	Factory::getInstance()->exploration();
}