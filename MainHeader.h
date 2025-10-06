#pragma once

#ifndef MAIN_HEADER_H
#define MAIN_HEADER_H

#include <string>
#include "MyStructs.h"

using namespace std;

//-----------------------My Headers-----------------------------

int CoordsToIndex(int x, int y);
int CoordsToIndex(Vector2 vec);

Vector2 IndexToCoords(int index);
float Magnitude(Vector2f vec);
Vector2 Normalize(Vector2 vec);
Vector2f Normalizef(Vector2f vec);
Vector2 V2clamp(Vector2 vec, Vector2 VMax, Vector2 VMin);
Vector2f V2clamp(Vector2f vec, Vector2 VMax, Vector2 VMin);
wstring FloatToWString(float value, int precision = 1);
float Time();

bool OnKeyDown(int key);
bool OnKey(char key);
bool OnKey(int key);
bool OnKeyUp(char key);
bool OnKeyUp(int key);

int SineMovement(int amplitude, int frequency);

wstring ClearScreen(Vector2 playerMaxBorders, Vector2 playerMinBorders, wstring Pallet);
wstring DrawUI(wstring strokeToDrawIn, Vector2 playerMaxBorders, Vector2 playerMinBorders, wstring Pallet);
wstring DrawStroke(wstring strokeToDrawIn, wstring strokeToDraw, Vector2 pos);
wstring DrawStroke(wstring strokeToDrawIn, wstring strokeToDraw, Vector2 pos, int Format);
wstring DrawStrokesByRows(wstring strokeToDrawIn, vector<wstring> strokesToDraw, Vector2 pos);
wstring DrawBox(wstring strokeToDrawIn, Vector2f pos1, Vector2f pos2, wchar_t color);
wstring DrawWireBox(wstring strokeToDrawIn, Vector2f pos1, Vector2f pos2, wchar_t color);
wstring DrawCircle(wstring strokeToDrawIn, float size, Vector2f pos, wchar_t color, Vector2 MaxBorder, Vector2 MinBorder);
wstring FastFloodFill(wstring strokeToDrawIn, int width, int height, Vector2 start, wchar_t fillChar);
wstring DrawPixel(wstring strokeToDrawIn, int posX, int posY, int pixelColor);
wstring DrawPixel(wstring strokeToDrawIn, int posX, int posY, wchar_t pixelColor);
wstring DrawLine(wstring strokeToDrawIn, Vector2f pos1, Vector2f pos2, wchar_t pixelColor);

void SetupConsole();
float GetFPS();
COORD GetConsoleSize();
void fastPrint(const std::wstring& text, int x, int y);
void ClearConsoleColor();

#endif
