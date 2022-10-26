#include <iostream>
#include "Winsock2.h"
#pragma comment (lib, "WS2_32.lib")
#pragma warning(disable:4996)
using namespace std;





string GetErrorMsgText(int code)
{
	string msgText;
	switch (code)
	{
	case WSAEINTR:          msgText = "WSAEINTR";         break;
	case WSAEACCES:         msgText = "WSAEACCES";        break;
	case WSASYSCALLFAILURE: msgText = "WSASYSCALLFAILURE"; break;
	default:                msgText = "***ERROR***";      break;
	};
	return msgText;
};


string SetErrorMsgText(string msgText, int code)
{
	return msgText + GetErrorMsgText(code);
}





SOCKET  sS;


bool GetRequestFromClient(
    char* name,                 // [in] позывной сервера  
    short port,                 // [in] номер просушиваемого порта 
    struct sockaddr* from,      // [out] указатель на SOCKADDR_IN
    int* flen                   // [out] указатель на размер from 
)
{
	// 0.
	char clientName[20];
	memset(from, 0, sizeof(flen));



	// 2.
	if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
		throw  SetErrorMsgText("socket:", WSAGetLastError());

	SOCKADDR_IN serv;
	serv.sin_family = AF_INET;
	serv.sin_port = htons(port);
	serv.sin_addr.s_addr = INADDR_ANY;

	if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
		throw  SetErrorMsgText("bind:", WSAGetLastError());



	// 3.
	if (recvfrom(sS, clientName, sizeof(clientName), NULL, from, flen) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAETIMEDOUT)
			return false;
		else
			throw SetErrorMsgText("recvfrom:", WSAGetLastError());
	}



	// compare server-name
	if (!strcmp(clientName, name))
	{
		cout << "[OK] Correct server callsign: " << clientName << "\n";
		return true;
	}
	else
	{
		cout << "[ERROR] Incorrect server callsign: " << clientName << "\n";
		return false;
	}

}







bool PutAnswerToClient(
	char* name,				// [in] позывной сервера  
	struct sockaddr* to,	// [in] указатель на SOCKADDR_IN
	int* lto				// [in] указатель на размер from 
)
{
	if ((sendto(sS, name, strlen(name) + 1, NULL, to, *lto)) == SOCKET_ERROR)
	{
		cout << "[ERROR] The response was not sent to the client.\n";
		throw  SetErrorMsgText("send:", WSAGetLastError());
	}

	cout << "[OK] The response was sent to the client: " << name << "\n";
	return true;
}









int main()
{
	// 0.
	setlocale(LOCALE_ALL, "ru");
	WSADATA wsaData;
	SOCKADDR_IN clnt;
	int lc = sizeof(clnt);
	char name[] = "Hello";



	// 1.
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		throw  SetErrorMsgText("Startup:", WSAGetLastError());


	
	// get request and put answer
	while (true)
	{
		if (GetRequestFromClient(name, 2000, (sockaddr*)&clnt, &lc))
		{
			PutAnswerToClient(name, (sockaddr*)&clnt, &lc);
		}
		else
		{
			PutAnswerToClient((char*)"[ERROR]", (sockaddr*)&clnt, &lc);
		}

		SOCKADDR_IN* addr = (SOCKADDR_IN*)&clnt;
		cout << "\n[INFO] Client port: " << addr->sin_port;
		cout << "\n[INFO] Client IP: " << inet_ntoa(addr->sin_addr) << "\n\n\n";

		if (closesocket(sS) == SOCKET_ERROR)
			throw  SetErrorMsgText("closesocket:", WSAGetLastError());
	}



	// 5.
	if (WSACleanup() == SOCKET_ERROR)
		throw SetErrorMsgText("Cleanup:", WSAGetLastError());
}
