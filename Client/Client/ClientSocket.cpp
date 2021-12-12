#include "pch.h"
#include "ClientSocket.h"
#include <string>
#include <cstring>
#pragma warning (disable : 4996)

//Khởi tạo đối tượng ClientSocket
ClientSocket::ClientSocket() {}

//Hủy đối tượng ClientSocket
ClientSocket::~ClientSocket() {
	soc.~CSocket();
}

//Khai báo socket cho đối tượng ClientSocket
bool ClientSocket::create() {
	if (soc.Create()) { return TRUE; }
	return FALSE;
}

//Đóng socket
void ClientSocket::close() {
	soc.Close();
}

//Kết nối socket đến server
bool ClientSocket::connect(const CString ip, const int port) {
	if (!soc.Connect(ip, port)) {
		return FALSE;
	}
	return TRUE;
}

//Hàm dùng để gửi các thông điệp như THOÁT,... đến server
void ClientSocket::SendMessage(const CString mes) {
	size_t len = mes.GetLength();
	soc.Send(&len, sizeof(int), 0);
	char* temp = new char[len + 1];
	memset(temp, 0, len + 1);
	wcstombs(temp, mes, len + 1);
	soc.Send(temp, len, 0);
	delete[] temp;
}

//Hàm gửi kết nối đăng nhập đến server
int ClientSocket::login(const CString user, const CString pass, const CString function) {
	size_t len = 0;
	len = function.GetLength();

	//Gửi yêu cầu login cho server
	soc.Send(&len, sizeof(int), 0);
	char* mess1 = new char[len + 1];
	memset(mess1, 0, len);
	wcstombs(mess1, function, len + 1);
	soc.Send(mess1, len, 0);

	//Gửi username và password đến server
	CString compartment = _T("|");//Ký tự vách ngăn giữa chuỗi username và password
	CString m = user + compartment + pass;//"<username>|<password>
	//Gửi độ dài của chuỗi qua cho server để server khởi tạo một biến char có cùng độ dài
	len = m.GetLength();
	soc.Send(&len, sizeof(int), 0);
	char* mess2 = new char[len + 1];
	memset(mess2, 0, len);
	wcstombs(mess2, m, len + 1);
	//Gửi thông điệp mess gồm chuỗi username và password đến server
	soc.Send(mess2, len, 0);
	delete[] mess2;
	//Client nhận thông điệp trả về cho việc login (error1/error2/ok)
	soc.Receive(&len, sizeof(int), 0);
	char* temp = new char[len + 1]; temp[len] = '\0';
	//Nhận thông điệp phản hồi cho quá trình đăng nhập từ server
	soc.Receive(temp, len, 0); 
	if (strcmp(temp, "error1") == 0) { delete[] temp; return 1; }//Lỗi tên đăng nhập chưa được đk
	else if (strcmp(temp, "error2") == 0) { delete[] temp; return 2; }//Lỗi ko khớp mk
	else { delete[] temp; return 0; }
}

//Hàm gửi đăng ký tài khoản đến server
int ClientSocket::Register(const CString user, const CString pass, const CString function) 
{
	size_t len = 0;
	len = function.GetLength();

	//Gửi yêu cầu register cho server
	soc.Send(&len, sizeof(int), 0);
	char* mess1 = new char[len + 1];
	memset(mess1, 0, len);
	wcstombs(mess1, function, len + 1);
	soc.Send(mess1, len, 0);

	//Gửi thông tin đăng ký đến server
	CString compartment = _T("|");//Ký tự vách ngăn giữa chuỗi username và password
	CString m = user + compartment + pass;//"<username>|<password>
	//Gửi độ dài của chuỗi qua cho server để server khởi tạo một biến char có cùng độ dài
	len = m.GetLength();
	soc.Send(&len, sizeof(int), 0);
	char* mess2 = new char[len + 1];
	memset(mess2, 0, len);
	wcstombs(mess2, m, len + 1);
	//Gửi thông điệp mess gồm chuỗi username và password đến server
	soc.Send(mess2, len, 0);
	delete[] mess2;
	//Client nhận thông điệp trả về cho việc login (error1/error2/ok)
	soc.Receive(&len, sizeof(int), 0);
	char* temp = new char[len + 1]; temp[len] = '\0';
	//Nhận thông điệp phản hồi cho quá trình đăng ký từ server
	soc.Receive(temp, len, 0);
	if (strcmp(temp, "error") == 0) { delete[] temp; return 1; }//Lỗi trùng với username đã tồn tại
	else { delete[] temp; return 0; }
}