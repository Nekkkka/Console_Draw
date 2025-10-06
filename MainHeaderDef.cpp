#include <vector>
#include <string>
#include <stack>
#include <sstream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <ctime>
#include <cmath>
#include <conio.h>
#include <Windows.h>
#include "MainHeader.h"
#include "MyStructs.h"
#include "MyConsts.h"

using namespace std;

//-----------------------My Functions-----------------------------

int CoordsToIndex(int x, int y) {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

	int offsetX = csbi.srWindow.Left;
	int offsetY = csbi.srWindow.Top;

	return (x + offsetX) + SCREEN_SIZE.x * (y + offsetY);
}

int CoordsToIndex(Vector2 vec) {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

	int offsetX = csbi.srWindow.Left;
	int offsetY = csbi.srWindow.Top;

	return (vec.x + offsetX) + SCREEN_SIZE.x * (vec.y + offsetY);
}

Vector2 IndexToCoords(int index) {
	Vector2 result = { 0, 0 };
	int x = index;
	while (x > SCREEN_SIZE.x)
	{
		x -= SCREEN_SIZE.x;
	}

	result.x = x;
	result.y = index / SCREEN_SIZE.x - 1;

	return result;
}

Vector2 Normalize(Vector2 vec) {
	Vector2 _vec = vec;
	int max = 1;

	if (vec.x > vec.y) max = vec.x;
	else max = vec.y;

	if (max == 0) max = 1;

	_vec.x /= max;
	_vec.y /= max;

	return _vec;
}

float Magnitude(Vector2f vec) {
	return abs(vec.x + vec.y);
}

Vector2f Normalizef(Vector2f vec) {
	Vector2f _vec = vec;
	int max = 1;

	if (vec.x > vec.y) max = vec.x;
	else max = vec.y;

	if (max == 0) max = 1;

	_vec.x /= max;
	_vec.y /= max;

	return _vec;
}

Vector2 V2clamp(Vector2 vec, Vector2 VMax, Vector2 VMin) {
	Vector2 _vec = vec;
	
	if (vec.x > VMax.x) _vec.x = VMax.x;
	if (vec.y > VMax.y) _vec.y = VMax.y;

	if (vec.x < VMin.x) _vec.x = VMin.x;
	if (vec.x < VMin.y) _vec.y = VMin.y;

	return _vec;
}

Vector2f V2clamp(Vector2f vec, Vector2 VMax, Vector2 VMin) {
	Vector2f _vec = vec;

	if (vec.x > VMax.x) _vec.x = VMax.x;
	if (vec.y > VMax.y) _vec.y = VMax.y;

	if (vec.x < VMin.x) _vec.x = VMin.x;
	if (vec.y < VMin.y) _vec.y = VMin.y;

	return _vec;
}

wstring FloatToWString(float value, int precision) {
	wchar_t buffer[32];
	swprintf(buffer, sizeof(buffer) / sizeof(wchar_t), L"%.*f", precision, value);
	return buffer;
}

float Time() {
	auto now = chrono::high_resolution_clock::now();
	auto nanos = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()).count();

	return nanos;
}

bool OnKeyDown(int key) {
	SHORT state = GetAsyncKeyState(key);
	
	if (state & 0x0001) return true;
	else return false;
}
bool OnKey(char key) {
	SHORT state = GetAsyncKeyState(key);

	if (state & 0x8000) return true;
	else return false;
}

bool OnKey(int key) {
	SHORT state = GetAsyncKeyState(key);

	if (state & 0x8000) return true;
	else return false;
}

bool OnKeyUp(char key) {
	SHORT state = GetAsyncKeyState(key);

	if (state & 0x0001) return true;
	else return false;
}

bool OnKeyUp(int key) {
	SHORT state = GetAsyncKeyState(key);

	if (state & 0x0001) return true;
	else return false;
}

int SineMovement(int amplitude, int frequency) {
	return (sin(Time() / frequency) * amplitude);
}

bool IsFitToBox(Vector2 point, Vector2 MaxBorder, Vector2 MinBorder) {
	return (point.x < MaxBorder.x && point.x > MinBorder.x &&
			point.y < MaxBorder.y - 1 && point.y >= MinBorder.y);
}

//------------------------My Methods------------------------------

wstring DrawStroke(wstring strokeToDrawIn, wstring strokeToDraw, Vector2 pos) {
	for (int i = 0; i < strokeToDraw.size(); i++)
	{
		strokeToDrawIn[CoordsToIndex(i + pos.x, pos.y)] = strokeToDraw[i];
	}

	return strokeToDrawIn;
}

