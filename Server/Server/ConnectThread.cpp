#include "pch.h"
#include "ConnectThread.h"

//Hàm xử lý tách chuỗi mà client gửi đến
vector<string> ConnectThread::split_string(string s, char ch) {
	vector<string> tokens;
	string field;
	int len = int(s.length());
	int j;
	for (int i = 0; i < len; ++i) {
		for (j = i; j < len && s[j] != '|'; ++j) {
			field += s[j];
		}
		tokens.push_back(field);
		field = "";
		i = j;
	}
	return tokens;
}

//Hàm xử lý luồng
void ConnectThread::operator()(CSocket& client, int NumberCode)
{
	int len = 0;
	while (true) {
		//Nhận thông điệp yêu cầu từ client
		client.Receive(&len, sizeof(int), 0);
		char* temp1 = new char[len + 1]; temp1[len] = '\0';
		client.Receive(temp1, len, 0);

		//Yêu cầu đăng nhập từ client
		if (strcmp(temp1, "LOGIN") == 0) {
			delete[] temp1;

			//Nhận thông điệp chuỗi tin nhắn gồm username và password 
			client.Receive(&len, sizeof(int), 0);
			char* temp2 = new char[len + 1];
			client.Receive(temp2, len, 0); temp2[len] = '\0';
			
		}
	}
}
