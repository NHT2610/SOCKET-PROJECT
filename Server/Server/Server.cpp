// Server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "framework.h"
#include "Server.h"
#include <afxsock.h>
#include <thread>
#include "ConnectThread.h"
#include <winsqlite/winsqlite3.h>

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
            CSocket connect[NUMBERS_OF_THREAD];
            int count = 0;

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
                cout << "DANG DOI KET NOI TU CLIENT...\n";
                socket_server.Listen(1);
                if (socket_server.Accept(connect[0])) {
                    cout << "Client da ket noi!\n";
                    thread thr1(ConnectThread(), ref(connect[0]), 1);
                    //thr1.join();
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
