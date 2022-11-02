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
	HANDLE cH; // дескриптор канала


	try
	{
		// 1.
		if ((cH = CreateFile(
			PIPE_NAME,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, OPEN_EXISTING, NULL,
			NULL)) == INVALID_HANDLE_VALUE)
		throw  SetPipeError("CreateFile: ", GetLastError());




		// 4.
		if (!CloseHandle(cH))
			throw SetPipeError("close:", GetLastError());
	}
	catch (string ErrorPipeText)
	{
		cout << endl << ErrorPipeText;
	}

}
