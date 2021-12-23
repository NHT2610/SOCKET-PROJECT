#pragma once
#include "framework.h"
#include <string>
#include <cstring>
#include <vector>
#include <sstream>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <locale>
#include <codecvt>

using namespace std;

//Khai báo socket cho client


class ClientSocket
{
public:
	struct GoldInformations {
		wstring LoaiVang;
		wstring GiaMua;
		wstring GiaBan;
		wstring Ngay;
	};
private:
	CSocket soc;
public:
	ClientSocket();
	~ClientSocket();
	bool create();
	void close();
	bool connect(const CString ip, const int port);
	void SendMessage(const CString mes);
	int login(const CString user, const CString pass, const CString function);
	int Register(const CString user, const CString pass, const CString function);
	GoldInformations ParseData(wstring s);
	wstring ParseDay(wstring Day);
	vector<GoldInformations> GetDataFromServer(string message);
};

extern ClientSocket client_socket;