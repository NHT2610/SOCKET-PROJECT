#include "pch.h"
#include "ServerManager.h"

ServerManager::ServerManager(CServerDlg* dialog) {
	DlgTextBox = dialog;
	GetAccountDataFromFile("ACCOUNT.txt");
}

ServerManager::~ServerManager() {
	Connector.~CSocket();
}

//bool ServerManager::ServerCreate(int PORT) {
//	if (Server.Create(PORT, SOCK_STREAM, NULL)) { return TRUE; }
//	return FALSE;
//}
//
//void ServerManager::ServerClose() {
//	Server.Close();
//}

//void ServerManager::ConnectorClose() {
//	Connector.Close();
//}

//bool ServerManager::ServerListen() {
//	if (Server.Listen()) { return TRUE; }
//	return FALSE;
//}

//bool ServerManager::ServerAccept() {
//	if (Server.Accept(Connector)) { return TRUE; }
//	return FALSE;
//}

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
	Connector.Attach(*hConnected);
	int len = 0;
	while (true) {
		//Nhận độ dài chuỗi yêu cầu chức năng
		Connector.Receive(&len, sizeof(int), 0);
		char* temp1 = new char[len + 1]; temp1[len] = '\0';
		Connector.Receive(temp1, len, 0);

		/*Xử lý yêu cầu đăng nhập từ client*/
		if (strcmp(temp1, "LOGIN") == 0) {
			//Cập nhật lại dữ liệu mới
			AccountData.clear();
			GetAccountDataFromFile("ACCOUNT.txt");
			DlgTextBox->ShowServerInfo(">>> Client " + to_string(CountThread) + " dang nhap\r\n");
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
				DlgTextBox->ShowServerInfo("<<< Client " + to_string(CountThread)
					+ " da nhap sai mat khau!\r\n");
				len = mess[1].length();
				Connector.Send(&len, sizeof(int), 0);
				Connector.Send(mess[1].c_str(), len, 0);
			}
			//Lỗi tài khoản không tồn tại
			else if (CheckLogin == 2) {
				DlgTextBox->ShowServerInfo("<<< Client " + to_string(CountThread)
					+ " dang nhap tai khoan chua dang ky!\r\n");
				len = mess[0].length();
				Connector.Send(&len, sizeof(int), 0);
				Connector.Send(mess[0].c_str(), len, 0);
			}
			//Đăng nhập thành công
			else {
				DlgTextBox->ShowServerInfo("<<< Client " + to_string(CountThread)
					+ " dang nhap thanh cong\r\n");
				len = mess[2].length();
				Connector.Send(&len, sizeof(int), 0);
				Connector.Send(mess[2].c_str(), len, 0);
			}
		}
		/*Xử lý yêu cầu đăng ký từ client*/
		else if (strcmp(temp1, "REGISTER") == 0) {
			//Cập nhật lại dữ liệu mới
			AccountData.clear();
			GetAccountDataFromFile("ACCOUNT.txt");
			DlgTextBox->ShowServerInfo(">>> Client " + to_string(CountThread)
				+ " dang ky tai khoan\r\n");
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
				DlgTextBox->ShowServerInfo("<<< Client " + to_string(CountThread)
					+ " dang ky tai khoan thanh cong!\r\n");
				AccountData.push_back({ tokens[0],tokens[1] });
				WriteAccountDataToFile("ACCOUNT.txt");
				len = mess[1].length();
				Connector.Send(&len, sizeof(int), 0);
				Connector.Send(mess[1].c_str(), len, 0);
			}
			//Đăng ký thất bại
			else {
				DlgTextBox->ShowServerInfo("<<< Client " + to_string(CountThread)
					+ " dang ky tai khoan that bai!\r\n");
				len = mess[0].length();
				Connector.Send(&len, sizeof(int), 0);
				Connector.Send(mess[0].c_str(), len, 0);
			}
		}
		/*Xử lý yêu cầu lấy thông tin tỷ giá vàng từ client*/
		else if (strcmp(temp1, "GET_INFORMATION") == 0) {
			delete[] temp1;
			//CODE HERE
			//Client yêu cầu giá vàng của ngày/tháng/năm + loại vàng
			Connector.Receive(&len, 4, 0);
			char* temp2 = new char[len + 1];
			Connector.Receive(temp2, len, 0); temp2[len] = '\0';
			//Tách chuỗi ddmmyy và loại vàng
			vector<string> tokens = split_string(temp2, '|');
			delete[] temp2;
			if (strcmp(tokens[1].c_str(), "DEFAULT") == 0) {
				DlgTextBox->ShowServerInfo(">>> Client " + to_string(CountThread)
					+ " yeu cau thong tin ty gia vang ngay " + tokens[0] + "\r\n");
				string TempFileName = "DataBase\\" + tokens[0] + ".txt";
				const char* FileName = TempFileName.c_str();
				FILE* f = fopen(FileName, "r");
				if (f == NULL) {
					//Gửi tín hiệu để thông báo không có dữ liệu
					string mess = "NO_INFORMATION";
					len = int(mess.length());
					Connector.Send(&len, sizeof(int), 0);
					Connector.Send(mess.c_str(), len, 0);
				}
				else {
					//Gửi tín hiệu thông báo có dữ liệu
					string mess = "OK";
					len = int(mess.length());
					Connector.Send(&len, sizeof(int), 0);
					Connector.Send(mess.c_str(), len, 0);
					//Load dữ liệu mặc định của ngày yêu cầu lên server
					wstring s = ReadDataFile(FileName);
					GoldData.clear();
					GoldData = GetData(s, L'|');
					SendGoldData();
				}
			}
			else if (strcmp(tokens[1].c_str(), "DOJI") == 0) {
				DlgTextBox->ShowServerInfo(">>> Client " + to_string(CountThread)
					+ " yeu cau thong tin ty gia vang " + tokens[1] + " ngay " + tokens[0] + "\r\n");
				string TempFileName = "DataBase\\" + tokens[0] + ".txt";
				const char* FileName = TempFileName.c_str();
				FILE* f = fopen(FileName, "r");
				if (f == NULL) {
					//Gửi tín hiệu để thông báo không có dữ liệu
					string mess = "NO_INFORMATION";
					len = int(mess.length());
					Connector.Send(&len, sizeof(int), 0);
					Connector.Send(mess.c_str(), len, 0);
				}
				else {
					//Gửi tín hiệu thông báo có dữ liệu
					string mess = "OK";
					len = int(mess.length());
					Connector.Send(&len, sizeof(int), 0);
					Connector.Send(mess.c_str(), len, 0);
					//Load dữ liệu mặc định của ngày yêu cầu lên server
					wstring s = ReadDataFile(FileName);
					wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;
					wstring Type = converter.from_bytes(tokens[1]);
					GoldData.clear();
					GoldData = GetDataByTypeGold(s, Type);
					SendGoldData();
				}
			}
			else if (strcmp(tokens[1].c_str(), "SJC") == 0) {
				DlgTextBox->ShowServerInfo(">>> Client " + to_string(CountThread)
					+ " yeu cau thong tin ty gia vang " + tokens[1] + " ngay " + tokens[0] + "\r\n");
				string TempFileName = "DataBase\\" + tokens[0] + ".txt";
				const char* FileName = TempFileName.c_str();
				FILE* f = fopen(FileName, "r");
				if (f == NULL) {
					//Gửi tín hiệu để thông báo không có dữ liệu
					string mess = "NO_INFORMATION";
					len = int(mess.length());
					Connector.Send(&len, sizeof(int), 0);
					Connector.Send(mess.c_str(), len, 0);
				}
				else {
					//Gửi tín hiệu thông báo có dữ liệu
					string mess = "OK";
					len = int(mess.length());
					Connector.Send(&len, sizeof(int), 0);
					Connector.Send(mess.c_str(), len, 0);
					//Load dữ liệu mặc định của ngày yêu cầu lên server
					wstring s = ReadDataFile(FileName);
					wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;
					wstring Type = converter.from_bytes(tokens[1]);
					GoldData.clear();
					GoldData = GetDataByTypeGold(s, Type);
					SendGoldData();
				}
			}
		}
		//Xử lý yêu cầu thoát từ client
		else if (strcmp(temp1, "QUIT") == 0) {
			DlgTextBox->ShowServerInfo(">>> Client " + to_string(CountThread) + " yeu cau thoat\r\n");
			delete[] temp1;
			Connector.Receive(&len, 4, 0);
			char* temp2 = new char[len + 1];
			Connector.Receive(temp2, len, 0); temp2[len] = '\0';
			if (strcmp(temp2, "YES") == 0) {//Client đồng ý thoát
				DlgTextBox->ShowServerInfo(">>> Client " + to_string(CountThread) + " da thoat!\r\n");
				delete[] temp2;
				break;
			}
			else {//Client không thoát
				DlgTextBox->ShowServerInfo(">>> Client " + to_string(CountThread) + " da huy thoat!\r\n");
				delete[] temp2;
			}
		}
		else if (strcmp(temp1, "DISCONNECTED") == 0) {
			delete[] temp1;
			DlgTextBox->ShowServerInfo(">>> Client " + to_string(CountThread) + " bi mat ket noi!\r\n");
			break;
		}
	}
	Connector.Close();
}

