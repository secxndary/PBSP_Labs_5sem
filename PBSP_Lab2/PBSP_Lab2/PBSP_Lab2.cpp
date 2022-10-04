#include <iostream>
#include <clocale>
#include <ctime>
#include "Winsock2.h"
#pragma comment (lib, "WS2_32.lib")
#pragma warning(disable:4996)
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
	//setlocale(LC_ALL, "Russian");

	SOCKET  sS;           // дескриптор сокета 
	WSADATA wsaData;

	try
	{

		//if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		//	throw  SetErrorMsgText("Startup:", WSAGetLastError());
		//if ((sS = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
		//	throw  SetErrorMsgText("socket:", WSAGetLastError());
		////...........................................................
		//SOCKADDR_IN serv;                     // параметры  сокета sS
		//serv.sin_family = AF_INET;           // используется IP-адресация  
		//serv.sin_port = htons(2000);          // порт 2000
		//serv.sin_addr.s_addr = INADDR_ANY;   // любой собственный IP-адрес 

		//if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
		//	throw  SetErrorMsgText("bind:", WSAGetLastError());
		//if (listen(sS, SOMAXCONN) == SOCKET_ERROR)
		//	throw  SetErrorMsgText("listen:", WSAGetLastError());



		//SOCKET cS;	                 // сокет для обмена данными с клиентом 
		//SOCKADDR_IN clnt;             // параметры  сокета клиента
		//memset(&clnt, 0, sizeof(clnt)); // обнулить память
		//int lclnt = sizeof(clnt);    // размер SOCKADDR_IN
		//

		//char* ip = inet_ntoa(clnt.sin_addr);	// ф-ия для получения айпи адреса клиента
		//cout << "Client IP:     " << ip << endl;
		//cout << "Client port:   " << clnt.sin_port << endl;












		// =======================  TASK 11  =======================


		//char ibuf[19] = "Hello from client\n";
		//int  libuf = 19;

		//for (int i = 1; i <= 1000; i++)
		//{
		//	if ((libuf = recv(cS, ibuf, sizeof(ibuf) - 1, NULL)) == SOCKET_ERROR)
		//		throw  SetErrorMsgText("recv:", WSAGetLastError());

		//	const int outLen = 19;
		//	char charOut[outLen + 1];
		//	int numBytes = recv(cS, charOut, outLen, 0);
		//	if (numBytes > 0) {
		//		charOut[numBytes] = '\0';
		//		cout << charOut;
		//	}
		//}









		// =======================  TASK 13  =======================


		/*char ibuf10[21] = "Hello from client\n";
		int  libuf10 = 21;
		for (int i = 0; i < 10; i++)
		{
			if ((libuf10 = recv(cS, ibuf10, sizeof(ibuf10) - 21, NULL)) == SOCKET_ERROR)
				throw  SetErrorMsgText("recv:", WSAGetLastError());
			const int outLen = 21;
			char charOut[outLen + 1];
			int numBytes = recv(cS, charOut, outLen, 0);
			if (numBytes > 0)
			{
				charOut[numBytes] = '\0';
				cout << charOut;
			}
		}

		for (int i = 10; i < 100; i++)
		{
			if ((libuf10 = recv(cS, ibuf10, sizeof(ibuf10) - 21, NULL)) == SOCKET_ERROR)
				throw  SetErrorMsgText("recv:", WSAGetLastError());
			const int outLen = 22;
			char charOut[outLen + 1];
			int numBytes = recv(cS, charOut, outLen, 0);
			if (numBytes > 0)
			{
				charOut[numBytes] = '\0';
				cout << charOut;
			}
		}


		for (int i = 100; i < 1000; i++)
		{
			if ((libuf10 = recv(cS, ibuf10, sizeof(ibuf10) - 21, NULL)) == SOCKET_ERROR)
				throw  SetErrorMsgText("recv:", WSAGetLastError());
			const int outLen = 23;
			char charOut[outLen + 1];
			int numBytes = recv(cS, charOut, outLen, 0);
			if (numBytes > 0)
			{
				charOut[numBytes] = '\0';
				cout << charOut;
			}
		}*/










		// =======================  TASK 15  =======================
		// to run this comment lines 48-73 

		int i = 0;
		WSADATA ws;
		SOCKET s;
		SOCKET c;
		char ibuf[50];
		char obuf[50] = "Hello from Client";
		int t;

		if (FAILED(WSAStartup(MAKEWORD(1, 1), &ws)))
			cout << "socket:" << WSAGetLastError() << endl;
		if (INVALID_SOCKET == (s = socket(AF_INET, SOCK_STREAM, 0)))
			cout << "socket:" << WSAGetLastError() << endl;

		sockaddr_in c_adr;
		sockaddr_in s_adr;
		{
			s_adr.sin_port = htons(2000);
			s_adr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
			s_adr.sin_family = AF_INET;
		}

		if (SOCKET_ERROR == (bind(s, (LPSOCKADDR)&s_adr, sizeof(s_adr))))
			cout << "bind:" << WSAGetLastError() << endl;

		if (SOCKET_ERROR == listen(s, 2))
			cout << "listen:" << WSAGetLastError << endl;


		while (true)
		{
			int lcInt = sizeof(c_adr);
			if (INVALID_SOCKET == (c = accept(s, (sockaddr*)&c_adr, &lcInt)))
				cout << "accept:" << WSAGetLastError() << endl;

			cout << "Client connected." << endl;
			cout << "Client IP:    " << inet_ntoa(c_adr.sin_addr) << endl;
			cout << "Client port:  " << htons(c_adr.sin_port) << "\n\n";

			int time = clock();
			while (true)
			{
				if (SOCKET_ERROR == recv(c, ibuf, sizeof(ibuf), NULL))
					break;

				cout << ibuf << " " << i << "\n";
				i++;

				if (!strcmp(ibuf, "CLOSE")) { break; }

				if (SOCKET_ERROR == send(c, obuf, strlen(obuf) + 1, NULL))
					break;
			}
			i = 0;
			cout << "Client Disconected.\n";
			cout << "Program was running for " << time << " ticks or " << ((float)time) / CLOCKS_PER_SEC << " seconds.\n\n";
		}
		if (closesocket(c) == SOCKET_ERROR)
			throw SetErrorMsgText("closesocket:", WSAGetLastError());

		if (WSACleanup() == SOCKET_ERROR)
			throw SetErrorMsgText("Cleanup:", WSAGetLastError());












		if (closesocket(sS) == SOCKET_ERROR)
			throw  SetErrorMsgText("closesocket:", WSAGetLastError());
		if (WSACleanup() == SOCKET_ERROR)
			throw  SetErrorMsgText("Cleanup:", WSAGetLastError());

	}
	catch (string errorMsgText)
	{
		cout << endl << "WSAGetLastError: " << errorMsgText;
	}
}
