#pragma once
#include <afxsock.h>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>
#include "ServerDlg.h"
#include <thread>
#include <sstream>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <locale>
#include <codecvt>
#pragma warning(disable : 4996)

class CServerDlg;
using namespace std;

class ServerManager
{
public:
	struct GoldInformations {
		wstring Date;
		wstring LoaiVang;
		wstring GiaMua;
		wstring GiaBan;
	};
	struct Account {
		string username;
		string password;
	};
private:
	CSocket Connector;
	vector<Account> AccountData;
	fstream Obj;
	vector<string> split_string(string s, char ch);
	void GetAccountDataFromFile(const char* FileName);
	int CheckUsernameAndPasswordLOGIN(string& user, string& pass);
	bool CheckUsernameAndPasswordREGISTER(string& user);
	void WriteAccountDataToFile(const char* FileName);
	//Vector chứa dữ liệu giá vàng
	vector<GoldInformations> GoldData;
public:
	ServerManager(CServerDlg* dialog);
	~ServerManager();

	int CountThread;
	SOCKET* hConnected;
	/*bool ServerCreate(int PORT);
	void ServerClose();
	void ConnectorClose();
	bool ServerListen();
	bool ServerAccept();*/
	void ServerSend(string mess);
	void MainProcess();
	CServerDlg* DlgTextBox;

	//Các hàm xử lý dịch vụ xem giá vàng
	void CurrentDay(int& day, int& month, int& year);
	wstring ReadDataFile(const char* FileName);
	vector<GoldInformations> GetData(wstring s, wchar_t ch);
	vector<GoldInformations> GetDataByTypeGold(wstring s, wstring Type);
	void SendGoldData();
};

DWORD WINAPI ThreadProccess(LPVOID lpParameter);