DWORD WINAPI ThreadProccess(LPVOID lpParameter) {
	ServerManager* myServer = (ServerManager*)lpParameter;
	myServer->MainProcess();
	return 0;
}

//Lấy ngày, tháng, năm của ngày hôm nay
void ServerManager::CurrentDay(int& day, int& month, int& year) {
	time_t now = time(0);
	tm ltm;
	localtime_s(&ltm, &now);
	day = ltm.tm_mday;
	month = ltm.tm_mon + 1;
	year = ltm.tm_year + 1900;
}

//Đọc dữ liệu từ file
wstring ServerManager::ReadDataFile(const char* FileName) {
	_setmode(_fileno(stdin), _O_U16TEXT);
	_setmode(_fileno(stdout), _O_U16TEXT);
	wifstream wif(FileName);
	wif.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>));
	wstringstream wss;
	wss << wif.rdbuf();
	return wss.str();
}

//Lấy dữ liệu từ file đưa vào vector
vector<ServerManager::GoldInformations> ServerManager::GetData(wstring s, wchar_t ch) {
	vector<GoldInformations> DataResult;
	wstring ngay, loai, mua, ban;
	wstring field;
	int FieldCount = 1;
	int size = int(s.length());
	for (int i = 0; i < size; ++i) {
		int j = i;
		while (j < size) {
			if (FieldCount == 5) {
				DataResult.push_back({ ngay,loai,mua,ban });
				ngay = L""; loai = L""; mua = L""; ban = L"";
				FieldCount = 1;
				break;
			}
			if (s[j] == ch) {
				switch (FieldCount)
				{
				case 1: ngay = field; break;
				case 2: loai = field; break;
				case 3: mua = field; break;
				default: ban = field;
				}
				++j;
				++FieldCount;
				field = L"";
				continue;
			}
			field += s[j];
			++j;
		}
		i = j;
	}
	return DataResult;
}