wstring DrawStroke(wstring strokeToDrawIn, wstring strokeToDraw, Vector2 pos, int Format) {
	switch (Format){
		case (0) :
			for (int i = 0; i < strokeToDraw.size(); i++)
			{
				strokeToDrawIn[CoordsToIndex(i + pos.x, pos.y)] = strokeToDraw[i];
			}
		break;

		case (1) :
			for (int i = 0; i < strokeToDraw.size(); i++)
			{
				strokeToDrawIn[CoordsToIndex((SCREEN_SIZE.x / 2) - (strokeToDraw.size() / 2) + i, pos.y)] = strokeToDraw[i];
			}

		default:
	break;
	}

	return strokeToDrawIn;
}

wstring DrawStrokesByRows(wstring strokeToDrawIn, vector<wstring> strokesToDraw, Vector2 pos) {
	int i = 0;
	for (wstring str : strokesToDraw) {
		strokeToDrawIn = DrawStroke(strokeToDrawIn, str, Vector2(pos.x, pos.y + i));
		i++;
	}

	return strokeToDrawIn;
}

wstring ClearScreen(Vector2 playerMaxBorders, Vector2 playerMinBorders, wstring Pallet) {
	wstring _map;

	for (size_t i = 0; i < SCREEN_SIZE.y; i++)
	{
		for (size_t i = 0; i < SCREEN_SIZE.x; i++)
		{
			_map += L' ';
		}
	}

	_map = DrawUI(_map, playerMaxBorders, playerMinBorders, Pallet);

	return _map;
}

wstring DrawUI(wstring strokeToDrawIn, Vector2 playerMaxBorders, Vector2 playerMinBorders, wstring Pallet) {
	
	wstring _map = strokeToDrawIn;
	
	for (int y = 0; y < SCREEN_SIZE.y; y++)
	{
		for (int x = 0; x < SCREEN_SIZE.x; x++)
		{
			if ((x == playerMaxBorders.x || x == playerMinBorders.x) &&
				(y <= playerMaxBorders.y && y >= playerMinBorders.y)) _map[CoordsToIndex(x, y)] = '|';

			if ((y == playerMaxBorders.y || y == playerMinBorders.y) &&
				(x <= playerMaxBorders.x && x >= playerMinBorders.x)) _map[CoordsToIndex(x, y)] = '-';
		}
	}

	wstring Header = L"Console Draw!";

	wstring tut1 = L"|| Controls: WASD - to move the cursor || LMB - to draw || RBM - to erase || X - to clear canvas ||";
	wstring tut2 = L"||   Symbol: _____________________________________________________________|| Arrows - to choose  ||";
	wstring tut3 = L"||  Pallete: " + Pallet;

	//-------------------------------Tutorial and Pallet--------------------------------------------------

	_map = DrawStroke(_map, Header, Vector2(0, 1), 1);

	_map = DrawStroke(_map, tut1, Vector2(playerMinBorders.x, 2));
	_map = DrawStroke(_map, tut2, Vector2(playerMinBorders.x, 3));
	_map = DrawStroke(_map, tut3, Vector2(playerMinBorders.x, 4));

	//--------------------------------------Brushes--------------------------------------------------

	wstring borders =  L"|===========|";
	wstring brush =    L"| Brush - 1 |";
	wstring Line =     L"|  Line - 2 |";
	wstring Cube =     L"|  Cube - 3 |";
	wstring Circle =   L"|Circle - 4 |";
	wstring Fill =     L"|  Fill - 5 |";
	wstring Pipette =  L"|  Drop - 6 |";
	wstring borders2 = L"|===========|";

	vector<wstring> brushes = { borders, brush, Line, Cube, Circle, Fill, Pipette, borders2 };

	_map = DrawStrokesByRows(_map, brushes, Vector2(2, 1));

	return _map;
}

wstring DrawBox(wstring strokeToDrawIn, Vector2f pos1, Vector2f pos2, wchar_t color) {
	
	int left = min(pos1.x, pos2.x);
	int right = max(pos1.x, pos2.x);
	int top = min(pos1.y, pos2.y);
	int bottom = max(pos1.y, pos2.y);

	for (int y = 0; y < SCREEN_SIZE.y; y++)
	{
		for (int x = 0; x < SCREEN_SIZE.x; x++)
		{
			Vector2 p = { x, y };

			if (p.x >= left && p.x <= right && p.y >= top && p.y <= bottom)
				strokeToDrawIn = DrawPixel(strokeToDrawIn, p.x, p.y, color);
		}
	}

	return strokeToDrawIn;
}

