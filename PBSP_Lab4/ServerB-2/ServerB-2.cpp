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



	// compare server callsign
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






bool GetServer(
	char* call,					// [in] позывной сервера  
	short            port,		// [in] номер порта сервера    
	struct sockaddr* from,		// [out] указатель на SOCKADDR_IN
	int* flen					// [out] указатель на размер from 
)
{
	memset(from, 0, sizeof(flen));
	int optval = 1;
	SOCKET cC;
	int countOfServers = 0;
	int timeout = 5000;

	try
	{
		if ((cC = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
			throw SetErrorMsgText("socket:", WSAGetLastError());

		if (setsockopt(cC, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(int)) == SOCKET_ERROR)
			throw SetErrorMsgText("opt:", WSAGetLastError());

		// we need this to prevent the server from going into standby mode
		if (setsockopt(cC, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(int)) == SOCKET_ERROR)
			throw SetErrorMsgText("opt:", WSAGetLastError());


		SOCKADDR_IN all;                        // параметры  сокета sS
		all.sin_family = AF_INET;               // используется IP-адресация  
		all.sin_port = htons(port);				// порт 2000
		all.sin_addr.s_addr = INADDR_BROADCAST; // всем 
		char clientCall[50];


		if ((sendto(cC, call, strlen(call) + 1, NULL, (sockaddr*)&all, sizeof(all))) == SOCKET_ERROR)
			throw SetErrorMsgText("sendto:", WSAGetLastError());
		//cout << "[OK] Sent message: " << call << "\n";


		if ((recvfrom(cC, clientCall, sizeof(clientCall), NULL, from, flen)) == SOCKET_ERROR)
		{
			/*if (WSAGetLastError() == WSAETIMEDOUT)
				return false;
			else*/
			throw SetErrorMsgText("recv:", WSAGetLastError());
		}
		//cout << "[OK] Received message: " << clientCall << "\n";




		if (!strcmp(clientCall, call))
		{
			++countOfServers;
			cout << "[INFO] Found server #" << countOfServers << "\n";
			SOCKADDR_IN* addr = (SOCKADDR_IN*)&from;
			cout << "[INFO] Server port: " << addr->sin_port << "\n";
			cout << "[INFO] Server IP: " << inet_ntoa(addr->sin_addr) << "\n\n\n";
			return true;
		}
		else
			cout << "[ERROR] Did not found server with callname: " << call << "\n\n";

	}
	catch (string errorMsgText)
	{
		if (WSAGetLastError() == WSAETIMEDOUT)
		{
			cout << "Number of servers: " << countOfServers << endl;
			if (closesocket(cC) == SOCKET_ERROR)
				throw SetErrorMsgText("closesocket:", WSAGetLastError());
		}
		else
			throw SetErrorMsgText("GetServer:", WSAGetLastError());
	}

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
		throw SetErrorMsgText("Startup:", WSAGetLastError());



	// find servers with the same port
	GetServer(name, 2000, (sockaddr*)&clnt, &lc);



	// get request and put answer
	while (true)
	{
		if (GetRequestFromClient(name, 2000, (sockaddr*)&clnt, &lc))
			PutAnswerToClient(name, (sockaddr*)&clnt, &lc);
		// (you can uncomment this if you want to)
		// else PutAnswerToClient((char*)"ERROR! Enter correct callsign.", (sockaddr*)&clnt, &lc);

		SOCKADDR_IN* addr = (SOCKADDR_IN*)&clnt;
		cout << "\n[INFO] Client port: " << addr->sin_port;
		cout << "\n[INFO] Client IP: " << inet_ntoa(addr->sin_addr) << "\n\n\n\n\n";

		if (closesocket(sS) == SOCKET_ERROR)
			throw  SetErrorMsgText("closesocket:", WSAGetLastError());
	}



	// 5.
	if (WSACleanup() == SOCKET_ERROR)
		throw SetErrorMsgText("Cleanup:", WSAGetLastError());
}
