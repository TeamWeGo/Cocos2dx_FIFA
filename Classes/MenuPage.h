#ifndef __MENUPAGE_H__
#define __MENUPAGE_H__
#include "cocos2d.h"
#include "ui\CocosGUI.h"
#include "SqliteManager.h"
using namespace cocos2d::ui;

class MenuPage : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
	void loginButtonCallback(Ref *pSender);
	void registerButtonCallback(Ref *pSender);
	void rankButtonCallback(Ref *pSender);
    CREATE_FUNC(MenuPage);
	
private:
	TextField * usernameInput;
	TextField * passwordInput;
	SqliteManager* sqlite;
	Label * message;
	Label * rank;
	Label * login;
};

#endif // __MENUPAGE_H__
