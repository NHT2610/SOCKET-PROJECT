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
};

extern ClientSocket client_socket;