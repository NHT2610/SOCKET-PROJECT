#include "pch.h"
#include "ClientSocket.h"

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