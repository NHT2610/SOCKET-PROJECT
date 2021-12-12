#pragma once
#include "framework.h"

//Khai báo socket cho client


class ClientSocket
{
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
};

extern ClientSocket client_socket;