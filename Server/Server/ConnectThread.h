#pragma once
#include <afxsock.h>
#include <vector>
#include <string>
using namespace std;

class ConnectThread
{
public:
	vector<string> split_string(string s, char ch);
	void operator()(CSocket& client, int NumberCode);
};