wstring DrawWireBox(wstring strokeToDrawIn, Vector2f pos1, Vector2f pos2, wchar_t color) {
	
	int left = min(pos1.x, pos2.x);
	int right = max(pos1.x, pos2.x);
	int top = min(pos1.y, pos2.y);
	int bottom = max(pos1.y, pos2.y);

	for (int y = 0; y < SCREEN_SIZE.y; y++)
	{
		for (int x = 0; x < SCREEN_SIZE.x; x++)
		{
			Vector2 p = { x, y };

			bool onTop = (p.y == top) && (p.x >= left && p.x <= right);
			bool onBottom = (p.y == bottom) && (p.x >= left && p.x <= right);
			bool onLeft = (p.x == left) && (p.y >= top && p.y <= bottom);
			bool onRight = (p.x == right) && (p.y >= top && p.y <= bottom);

			if (onTop || onBottom || onLeft || onRight)
				strokeToDrawIn = DrawPixel(strokeToDrawIn, p.x, p.y, color);
		}
	}

	return strokeToDrawIn;
}

wstring DrawCircle(wstring strokeToDrawIn, float size, Vector2f pos, wchar_t color, Vector2 MaxBorder, Vector2 MinBorder) {
	for (size_t i = 0; i < strokeToDrawIn.size() - 1; i++)
	{
		if (IsFitToBox(IndexToCoords(i), MaxBorder, MinBorder)) {
			int _circleEquasion = pow((IndexToCoords(i).x - pos.x) * SYMBOL_OFFSET, 2) + pow(IndexToCoords(i).y - pos.y, 2);

			if (_circleEquasion <= pow(size, 2)) {
				int index = _circleEquasion / (size * size) * (GRADIENT_PALLET.size() - 1);

				strokeToDrawIn[i] = color;
			}
		}		
	}

	return strokeToDrawIn;
}

wstring FastFloodFill(wstring strokeToDrawIn, int width, int height, Vector2 start, wchar_t pixelColor) {
	if (start.x < 0 || start.y < 0 || start.x >= width || start.y >= height)
		return strokeToDrawIn;

	const wchar_t target = strokeToDrawIn[CoordsToIndex(start.x, start.y)];
	if (target == pixelColor)
		return strokeToDrawIn;

	stack<Vector2> s;
	s.push(start);

	while (!s.empty()) {
		Vector2 p = s.top();
		s.pop();

		int y = p.y;
		int xLeft = p.x;
		int xRight = p.x;

		// двигаемся влево
		while (xLeft >= 0 && strokeToDrawIn[CoordsToIndex(xLeft, y)] == target)
			xLeft--;
		xLeft++;

		// двигаемся вправо
		while (xRight < width && strokeToDrawIn[CoordsToIndex(xRight, y)] == target)
			xRight++;
		xRight--;

		// заливаем строку
		for (int x = xLeft; x <= xRight; x++)
			strokeToDrawIn[CoordsToIndex(x, y)] = pixelColor;

		// Проверяем строки сверху и снизу
		for (int ny = y - 1; ny <= y + 1; ny += 2) {
			if (ny < 0 || ny >= height) continue;
			int x = xLeft;
			while (x <= xRight) {
				bool inSpan = false;
				while (x < width && strokeToDrawIn[CoordsToIndex(x, ny)] == target) {
					if (!inSpan) {
						s.push({ x, ny });
						inSpan = true;
					}
					x++;
				}
				x++;
			}
		}
	}

	return strokeToDrawIn;
}

wstring DrawPixel(wstring strokeToDrawIn, int posX, int posY, int pixelColor) {
	strokeToDrawIn[CoordsToIndex(posX, posY)] = GRADIENT_PALLET[pixelColor];

	return strokeToDrawIn;
}

wstring DrawPixel(wstring strokeToDrawIn, int posX, int posY, wchar_t pixelColor) {
	strokeToDrawIn[CoordsToIndex(posX, posY)] = pixelColor;

	return strokeToDrawIn;
}

bool IsPixelOnLine(Vector2f point, Vector2f pos1, Vector2f pos2) {
	
	Vector2 localPoint = { (int)round(point.x), (int)round(point.y) };
	Vector2 localPos1 = { (int)round(pos1.x), (int)round(pos1.y) };
	Vector2 localPos2 = { (int)round(pos2.x), (int)round(pos2.y) };
	
	while (true) {
		if (localPoint == localPos1) return true;
		if (localPos1 == localPos2) break;

		localPos1 += Normalize(localPos2 - localPos1);
	}

	return false;
}

