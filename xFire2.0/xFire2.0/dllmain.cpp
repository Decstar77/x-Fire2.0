#include <Windows.h>
#include "main.h"




bool WINAPI DllMain(HINSTANCE hDLL, DWORD dwReason, LPVOID)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)main, 0, 0, 0);
	}

	return true;
}