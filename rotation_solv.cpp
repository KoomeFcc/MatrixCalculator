#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#define NOCOMM
#include "windows.h"
#include <iostream>
#include <cmath>

int winX = 80;
int winY = 40;
int sizeA = winX * winY; 
char letter;

void drawC(const HANDLE &hConsole, wchar_t *screen, DWORD dwBytesWritten);
void cleanBuffer(wchar_t *screen);

int main(int argc, char const *argv[])
{

	wchar_t *screen = new wchar_t[winX * winY];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL,
		CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	if (!SetConsoleTitle("Test console")) {std::cout << "title fail\n"; return 1;}

	cleanBuffer(screen);
	wsprintfW(&screen[0], L"printex text");
	
	while(1)
	{
		wsprintfW(&screen[0], L"printex text");
		drawC(hConsole, screen, dwBytesWritten);
		Sleep(200);
	}

	std::cout << "Insert a R2 vector" << std::endl;
	float vect[2]; std::cin >> vect[0] >> vect[1];

	std::cout << "Insert the angle to rotate" << std::endl;
	int ang; std::cin >> ang;

	float x = vect[0]; float y = vect[1];
	float rad = ang * M_PI / 180;

	vect[0] =  x * cos(rad) + -1 * (y * sin(rad));
	vect[1] =  x * sin(rad) + y * cos(rad);

	std::cout << "|" << vect[0] << "|\n"
			  << "|" << vect[1] << "|" << std::endl;

	return 0;
}

void drawC(const HANDLE &hConsole, wchar_t *screen, DWORD dwBytesWritten)
{
	screen[winX * winY -1] = '\0';
	WriteConsoleOutputCharacterW(hConsole, screen, winX * winY, {0, 0}, &dwBytesWritten);
}

void cleanBuffer(wchar_t *screen)
{
	for (int i = 0; i < sizeA - 2; ++i) screen[i] = L' ';
}