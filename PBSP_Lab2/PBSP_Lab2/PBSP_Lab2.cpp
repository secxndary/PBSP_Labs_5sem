#include <iostream>
#include "Winsock2.h"
#pragma comment (lib, "WS2_32.lib")
using namespace std;
 




string  GetErrorMsgText(int code)    // cформировать текст ошибки 
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
	// ======================  TASK 2  =======================

	//SOCKET sS = socket(AF_INET, SOCK_STREAM, NULL);
	//try
	//{
	//	//............................................................
	//	if ((sS = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
	//		throw  SetErrorMsgText("socket:", WSAGetLastError());
	//	//.............................................................
	//}
	//catch (string errorMsgText)
	//{
	//	cout << endl << "WSAGetLastError: " << errorMsgText;
	//}



	// ======================  TASK 3  =======================
	
	//WSADATA wsaData;
	//try
	//{
	//	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
	//		throw  SetErrorMsgText("Startup:", WSAGetLastError());
	//	//...........................................................
	//	if (WSACleanup() == SOCKET_ERROR)
	//		throw  SetErrorMsgText("Cleanup:", WSAGetLastError());
	//}
	//catch (string errorMsgText)
	//{
	//	cout << endl << errorMsgText;
	//}




	// ======================  TASK 4  =======================

	//SOCKET  sS;           // дескриптор сокета 
	//WSADATA wsaData;
	//try
	//{
	//	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
	//		throw  SetErrorMsgText("Startup:", WSAGetLastError());
	//	if ((sS = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
	//		throw  SetErrorMsgText("socket:", WSAGetLastError());
	//	//...........................................................
	//	if (closesocket(sS) == SOCKET_ERROR)
	//		throw  SetErrorMsgText("closesocket:", WSAGetLastError());
	//	if (WSACleanup() == SOCKET_ERROR)
	//		throw  SetErrorMsgText("Cleanup:", WSAGetLastError());
	//}
	//catch (string errorMsgText)
	//{
	//	cout << endl << errorMsgText;
	//}




	// ======================  TASK 5  =======================

	try
	{
		SOCKET  sS;                           // серверный сокет
		if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
			throw  SetErrorMsgText("socket:", WSAGetLastError());

		SOCKADDR_IN serv;                     // параметры  сокета sS
		serv.sin_family = AF_INET;           // используется IP-адресация  
		serv.sin_port = htons(2000);          // порт 2000
		serv.sin_addr.s_addr = INADDR_ANY;   // любой собственный IP-адрес 

		if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
			throw  SetErrorMsgText("bind:", WSAGetLastError());
	}
	catch (string errorMsgText)
	{
		cout << endl << errorMsgText;
	}
}
