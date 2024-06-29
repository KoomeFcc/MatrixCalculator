#include "windows.h"
#include <cstring>
#include <iostream>
#include "strsafe.h"
#include "stdio.h"
#include <cmath>
#include <array>
#include <string>

#define RAD M_PI / 180

int winX = 80;
int winY = 40;
int number = 0;
const int stringSize = 150;
const int numBuffer = 10;
int n = 0;
HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
// HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL,
// 	CONSOLE_TEXTMODE_BUFFER, NULL);
DWORD dwBytesWritten;
DWORD bytesRead;
struct vectorResult
{
	float vx;
	float vy;
};
struct vector2
{
	char x[10];
	char y[10];
};
vector2 Vinput;
vectorResult vectorR; 
vectorResult result;
vectorResult output;

void  ErrorExit(LPCTSTR lpszFunction);
void printMenu();
void cls();
vectorResult rotMatrix(int angle);
vectorResult scaMatrix(int scale);
vectorResult shadMatrix(int shade);
vectorResult disMatrix(int mov);
void GetVector();
char Iterator();
void DisplayResult(vectorResult vecResult);
void DisplaySomething(const char *text);
char GetInput();

int main(int argc, char const *argv[])
{

	if (!SetConsoleTitle("Console Output test")) return 1;

	//###########################################
	//IMPORTANT, when dealing with ReadConsole always give extra size to the
	//input buffer, at least 2 bytes, otherwise the next ReadConsole gets skypped

	while(true)
	{
		char buff[5];
		cls();
		printMenu();
		if (!ReadConsole(hInput, buff, sizeof buff, &bytesRead, NULL))
			std::cout << "Failes to get input" << std::endl;
		// buff = GetInput(hInput, &bytesRead);
		std::cout << buff << std::endl;

		cls();
		GetVector();
		output = {0.0f, 0.0f};
		result = {0.0f, 0.0f};
		if(buff[0] == '1')
		{
			vectorR = rotMatrix(87);
			DisplayResult(vectorR);
		}else if(buff[0] == '2')
		{
			vectorR = scaMatrix(3);
			DisplayResult(vectorR);
		}else if(buff[0] == '3')
		{
			vectorR = shadMatrix(3);
			DisplayResult(vectorR);
		}else if(buff[0] == '4')
		{
			vectorR = disMatrix(3);
			DisplayResult(vectorR);
		}else {
			std::cout << "something go wrong with buff\n";
		}

		char state = Iterator();
		if(state == '1')
		{
			continue;
		}else if(state == '2')
		{
			break;
		}

	}

	cls();
	CloseHandle(hConsole);

	return 0;
}

char GetInput()
{
	char input;
	ReadConsole(hInput, &input, sizeof input, &bytesRead, NULL);
	return input;
}

vectorResult rotMatrix(int angle)
{
	float rad = angle * RAD;
	result.vx = atof(Vinput.x); result.vy = atof(Vinput.y);
	output.vx = result.vx * cos(rad) + -1 *(result.vy * sin(rad));
	output.vy = result.vx * sin(rad) + result.vy * cos(rad);
	return output; 
}

vectorResult scaMatrix(int scale)
{
	result.vx = atof(Vinput.x); result.vy = atof(Vinput.y);
	output.vx = result.vx * scale + result.vy * 0;
	output.vy = result.vx * 0 + result.vy * scale;
	return output;
}

vectorResult shadMatrix(int scale)
{
	result.vx = atof(Vinput.x); result.vy = atof(Vinput.y);
	output.vx = result.vx * 1 + result.vy * scale;
	output.vy = result.vx * 0 + result.vy * 1;
	return output;
}

vectorResult disMatrix(int scale)
{
	result.vx = atof(Vinput.x); result.vy = atof(Vinput.y);
	output.vx = result.vx + scale;
	output.vy = result.vx * 1;
	return output;
}

void GetVector()
{
	const char *tex = "Input the two elements of the vector:\n\n\0";
	DisplaySomething(tex);

	DisplaySomething("Input the x coordinate: \0");
	if(ReadConsole(hInput, Vinput.x, numBuffer, &bytesRead, NULL))
		Vinput.x[bytesRead/sizeof(char)] = '\0';
	DisplaySomething("Input the y coordinate: \0");
	if(ReadConsole(hInput, Vinput.y, numBuffer, &bytesRead, NULL))
		Vinput.y[bytesRead/sizeof(char)] = '\0';

	return;
}

void DisplaySomething(const char *text)
{
	if (!WriteConsole(hConsole, text, strlen(text), &dwBytesWritten, NULL))
	{
		std::cout << "could not display\n";
	}
	 return;
}

void DisplayResult(vectorResult vecResult)
{
	char textres[stringSize] = {' '};
	StringCbPrintf(textres, sizeof textres, "The results are:\n"
											"|%f|\n|%f|\n\0", vecResult.vx, vecResult.vy);

	if (!WriteConsole(hConsole, textres, sizeof textres, &dwBytesWritten, NULL))
	{
		ErrorExit(TEXT("WriteConsole"));
	}

	return;
}

char Iterator()
{
	char state[5];
	const char *text = "\nDo you want to calculate another transformation?: \n"
							"1. Yes\n"
							"2. No\n\0";

	while(true)
	{
		DisplaySomething(text);
		ReadConsole(hInput, state, 5, &bytesRead, NULL);
		state[bytesRead / sizeof(char)] = '\0';
		std::cout << state<< std::endl;

		if( state[0] == '1' || state[0] == '2')
		{
			return state[0];
		}else
		{
			const char *text1 = "\nYou did'nt input a correct number, try again\n\0";
			DisplaySomething(text1);
			continue;
		}
	}
}


void printMenu()
{
	char menu[stringSize] = {' '};
	StringCbPrintf(menu, sizeof(menu),
		 			"1. Get a rotation matrix\n"
	 				"2. Get a scale matrix\n" 
	 				"3. Get a shade matrix\n" 
	 				"4. Get a displacement matrix\n\n"
	 				"Select a number: \n\0");


	if(!WriteConsole(hConsole, menu, strlen(menu), &dwBytesWritten, NULL))
		ErrorExit(TEXT("WriteConsole"));

	return;
}

void ErrorExit(LPCTSTR lpszFunction) 
{ 
    // Retrieve the system error message for the last-error code

    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    // Display the error message and exit the process

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
        (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR)); 
		StringCchPrintf((LPTSTR)lpDisplayBuf, 
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"), 
        lpszFunction, dw, lpMsgBuf); 
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
    ExitProcess(dw); 
}

void cls()
{
	COORD coordScreen = { 0, 0 };    // home for the cursor
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;

    // Get the number of character cells in the current buffer.
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
        return;

    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    // Fill the entire screen with blanks.
    if (!FillConsoleOutputCharacter(hConsole,        // Handle to console screen buffer
                                    (TCHAR)' ',      // Character to write to the buffer
                                    dwConSize,       // Number of cells to write
                                    coordScreen,     // Coordinates of first cell
                                    &cCharsWritten)) // Receive number of characters written
    {
        return;
    }

    // Get the current text attribute.
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
    {
        return;
    }

    // Set the buffer's attributes accordingly.
    if (!FillConsoleOutputAttribute(hConsole,         // Handle to console screen buffer
                                    csbi.wAttributes, // Character attributes to use
                                    dwConSize,        // Number of cells to set attribute
                                    coordScreen,      // Coordinates of first cell
                                    &cCharsWritten))  // Receive number of characters written
    {
        return;
    }

    // Put the cursor at its home coordinates.
    SetConsoleCursorPosition(hConsole, coordScreen);
}
