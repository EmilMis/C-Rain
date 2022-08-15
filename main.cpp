#include <iostream>
#include <windows.h>
#include <vector>
#include <random>
#include <conio.h>
#include <stdio.h>

#define nScreenWidth 80
#define nScreenHeight 30

#define nFieldWidth 75
#define nFieldHeight 20

#define rain_droplet pair<int, int>

using namespace std;

vector<rain_droplet> rain;
int ignore[nFieldWidth + 2] = {};

int random_number(int start, int end) {
	random_device device;
	mt19937 generator(device());
	uniform_int_distribution<int> distribution(start, end);
	return distribution(generator);
}

void update_rain() {

	for (int i = 0; i < rain.size(); i++) {
		rain[i].second++;
		if (rain[i].second >= nFieldHeight && !ignore[rain[i].first]) { rain.push_back({random_number(0, nFieldWidth - 2), 0}); ignore[rain[i].first] = 1; }
		if (rain[i].second - nFieldHeight >= 10) { ignore[rain[i].first] = 0; rain.erase(rain.begin() + i); };
	}

}

int main() {

	for (int i = 0; i < 100; i++)rain.push_back({ random_number(0, nFieldWidth - 2), 0 });

	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	for (int x = 0; x < nFieldWidth; x++)
		for (int y = 0; y < nFieldHeight; y++)
			screen[(y + 2) * nScreenWidth + (x + 2)] = L'B';
	WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
	_getch();
	while (1) {

		for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';

		for (auto el : rain) {
			if (el.second - nFieldHeight >= 5)screen[(nFieldHeight)*nScreenWidth + (el.first + 2)] = L'.';
			else if (el.second - nFieldHeight >= 0)screen[(nFieldHeight)*nScreenWidth + (el.first + 2)] = L'_';
			else screen[(el.second + 2) * nScreenWidth + (el.first + 2)] = L'|';
		}

		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
		update_rain();
		Sleep(40);
	}

	//while(1)WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
}
