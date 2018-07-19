#pragma once
# include "sqlite3.h"
# include "cocos2d.h"
using namespace std;
using namespace cocos2d;
class SqliteManager{
public :
    SqliteManager();
    ~SqliteManager();
    bool createDatabase();
    bool createTable();
    bool InsertData(const char* username,const char* password, int health);
	bool login(const char*username, const char* password);
    bool UpdateData(const char* username, int health);
    bool DeleteData(const char* username);
    string getUsername1();
	string getUsername2();
	string getErrorMessage();
	string getTop5();
	static SqliteManager* getInstance();
    void closeDatabase();
private:
	sqlite3 * pdb;
	string sql;
	int result;
	char **re;
	string username1;
	string username2;
	string errorMessage;
};