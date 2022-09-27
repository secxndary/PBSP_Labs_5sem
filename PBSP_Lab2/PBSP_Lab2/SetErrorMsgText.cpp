#include <iostream>
#include "Winsock2.h"
#pragma comment (lib, "WS2_32.lib")
using namespace std;


string SetErrorMsgText(string msgText, int code)
{
	return  msgText + GetErrorMsgText(code);
};


string  GetErrorMsgText(int code)    // c����������� ����� ������ 
{
	string msgText;
	switch (code)                      // �������� ���� ��������  
	{
	case WSAEINTR:          msgText = "WSAEINTR";         break;
	case WSAEACCES:         msgText = "WSAEACCES";        break;
		//..........���� WSAGetLastError ..........................
	case WSASYSCALLFAILURE: msgText = "WSASYSCALLFAILURE"; break;
	default:                msgText = "***ERROR***";      break;
	};
	return msgText;
};
