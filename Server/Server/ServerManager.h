#pragma once
#include <afxsock.h>
#include <vector>
#include <string>
#include <fstream>
#include "ServerDlg.h"

class CServerDlg;
using namespace std;

class ServerManager
{
public:
	struct Account {
		string username;
		string password;
	};
private:
	CSocket Server;
	CSocket Connector;
	vector<Account> AccountData;
	fstream Obj;
	vector<string> split_string(string s, char ch);
	void GetAccountDataFromFile(const char* FileName);
	int CheckUsernameAndPasswordLOGIN(string& user, string& pass);
	bool CheckUsernameAndPasswordREGISTER(string& user);
	void WriteAccountDataToFile(const char* FileName);
public:
	ServerManager(CServerDlg* dialog);
	~ServerManager();

	bool ServerCreate(int PORT);
	void ServerClose();
	void ConnectorClose();
	bool ServerListen();
	bool ServerAccept();
	void ServerSend(string mess);
	void MainProcess();
	CServerDlg* DlgTextBox;
};

