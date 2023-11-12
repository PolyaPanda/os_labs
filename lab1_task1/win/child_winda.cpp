#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <csignal>

BOOL WINAPI HandlerRoutine(DWORD ctrlType) {
	switch (ctrlType)
	{s
	case CTRL_CLOSE_EVENT:
		std::cout << "CTRL close event" << std::endl;
		break;
	case CTRL_C_EVENT:
		std::cout << "CTRL+C event" << std::endl;
		break;
	default:
		break;
	}
	return false;
}

int main() {
	SetConsoleCtrlHandler(HandlerRoutine, true);
	while (1);
	return 0;
}
