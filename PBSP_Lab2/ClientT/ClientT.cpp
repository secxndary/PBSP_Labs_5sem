#include <iostream>
#include <string>
#include <stdio.h>
#include "Winsock2.h"
#pragma comment (lib, "WS2_32.lib")
#pragma warning(disable:4996)
using namespace std;



string GetErrorMsgText(int code)    // cформировать текст ошибки 
{
	string msgText;
	switch (code)                      // проверка кода возврата  
	{
	case WSAEINTR:          msgText = "WSAEINTR";         break;
	case WSAEACCES:         msgText = "WSAEACCES";        break;
		//..........коды WSAGetLastError ..........................
	case WSASYSCALLFAILURE: msgText = "WSASYSCALLFAILURE"; break;
	default:                msgText = "***ERROR***";      break;
	};
	return msgText;
};



string SetErrorMsgText(string msgText, int code)
{
	return msgText + GetErrorMsgText(code);
}





int main()
{
	SOCKET  cC;           // дескриптор сокета 
	WSADATA wsaData;

	try
	{
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
			throw  SetErrorMsgText("Startup:", WSAGetLastError());
		if ((cC = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
			throw  SetErrorMsgText("socket:", WSAGetLastError());

		SOCKADDR_IN serv;                    // параметры  сокета сервера
		serv.sin_family = AF_INET;           // используется IP-адресация  
		serv.sin_port = htons(2000);                   // TCP-порт 2000
		serv.sin_addr.s_addr = inet_addr("192.168.56.104");  // адрес сервера
		if ((connect(cC, (sockaddr*)&serv, sizeof(serv))) == SOCKET_ERROR)
			throw  SetErrorMsgText("connect:", WSAGetLastError());


	


		char obufText[19] = "Hello from client ";  
		int lobufText = 19; 

		string strText(obufText);
		for (int i = 0; i < 10; i++)
		{
			char buf10[21];
			string num = to_string(i);
			string strWithNum = strText + num + "\n";
			strcpy(buf10, strWithNum.c_str());
			cout << buf10;
			if ((lobufText = send(cC, buf10, strlen(buf10), NULL)) == SOCKET_ERROR)
				throw  SetErrorMsgText("send:", WSAGetLastError());
		}

		for (int i = 10; i < 100; i++)
		{
			char buf100[22];
			string num = to_string(i);
			string strWithNum = strText + num + "\n";
			strcpy(buf100, strWithNum.c_str());
			cout << buf100;
			if ((lobufText = send(cC, buf100, strlen(buf100), NULL)) == SOCKET_ERROR)
				throw  SetErrorMsgText("send:", WSAGetLastError());
		}

		for (int i = 100; i < 1000; i++)
		{
			char buf100[23];
			string num = to_string(i);
			string strWithNum = strText + num + "\n";
			strcpy(buf100, strWithNum.c_str());
			cout << buf100;
			if ((lobufText = send(cC, buf100, strlen(buf100), NULL)) == SOCKET_ERROR)
				throw  SetErrorMsgText("send:", WSAGetLastError());
		}










		if (closesocket(cC) == SOCKET_ERROR)
			throw  SetErrorMsgText("closesocket:", WSAGetLastError());
		if (WSACleanup() == SOCKET_ERROR)
			throw  SetErrorMsgText("Cleanup:", WSAGetLastError());
	}
	catch (string errorMsgText)
	{
		cout << endl << "WSAGetLastError: " << errorMsgText;
	}
}