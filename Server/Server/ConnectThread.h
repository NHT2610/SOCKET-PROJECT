#pragma once
#include <afxsock.h>
#include <vector>
#include <string>
#include <winsqlite/winsqlite3.h>
#include <sql.h>
using namespace std;

class ConnectThread
{
public:
	void operator()(CSocket& client, int NumberCode);
};

