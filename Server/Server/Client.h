#pragma once
#include "framework.h"
#include "Resource.h"
#include <afxsock.h>

class Client
{
private:
	CSocket client_socket;
	char username[100];
	char password[100];
public:
	Client() {}
	~Client() {}
	
};

