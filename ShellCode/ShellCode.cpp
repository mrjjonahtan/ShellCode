// ShellCode.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main()
{
	PVOID  address = 0;

	//DWORD pid = 0;
	//HWND hwnd = ::FindWindow(TEXT("SciCalc"), TEXT("计算器"));
	//::GetWindowThreadProcessId(hwnd, &pid);
	//if (pid == 0)
	//{
	//	return 0;
	//}
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD)13192);

	if (hProcess == NULL)
	{
		return 0;
	}

	//
	wchar_t param[0x50] = { 0 };
	swprintf_s(param, 0x50, L"%s", L"E://DllInjection.dll");

	address = ::VirtualAllocEx(hProcess, NULL, 0x50, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (address == NULL)
	{
		VirtualFreeEx(hProcess, NULL, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return 0;
	}
	DWORD number = 0;
	if (!::WriteProcessMemory(hProcess, address, param, 0x50, &number))
	{
		return 0;
	}

	//
	HANDLE hRemoteThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibrary, address, NULL, NULL);

	if (NULL == hRemoteThread)
	{
		return 0;
	}

	::WaitForSingleObject(hRemoteThread, INFINITE);
	int d = GetLastError();
	DWORD lpExitCode = -1;
	::GetExitCodeThread(hRemoteThread, &lpExitCode);

	TCHAR sd[250] = { 0 };
	wsprintf(sd, L"-->%d", lpExitCode);
	//OutputDebugString(sd);
	MessageBox(NULL, sd, sd, MB_OK);

	VirtualFreeEx(hProcess, param, 0, MEM_RELEASE);

	::CloseHandle(hRemoteThread);
	hRemoteThread = NULL;
	
	getchar();
	return 0;
}