wstring DrawLine(wstring strokeToDrawIn, Vector2f pos1, Vector2f pos2, wchar_t pixelColor) {
	int x0 = (int)round(pos1.x);
	int y0 = (int)round(pos1.y);
	int x1 = (int)round(pos2.x);
	int y1 = (int)round(pos2.y);

	int dx = abs(x1 - x0);
	int dy = -abs(y1 - y0);
	int sx = (x0 < x1) ? 1 : -1;
	int sy = (y0 < y1) ? 1 : -1;
	int err = dx + dy;

	while (true) {
		strokeToDrawIn[CoordsToIndex(x0, y0)] = pixelColor;

		if (x0 == x1 && y0 == y1) break;
		int e2 = 2 * err;
		if (e2 >= dy) { err += dy; x0 += sx; }
		if (e2 <= dx) { err += dx; y0 += sy; }
	}

	return strokeToDrawIn;
}

void SetupConsole() {

	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);

	// === 1. Размер буфера и окна ===

	SMALL_RECT windowSize;
	windowSize.Left = 0;
	windowSize.Top = 0;
	windowSize.Right = 1;
	windowSize.Bottom = 1;
	SetConsoleWindowInfo(hOut, TRUE, &windowSize);

	COORD bufferSize;
	bufferSize.X = SCREEN_SIZE.x;
	bufferSize.Y = SCREEN_SIZE.y;
	SetConsoleScreenBufferSize(hOut, bufferSize);

	windowSize.Left = 0;
	windowSize.Top = 0;
	windowSize.Right = SCREEN_SIZE.x - 1;
	windowSize.Bottom = SCREEN_SIZE.y - 1;
	SetConsoleWindowInfo(hOut, TRUE, &windowSize);

	// 2. Определить размер экрана
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// === 2. Запретить растягивание и разворачивание ===
	HWND hwnd = GetConsoleWindow();
	LONG style = GetWindowLong(hwnd, GWL_STYLE);
	style &= ~(WS_MAXIMIZEBOX | WS_SIZEBOX); // убираем кнопки "развернуть" и растягивание
	SetWindowLong(hwnd, GWL_STYLE, style);
	style &= ~(WS_SIZEBOX | WS_MAXIMIZEBOX);

	// 3. Определить размеры консольного окна
	RECT consoleRect;
	GetWindowRect(hwnd, &consoleRect);
	int consoleWidth = consoleRect.right - consoleRect.left;
	int consoleHeight = consoleRect.bottom - consoleRect.top;

	// 4. Вычислить новые координаты
	int x = (screenWidth - consoleWidth) / 2;
	int y = (screenHeight - consoleHeight) / 2;

	// 5. Переместить окно
	SetWindowPos(hwnd, NULL, x, y, 0, 0, SWP_NOSIZE);

	// === 3. Отключить Quick Edit (выделение мышкой) ===
	DWORD prev_mode;
	GetConsoleMode(hIn, &prev_mode);
	prev_mode &= ~ENABLE_QUICK_EDIT_MODE; // убрать режим выделения
	prev_mode &= ~ENABLE_INSERT_MODE;
	prev_mode |= ENABLE_EXTENDED_FLAGS;   // обязательно нужно для отключения
	prev_mode |= ENABLE_MOUSE_INPUT;
	SetConsoleMode(hIn, prev_mode);

	// === 4. Скрыть курсор ===
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(hOut, &cursorInfo);
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(hOut, &cursorInfo);
}

float GetFPS() {
	static int frameCount = 0;
	static float fps = 0.0f;
	static DWORD lastTime = GetTickCount();

	frameCount++;

	DWORD currentTime = GetTickCount();
	DWORD delta = currentTime - lastTime;

	// обновляем FPS каждые 1000 мс (1 секунда)
	if (delta >= 1000) {
		fps = frameCount * 1000.0f / delta;
		frameCount = 0;
		lastTime = currentTime;
	}

	return fps;
}

// быстрый вывод
void fastPrint(const std::wstring& text, int x, int y) {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { (SHORT)x, (SHORT)y };
	DWORD written;

	WriteConsoleOutputCharacterW(hOut, text.c_str(), (DWORD)text.size(), pos, &written);
}

void ClearConsoleColor(vector<Vector2> poses) {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD written;

	WORD color = FOREGROUND_GREEN | FOREGROUND_RED;

	for (Vector2 v : poses) {

		COORD pos = { v.x, v.y };

		WriteConsoleOutputAttribute(hOut, &color, 1, pos, &written);
	}
}