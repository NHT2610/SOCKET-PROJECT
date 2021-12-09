// Server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "framework.h"
#include "Server.h"
#include <afxsock.h>
#include <thread>
#include "Client.h"

#define NUMBERS_OF_THREAD 10

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// The one and only application object

CWinApp theApp;

using namespace std;

int main()
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // initialize MFC and print and error on failure
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: code your application's behavior here.
            wprintf(L"Fatal Error: MFC initialization failed\n");
            nRetCode = 1;
        }
        else
        {
            // TODO: code your application's behavior here.
            thread thr[NUMBERS_OF_THREAD];
            Client* client_conector = new Client[NUMBERS_OF_THREAD];
            //Khai báo sử dụng socket trong Window
            if (!AfxSocketInit(NULL)) {
                cout << "KHONG THE khoi tạo thu vien socket!\n";
                return FALSE;
            }
            CSocket socket_server;//Socket của server
            //Khởi tạo socket với port 12345
            if (!socket_server.Create(12345, SOCK_STREAM, NULL)) {//Truyền kiểu socket: SOCK_STREAM <=> sử dụng giao thức TCP
                cout << "Khoi tao THAT BAI!\n";
                cout << socket_server.GetLastError();//Lấy mã lỗi khởi tạo không thành công
                return FALSE;
            }
            else {
                cout << "Server khoi tao THANH CONG!\n";
                if (!socket_server.Listen(3)) {
                    cout << "KHONG THE ket noi tren port nay!\n";
                    socket_server.Close();
                    return FALSE;
                }
            }
            socket_server.Close();
        }
    }
    else
    {
        // TODO: change error code to suit your needs
        wprintf(L"Fatal Error: GetModuleHandle failed\n");
        nRetCode = 1;
    }

    return nRetCode;
}
