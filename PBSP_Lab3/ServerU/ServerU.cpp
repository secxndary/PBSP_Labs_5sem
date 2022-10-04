#include <iostream>
#include <clocale>
#include <ctime>
#include "Winsock2.h"
#pragma comment (lib, "WS2_32.lib")
#pragma warning(disable:4996)
using namespace std;




string  GetErrorMsgText(int code)  
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






int main()
{
	SOCKET  sS;
	WSADATA wsaData;
	try
	{
		// =======================  TASK 2  =======================
		
		// 1.
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
			throw  SetErrorMsgText("Startup:", WSAGetLastError());


		// 2.
		if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
			throw  SetErrorMsgText("socket:", WSAGetLastError());
		SOCKADDR_IN serv;
		serv.sin_family = AF_INET;
		serv.sin_port = htons(2000);
		serv.sin_addr.s_addr = inet_addr("127.0.0.1"); 
		if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
			throw  SetErrorMsgText("bind:", WSAGetLastError());


		// 3.
		SOCKADDR_IN clnt;	
		clnt.sin_family = AF_INET;
		clnt.sin_port = htons(2000);
		clnt.sin_addr.s_addr = inet_addr("127.0.0.1");
		memset(&clnt, 0, sizeof(clnt));   
		int lc = sizeof(clnt);
		char ibuf[50];    
		int  lb = 0;         
		if (lb = recvfrom(sS, ibuf, sizeof(ibuf), NULL, (sockaddr*)&clnt, &lc) == SOCKET_ERROR)
			throw  SetErrorMsgText("recv:", WSAGetLastError());
		cout << "[SERVER] Recieved message:   " << ibuf << endl;


		// 4.
		ibuf[17] = '!';		// some changes in data
		if ((lb = sendto(sS, ibuf, strlen(ibuf) + 1, NULL, (sockaddr*)&clnt, sizeof(clnt))) == SOCKET_ERROR)
			throw  SetErrorMsgText("sendto:", WSAGetLastError());
		cout << "[SERVER] Sent message:       " << ibuf << endl;


		// 5.
		if (closesocket(sS) == SOCKET_ERROR)
			throw  SetErrorMsgText("closesocket:", WSAGetLastError());
		if (WSACleanup() == SOCKET_ERROR)
			throw SetErrorMsgText("Cleanup:", WSAGetLastError());
	}
	catch (string errorMsgText)
	{
		cout << endl << "WSAGetLastError: " << errorMsgText;
	}
}
