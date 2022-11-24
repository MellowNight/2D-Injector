#include "logging.h"

Logger* Logger::logger = NULL;

Logger::Logger()
{
	AllocConsole();
	HWND hWnd = GetConsoleWindow();
	FILE* fp;

	freopen_s(&fp, "CONOUT$", "w", stdout);

   // std::wcout, std::wclog, std::wcerr, std::wcin
	HANDLE hConOut = CreateFileA(("CONOUT$"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE hConIn = CreateFileA(("CONIN$"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	
	SetStdHandle(STD_OUTPUT_HANDLE, hConOut);
	SetStdHandle(STD_ERROR_HANDLE, hConOut);
	SetStdHandle(STD_INPUT_HANDLE, hConIn);

	printf("Hello\n");
}
