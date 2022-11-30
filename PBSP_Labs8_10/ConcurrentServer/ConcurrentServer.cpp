#include <iostream>
#include <clocale>
#include "ErrorMessage.h"
#include "Windows.h"
#include <time.h>
#pragma comment (lib, "WS2_32.lib")
#pragma warning(disable:4996)
using namespace std;
#define PORT 2000;



HANDLE hAcceptServer,    // дескриптор потока AcceptServer
hConsolePipe,			 // дескриптор потока ConsolePipe 
hGarbageCleaner;		 // дескриптор потока GarbageCleaner
DWORD WINAPI AcceptServer(LPVOID pPrm);  // прототипы функций 
DWORD WINAPI ConsolePipe(LPVOID pPrm);
DWORD WINAPI GarbageCleaner(LPVOID pPrm);



int _tmain(int argc, TCHAR* argv[])
{

    return 0;
}




DWORD WINAPI AcceptServer(LPVOID pPrm)
{
    DWORD rc = 0;    // код возврата 
    SleepEx(1000, TRUE);
    SOCKET  ServerSocket;
    WSADATA wsaData;


    try {
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
            throw SetErrorMsgText("Startup:", WSAGetLastError());
        if ((ServerSocket = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
            throw SetErrorMsgText("Socket:", WSAGetLastError());

        SOCKADDR_IN Server_IN;
        Server_IN.sin_family = AF_INET;
        Server_IN.sin_port = htons(2000);
        Server_IN.sin_addr.s_addr = ADDR_ANY;
        /*Server_IN.sin_addr.s_addr = inet_addr("127.0.0.1");*/

        if (bind(ServerSocket, (LPSOCKADDR)&Server_IN, sizeof(Server_IN)) == SOCKET_ERROR)
            throw SetErrorMsgText("Bind:", WSAGetLastError());
        if (listen(ServerSocket, SOMAXCONN) == SOCKET_ERROR)
            throw SetErrorMsgText("Listen:", WSAGetLastError());

        cout << "qwe";
        if (closesocket(ServerSocket) == SOCKET_ERROR)
            throw SetErrorMsgText("Closesocket:", WSAGetLastError());
        if (WSACleanup() == SOCKET_ERROR)
            throw SetErrorMsgText("Cleanup:", WSAGetLastError());


        ExitThread(rc);  // завершение работы потока
    }
    catch(string e) {
        cout << e;
    }
}
