# include "SqliteManager.h"
#include <iostream>

static SqliteManager *_sqlite = nullptr;

SqliteManager::SqliteManager(){
	username1 = "";
	username2 = "";
}
SqliteManager::~SqliteManager(){
	if (re != NULL) {
		sqlite3_free_table(re);
		sqlite3_close(pdb);
	}
}

bool SqliteManager::createDatabase(void){
    pdb = NULL;
    string path = FileUtils::getInstance()->getWritablePath()+"demo.db";
	result = sqlite3_open(path.c_str(),&pdb);
	if (result == SQLITE_OK) {
		;
	}
	else {
		;
	}
	this->createTable();
	return true;
}

bool SqliteManager::createTable(void)
{
	result = sqlite3_exec(pdb, "create table game(username char(16),password char(16),health integer,primary key(username))", NULL, NULL, NULL);
	if (result == SQLITE_OK) {
		return true;
	}
	else {
		return false;
	}
}

bool SqliteManager::InsertData(const char* username ,const char* password, int health)
{
	string s1("insert into game values(");
	string s3(",");
	string s4 = std::to_string(health);
	string s5(")");
	sql = s1 + "\""+username+"\""+s3+"\""+password+"\""+s3+ s4 + s5;
	result = sqlite3_exec(pdb,sql.c_str(), NULL, NULL, NULL);
	if (result == SQLITE_OK) {
		return true;
	}
	else {
		return false;
	}
}

bool SqliteManager::login(const char * username, const char * password)
{
	int r = 0, c = 0;
	string s1("select password from game where username=");
	sql = s1+"\'"+username+"\'";
	result = sqlite3_get_table(pdb, sql.c_str(), &re, &r, &c, NULL);
	if (result == SQLITE_OK) {
		if (r != 0 && c != 0) {
			string pa = re[1];
			string pass = password;
			if (pa == pass) {
				if (username1 == "") {
					username1 = username;
				}
				else {
					if (username1 != username) {
						username2 = username;
					}
					else {
						errorMessage = "alread login";
						return false;
					}
				}
				return true;
			}
			else {
				errorMessage = "not register or password was error";
				return false;
			}
		}
		else {
			errorMessage = "not register or password was error";
			return false;
		}
	}
	else {
		errorMessage = "database error";
		return false;
	}
	return false;
}

bool SqliteManager::UpdateData(const char* username, int health)
{
	string s1("update game set health=");
	string s2;
	s2 = std::to_string(health);
	string s3(" where username=");
	sql = s1  + s2 +s3+ "\""+username+"\"";
	result = sqlite3_exec(pdb, sql.c_str(), NULL, NULL, NULL);
	if (result == SQLITE_OK) {
		return true;
	}
	else {
		return false;
	}
}

bool SqliteManager::DeleteData(const char* username)
{
	string s1("detele from game where username=");
	sql = s1 + username;
	result = sqlite3_exec(pdb, sql.c_str(), NULL, NULL, NULL);
	if (result == SQLITE_OK) {
		return true;
	}
	else {
		return false;
	}
}

string SqliteManager::getUsername1()
{
	return username1;
}
string SqliteManager::getUsername2()
{
	return username2;
}

string SqliteManager::getErrorMessage()
{
	return errorMessage;
}

string SqliteManager::getTop5()
{
	int r = 0, c = 0;
	string s1("select username, health from game order by health DESC limit 5 ");
	sql = s1;
	string tem ;
	result = sqlite3_get_table(pdb, sql.c_str(), &re, &r, &c, NULL);
	if (result == SQLITE_OK) {
		if (r != 0 && c != 0) {
			for (int i = 0; i < r*c+2; i++) {
				if (i % 2 == 0) {
					tem = tem + re[i]+" ";
				}
				else {
					tem = tem + re[i]+"\n";
				}
			}
		}
		return tem;
	}
	else {
		return "";
	}
}

SqliteManager * SqliteManager::getInstance()
{
	if ( _sqlite == NULL) {
		_sqlite = new SqliteManager();
	}
	return _sqlite;
}


void SqliteManager::closeDatabase()
{
	sqlite3_free_table(re);
	sqlite3_close(pdb);
}