//Hàm lấy dữ liệu có ràng buộc ngày và loại vàng
vector<ServerManager::GoldInformations> ServerManager::GetDataByTypeGold
(wstring s, wstring Type)
{
	vector<GoldInformations> DataResult;
	wstring ngay, loai, mua, ban;
	wstring field;
	wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;
	int FieldCount = 1;
	int size = int(s.length());
	for (int i = 0; i < size; ++i) {
		int j = i;
		while (j < size) {
			if (FieldCount == 5) {
				string a = converter.to_bytes(loai);
				string b = converter.to_bytes(Type);
				if (strstr(a.c_str(), b.c_str())) {
					DataResult.push_back({ ngay,loai,mua,ban });
				}
				ngay = L""; loai = L""; mua = L""; ban = L"";
				FieldCount = 1;
				break;
			}
			if (s[j] == L'|') {
				switch (FieldCount)
				{
				case 1: ngay = field; break;
				case 2: loai = field; break;
				case 3: mua = field; break;
				default: ban = field;
				}
				++j;
				++FieldCount;
				field = L"";
				continue;
			}
			field += s[j];
			++j;
		}
		i = j;
	}
	return DataResult;
}

//Hàm gửi dữ liệu giá vàng từ server
void ServerManager::SendGoldData() {
	int size = int(GoldData.size());
	int len = 0;
	wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;
	for (int i = 0; i < size; ++i) {
		wstring Field = GoldData[i].LoaiVang + L"#" + GoldData[i].GiaMua + L"#"
			+ GoldData[i].GiaBan + L"#" + GoldData[i].Date;
		string message = converter.to_bytes(Field);
		len = int(message.length());
		Connector.Send(&len, sizeof(int), 0);
		Connector.Send(message.c_str(), len, 0);
	}
	string last = "END";
	len = int(last.length());
	Connector.Send(&len, sizeof(int), 0);
	Connector.Send(last.c_str(), len, 0);
}