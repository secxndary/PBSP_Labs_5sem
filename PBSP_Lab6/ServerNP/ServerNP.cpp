#include <iostream>
#include "Winsock2.h"
#include "Windows.h"
#include "ErrorMessage.h"
#pragma comment (lib, "WS2_32.lib")
#pragma warning(disable:4996)
#define PIPE_NAME L"\\\\.\\pipe\\Tube"
using namespace std;






int main()
{
	setlocale(LC_ALL, "ru");
	HANDLE sH;	// дескриптор канала


	try
	{
		// 1.
		if ((sH = CreateNamedPipe(
			PIPE_NAME,
			PIPE_ACCESS_DUPLEX,					// дуплексный канал 
			PIPE_TYPE_MESSAGE | PIPE_WAIT,		// сообщения|синхронный
			1, NULL, NULL,						// максимум 1 экземпляр
			INFINITE, NULL)) == INVALID_HANDLE_VALUE)
		throw SetPipeError("create:", GetLastError());

		if (!ConnectNamedPipe(sH, NULL))        // ожидать клиента   
			throw SetPipeError("connect:", GetLastError());




		// 4.
		if (!DisconnectNamedPipe(sH))
			throw SetPipeError("disconnect:", GetLastError());
		if (!CloseHandle(sH))
			throw SetPipeError("close:", GetLastError());
	}
	catch (string ErrorPipeText)
	{
		cout << endl << ErrorPipeText;
	}

}