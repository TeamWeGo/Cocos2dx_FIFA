#include "MenuPage.h"
#include "SimpleAudioEngine.h"
#include "GameScene.h"

USING_NS_CC;


Scene* MenuPage::createScene()
{
    return MenuPage::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MenuPage::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto menuItem = MenuItemFont::create("Begin", CC_CALLBACK_1(MenuPage::menuCloseCallback, this));
    auto menu = Menu::create(menuItem, NULL);
    menu->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
    this->addChild(menu, 1);



    auto label = Label::createWithTTF("Game", "fonts/Marker Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        label->setPosition(Vec2(origin.x + visibleSize.width/3,
                                origin.y + visibleSize.height - label->getContentSize().height));
        this->addChild(label, 1);
    }

	auto loginButton = MenuItemFont::create("Login", CC_CALLBACK_1(MenuPage::loginButtonCallback, this));
	if (loginButton) {
		float x = origin.x + visibleSize.width / 3;
		float y = origin.y + loginButton->getContentSize().height / 2+100;
		loginButton->setPosition(Vec2(x, y));
	}

	auto registerButton = MenuItemFont::create("Register", CC_CALLBACK_1(MenuPage::registerButtonCallback, this));
	if (registerButton) {
		float x = origin.x + visibleSize.width / 3;
		float y = origin.y + registerButton->getContentSize().height / 2 + 200;
		registerButton->setPosition(Vec2(x, y));
	}

	auto rankButton = MenuItemFont::create("rank", CC_CALLBACK_1(MenuPage::rankButtonCallback, this));
	if (rankButton) {
		float x = 2 * visibleSize.width / 3;
		float y = visibleSize.height / 2 + 200;
		rankButton->setPosition(Vec2(x, y));
	}

	Menu* menu1 = Menu::create(loginButton, registerButton,rankButton,  NULL);
	menu1->setPosition(Vec2::ZERO);
	this->addChild(menu1, 1);

	usernameInput = TextField::create("username", "arial", 24);
	if (usernameInput) {
		float x = origin.x + visibleSize.width / 3;
		float y = origin.y + visibleSize.height - 130.0f;
		usernameInput->setPosition(Vec2(x, y));
		this->addChild(usernameInput, 1);
	}

	passwordInput = TextField::create("password", "arial", 24);
	if (passwordInput) {
		float x = origin.x + visibleSize.width / 3;
		float y = origin.y + visibleSize.height - 160.0f;
		passwordInput->setPosition(Vec2(x, y));
		this->addChild(passwordInput, 1);
	}

	 message = Label::create("", "arial", 30);
	if (message) {
		float x = origin.x + visibleSize.width / 3;
		float y = origin.y + visibleSize.height - 220.0f;
		message->setPosition(Vec2(x, y));
		this->addChild(message, 1);
	}

	rank = Label::create("", "arial", 30);
	if (rank) {
		float x = 2*visibleSize.width / 3;
		float y = visibleSize.height / 2+100;
		rank->setPosition(Vec2(x, y));
		addChild(rank, 1);
	}
	
	 login = Label::create("", "arial", 30);
	 if (login) {
		 float x = 2 * visibleSize.width / 3;
		 float y = visibleSize.height - 130.0f;
		 login->setPosition(Vec2(x, y));
		 addChild(login, 1);
	 }

	sqlite = SqliteManager::getInstance();
	if (sqlite) {
		sqlite->createDatabase();
		sqlite->createTable();
	}

    return true;
}


void MenuPage::menuCloseCallback(Ref* pSender)
{
	if (sqlite->getUsername1() != ""&& sqlite->getUsername2() != "")
	{
		usernameInput->setString("");
		passwordInput->setString("");
		login->setString("");
		auto scene = GameScene::createScene();
		Director::getInstance()->replaceScene(scene);
	}else {
		string k = "you should login two players";
		message->setString(k);
	}
 
}

void MenuPage::loginButtonCallback(Ref * pSender)
{
	message->setString("");
	const char* username = usernameInput->getString().c_str();
	const char*  password = passwordInput->getString().c_str();
	if (username[0] == '\0' || password[0] == '\0')
	{
		string me = "username or password is empty";
		message->setString(me);
		return;
	}if (true) {
		if (sqlite->login(username,password)) {
			message->setString("");
			string k = username;
			string me = k + ":"+"login";
			message->setString(me);
			login->setString(login->getString() + "\n" + "player :"+username);
			usernameInput->setString("");
			passwordInput->setString("");
		}
		else {
			message->setString(sqlite->getErrorMessage());
		}
	}
	passwordInput->setString("");
}

void MenuPage::registerButtonCallback(Ref * pSender)
{
	message->setString("");
	const char* username = usernameInput->getString().c_str();
	const char*  password = passwordInput->getString().c_str();
	if (username[0]=='\0' || password[0]=='\0' ) 
	{
		string me = "username or password is empty";
		message->setString(me);
		return ;
	}if (true) {
		if (sqlite->InsertData(username, password, 0)) {
			message->setString("");
			string k = username ;
			string me = k + ":"+"register ok ";
			message->setString(me);
		}
		else {
			message->setString("");
			string me = "already register";
			message->setString(me);
		}
	}

}

void MenuPage::rankButtonCallback(Ref * pSender)
{
	rank->setString("");
	string top  =  sqlite->getTop5();
	rank->setString(top);
}
