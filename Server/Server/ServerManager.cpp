#include "pch.h"
#include "ServerManager.h"

ServerManager::ServerManager(CServerDlg* dialog) {
	DlgTextBox = dialog;
	GetAccountDataFromFile("ACCOUNT.txt");
}

ServerManager::~ServerManager() {
	Connector.~CSocket();
}

bool ServerManager::ServerCreate(int PORT) {
	if (Server.Create(PORT, SOCK_STREAM, NULL)) { return TRUE; }
	return FALSE;
}

void ServerManager::ServerClose() {
	Server.Close();
}

void ServerManager::ConnectorClose() {
	Connector.Close();
}

bool ServerManager::ServerListen() {
	if (Server.Listen()) { return TRUE; }
	return FALSE;
}

bool ServerManager::ServerAccept() {
	if (Server.Accept(Connector)) { return TRUE; }
	return FALSE;
}

void ServerManager::ServerSend(string mess) {
	int len = int(mess.length());
	Connector.Send(&len, sizeof(int), 0);
	Connector.Send(mess.c_str(), len, 0);
}

//Tách chuỗi tin nhắn client gửi đến
vector<string> ServerManager::split_string(string s, char ch) {
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

//Load dữ liệu tài khoản (username, password) từ file ACCOUNT.txt
void ServerManager::GetAccountDataFromFile(const char* FileName) {
	Obj.open(FileName, ios::in);
	string line;
	vector<string> temp(2);
	while (!Obj.eof()) {
		getline(Obj, line);
		if (line != "") {
			temp = split_string(line, '|');
			AccountData.push_back({ temp[0],temp[1] });
		}
		line = "";
	}
	Obj.close();
}

//Ghi dữ liệu xuống file ACCOUNT.txt
void ServerManager::WriteAccountDataToFile(const char* FileName) {
	Obj.open(FileName, ios::out | ios::app);
	Obj << endl;
	int size = int(AccountData.size());
	string NewAcc = AccountData[size - 1].username + '|' + AccountData[size - 1].password;
	Obj << NewAcc;
	Obj.close();
}

//Kiểm tra thông tin đăng nhập từ client
int ServerManager::CheckUsernameAndPasswordLOGIN(string& user, string& pass) {
	int size = int(AccountData.size());
	for (int i = 0; i < size; ++i) {
		if (AccountData[i].username == user) {
			if (AccountData[i].password == pass) {
				return 0;
			}
			else { return 1; }
		}
	}
	return 2;
}

//Kiểm tra thông tin đăng ký từ client
bool ServerManager::CheckUsernameAndPasswordREGISTER(string& user) {
	int size = int(AccountData.size());
	for (int i = 0; i < size; ++i) {
		if (AccountData[i].username == user) {
			return FALSE;
		}
	}
	return TRUE;
}

//Xử lý trao đổi thông tin giữa server và client
void ServerManager::MainProcess() {
	int len = 0;
	while (true) {
		//Nhận độ dài chuỗi yêu cầu chức năng
		Connector.Receive(&len, sizeof(int), 0);
		char* temp1 = new char[len + 1]; temp1[len] = '\0';
		Connector.Receive(temp1, len, 0);

		/*Xử lý yêu cầu đăng nhập từ client*/
		if (strcmp(temp1, "LOGIN") == 0) {
			DlgTextBox->ShowServerInfo(">>> Client dang nhap\n");
			delete[] temp1;
			//Nhận thông điệp về độ dài chuỗi tin nhắn từ client
			Connector.Receive(&len, 4, 0);
			//Khai báo biến chứa thông điệp có độ dài bằng với len
			char* temp2 = new char[len + 1];
			//nhận doạn tin nhắn từ client
			Connector.Receive(temp2, len, 0); temp2[len] = '\0';
			vector<string> tokens = split_string(temp2, '|');
			delete[] temp2;

			string mess[] = { "error1", "error2","ok" };
			int CheckLogin = CheckUsernameAndPasswordLOGIN(tokens[0], tokens[1]);
			//Lỗi nhập sai mật khẩu 
			if (CheckLogin == 1) {
				DlgTextBox->ShowServerInfo("<<< Client da nhap sai mat khau!\n");
				len = mess[1].length();
				Connector.Send(&len, sizeof(int), 0);
				Connector.Send(mess[1].c_str(), len, 0);
			}
			//Lỗi tài khoản không tồn tại
			else if (CheckLogin == 2) {
				DlgTextBox->ShowServerInfo("\n<<< Client dang nhap tai khoan chua dang ky!\n");
				len = mess[0].length();
				Connector.Send(&len, sizeof(int), 0);
				Connector.Send(mess[0].c_str(), len, 0);
			}
			//Đăng nhập thành công
			else {
				DlgTextBox->ShowServerInfo("<<< Client dang nhap thanh cong\n");
				len = mess[2].length();
				Connector.Send(&len, sizeof(int), 0);
				Connector.Send(mess[2].c_str(), len, 0);
			}
		}
		/*Xử lý yêu cầu đăng ký từ client*/
		else if (strcmp(temp1, "REGISTER") == 0) {
			DlgTextBox->ShowServerInfo(">>> Client dang ky tai khoan\n");
			delete[] temp1;
			Connector.Receive(&len, 4, 0);
			char* temp2 = new char[len + 1];
			Connector.Receive(temp2, len, 0); temp2[len] = '\0';
			vector<string> tokens = split_string(temp2, '|');
			delete[] temp2;

			bool CheckRegister = CheckUsernameAndPasswordREGISTER(tokens[0]);
			string mess[] = { "error","ok" };
			//Đăng ký thành công, tài khoản mới được ghi xuống file ACCOUNT.txt
			if (CheckRegister) {
				puts("<<< Client dang ky tai khoan thanh cong!");
				DlgTextBox->ShowServerInfo("<<< Client dang ky tai khoan thanh cong!\n");
				AccountData.push_back({ tokens[0],tokens[1] });
				WriteAccountDataToFile("ACCOUNT.txt");
				len = mess[1].length();
				Connector.Send(&len, sizeof(int), 0);
				Connector.Send(mess[1].c_str(), len, 0);
			}
			//Đăng ký thất bại
			else {
				DlgTextBox->ShowServerInfo("<<< Client dang ky tai khoan that bai!\n");
				len = mess[0].length();
				Connector.Send(&len, sizeof(int), 0);
				Connector.Send(mess[0].c_str(), len, 0);
			}
		}
		/*Xử lý yêu cầu lấy thông tin tỷ giá vàng từ client*/
		else if (strcmp(temp1, "GET_INFORMATION") == 0) {
			DlgTextBox->ShowServerInfo(">>> Client yeu cau thong tin ty gia vang\n");
			delete[] temp1;
			//CODE HERE
		}
		//Xử lý yêu cầu thoát từ client
		else if (strcmp(temp1, "QUIT") == 0) {
			DlgTextBox->ShowServerInfo(">>> Client yeu cau thoat\n");
			delete[] temp1;
			Connector.Receive(&len, 4, 0);
			char* temp2 = new char[len + 1];
			Connector.Receive(temp2, len, 0); temp2[len] = '\0';
			if (strcmp(temp2, "YES")) {//Client đồng ý thoát
				DlgTextBox->ShowServerInfo(">>> Client da thoat!\n");
				delete[] temp2;
				Connector.Close();
				break;
			}
			else {//Client không thoát
				DlgTextBox->ShowServerInfo(">>> Client da huy thoat!\n");
				delete[] temp2;
			}
		}
	}
}