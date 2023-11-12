#include "windows.h"
#include <iostream>
#include <csignal>
#include <signal.h>

void main()
{
	PROCESS_INFORMATION procInfo;
	STARTUPINFO cif;
	ZeroMemory(&cif, sizeof(STARTUPINFO));
	ZeroMemory(&procInfo, sizeof(PROCESS_INFORMATION));
	if (!CreateProcess(L"D:\\os_labs\\child_winda\\x64\\Debug\\child_winda.exe", NULL,
		NULL, NULL, FALSE, 0, NULL, NULL, &cif, &procInfo))
	{
		std::cout << "no process, error is " << GetLastError << std::endl;
		return;
	}
	
	std::cout << "process started" << std::endl;
	HANDLE hProc = procInfo.hProcess;
	if (hProc != NULL) {
		WaitForSingleObject(procInfo.hProcess, INFINITE);
		DWORD exitCode;
		GetExitCodeProcess(hProc, &exitCode);
		std::cout << "process ended with code: " << exitCode << std::endl;
	}

	CloseHandle(procInfo.hThread);
	return;
}
