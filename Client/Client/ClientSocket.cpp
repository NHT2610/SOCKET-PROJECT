#include "pch.h"
#include "ClientSocket.h"
#pragma warning (disable : 4996)

//Khởi tạo đối tượng ClientSocket
ClientSocket::ClientSocket() {}

//Hủy đối tượng ClientSocket
ClientSocket::~ClientSocket() {
	CString mess = _T("DISCONNECTED");
	int len = int(mess.GetLength());
	soc.Send(&len, sizeof(int), 0);
	char* temp = new char[len + 1];
	memset(temp, 0, len + 1);
	wcstombs(temp, mess, len + 1);
	delete[] temp;
	soc.Send(mess, len, 0);
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

//Hàm phân tích chuỗi ngày tháng năm thành ngày/tháng/năm
wstring ClientSocket::ParseDay(wstring Day) {
	wstring result;
	wstring day, month, year;
	day += Day[0]; day += Day[1];
	month += Day[2]; month += Day[3];
	year += Day[4];
	year += Day[5];
	year += Day[6];
	year += Day[7];
	result += day + L"/" + month + L"/" + year;
	return result;
}

//Hàm phân tích ra từng thành phần của một dòng dữ liệu
ClientSocket::GoldInformations ClientSocket::ParseData(wstring s) {
	GoldInformations result;
	int len = int(s.length());
	int j;
	wstring field;
	int count = 1;
	for (int i = 0; i < len; ++i) {
		for (j = i; j < len && s[j] != L'#'; ++j) {
			field += s[j];
		}
		switch (count)
		{
		case 1: result.LoaiVang = field; break;
		case 2: result.GiaMua = field; break;
		case 3: result.GiaBan = field; break;
		default: result.Ngay = ParseDay(field);
		}
		++count;
		field = L"";
		i = j;
	}
	return result;
}

//Hàm gửi yêu cầu tỷ giá vàng, và nhận về mảng dữ liệu
vector<ClientSocket::GoldInformations> ClientSocket::GetDataFromServer(string message) {
	int len = int(message.length());
	//Gửi yêu cầu đến server với thông tin ngày tháng năm và loại vàng
	soc.Send(&len, sizeof(int), 0);
	soc.Send(message.c_str(), len, 0);

	vector<GoldInformations> DataRecv;
	wstring ngay, loai, mua, ban;
	wstring field;
	len = 0;
	wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;
	while (true) {
		soc.Receive(&len, sizeof(int), 0);
		char* temp = new char[len + 1]; temp[len] = '\0';
		soc.Receive(temp, len, 0);
		if (strcmp(temp, "END") == 0) { break; }
		wstring DataLine = converter.from_bytes(temp);
		DataRecv.push_back(ParseData(DataLine));
		delete[] temp;
	}
	return DataRecv;
}