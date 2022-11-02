#include <iostream>
#include "Winsock2.h"
#include "Windows.h"
#include "ErrorMessage.h"
#pragma comment (lib, "WS2_32.lib")
#pragma warning(disable:4996)
#define PIPE_NAME L"\\\\.\\pipe\\Tube"
#define STOP "STOP"
using namespace std;








int main()
{
	setlocale(LC_ALL, "ru");
	HANDLE cH; // дескриптор канала
	DWORD lp;
	char buf[50];


	try
	{
		// 1.
		if ((cH = CreateFile(
			PIPE_NAME,								// [in] символическое имя канала 
			GENERIC_READ | GENERIC_WRITE,			// [in] чтение или запись в канал 
			FILE_SHARE_READ | FILE_SHARE_WRITE,		// [in] режим совместного использования
			NULL, OPEN_EXISTING, NULL,				// [in] атрибуты безопасности, флаги открытия канала
			NULL)) == INVALID_HANDLE_VALUE)
		throw  SetPipeError("CreateFile: ", GetLastError());




		// 2.
		string obuf = "Hello from ClientNP ";
		if (!WriteFile(cH, obuf.c_str(), sizeof(obuf), &lp, NULL))
			throw SetPipeError("WriteFile: ", GetLastError());



		// 3.
		if (!ReadFile(cH, buf, sizeof(buf), &lp, NULL))
			throw SetPipeError("ReadFile: ", GetLastError());
		cout << "[OK] Sent message: " << buf << endl;




		// 4.
		if (!CloseHandle(cH))
			throw SetPipeError("Close: ", GetLastError());
	}
	catch (string ErrorPipeText)
	{
		cout << endl << ErrorPipeText;
	}

}
