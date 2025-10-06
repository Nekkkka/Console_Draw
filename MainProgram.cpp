#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <thread>
#include <chrono>
#include <ctime>
#include <cmath>
#include <conio.h>
#include <Windows.h>
#include <fcntl.h>
#include <io.h>
#include "MainHeader.h"
#include "MyStructs.h"
#include "MyConsts.h"

using namespace std;

Vector2f PlayerCoordinates;
Vector2f _oldPlayerCoordinates;

float PlayerSpeed = 0.5;

int CursorBlincRate = 5;

//----------------------Main Programm-----------------------------

int main() {
	SetConsoleTitle(L"Concole Draw!");

	SetupConsole();


	wstring map;

	bool CanDraw;

	int BrushIndex = 0;

	int PalletIndex = 0;
	int palletChangeReload = 10;

	int _curcorCounter = 0;

	

	wchar_t CursorSymbol = GRADIENT_PALLET[PalletIndex];
	wchar_t DefaultCursor = '|';

	Vector2f OldPlayerCoordinates = Vector2f::zero;

	Vector2 playerMaxBorders = { SCREEN_SIZE.x - 5, SCREEN_SIZE.y - 5 };
	Vector2 playerMinBorders = { 17, 6 };


	Vector2f ObjetcStartPoint = { 0, 0 };
	bool IsDrawObject = false;


	vector<int> DrawInputKeys = { VK_LBUTTON, VK_RBUTTON };
	vector<char> BrushKeys = { '1', 'B', //Brush
							   '2', 'L', //Line
		                       '3', 'R', //Box
						       '4', 'O', //Circle
							   '5', 'C', //Fill
							   '6', 'I'};//Drop

	map = ClearScreen(playerMaxBorders, playerMinBorders, GRADIENT_PALLET);

	while (true)
	{
		//ClearConsoleColor();

		//-----------------------------------Cursor Setup--------------------------------------------

		if ((OldPlayerCoordinates.x != PlayerCoordinates.x ||
			OldPlayerCoordinates.y != PlayerCoordinates.y) || OnKey(DrawInputKeys[0]) || OnKey(DrawInputKeys[0])) {

			OldPlayerCoordinates = PlayerCoordinates;

			CursorSymbol = GRADIENT_PALLET[1];

			_curcorCounter = -CursorBlincRate * 2;
		}
		else {
			if (_curcorCounter < CursorBlincRate) {
				_curcorCounter++;
			}
			else {
				_curcorCounter = 0;

				if (CursorSymbol == ' ') CursorSymbol = GRADIENT_PALLET[1];
				else CursorSymbol = ' ';
			}
		}

		Vector2 localPlayerMaxBorders = { playerMaxBorders.x - 1, playerMaxBorders.y - 1 };
		Vector2 localPlayerMinBorders = { playerMinBorders.x + 1, playerMinBorders.y + 1 };

		POINT p;

		if (GetCursorPos(&p)) {
			ScreenToClient(GetConsoleWindow(), &p);

			int MouseX = p.x / SYMBOL_SIZE.x;
			int MouseY = p.y / SYMBOL_SIZE.y;

			PlayerCoordinates.x = MouseX;
			PlayerCoordinates.y = MouseY;
		}

		if (OnKey(VK_RIGHT)) {
			if (palletChangeReload <= 0) {
				palletChangeReload = 15;
				PalletIndex++;
			}
			else palletChangeReload--;
		}
		if (OnKey(VK_LEFT)) {
			if (palletChangeReload <= 0) {
				palletChangeReload = 15;
				PalletIndex--;
			}
			else palletChangeReload--;
		}
		if (OnKey(DrawInputKeys[0])) {
			if (PlayerCoordinates.x >= playerMinBorders.x + 13 &&
				PlayerCoordinates.x <= playerMinBorders.x + 13 + GRADIENT_PALLET.size() - 1 &&
				PlayerCoordinates.y >= 3 && PlayerCoordinates.y <= 4) {
				PalletIndex = PlayerCoordinates.x - (playerMinBorders.x + 13);
			}
		}

		if (PalletIndex > GRADIENT_PALLET.size() - 1 && PalletIndex > 0) {
			PalletIndex = GRADIENT_PALLET.size() - 1;
		}
		if (PalletIndex < 0) {
			PalletIndex = 0;
		}

		CanDraw = false;
		if ((PlayerCoordinates.x < playerMaxBorders.x + 2 && PlayerCoordinates.y < playerMaxBorders.y + 1) &&
			(PlayerCoordinates.x > playerMinBorders.x - 2 && PlayerCoordinates.y > playerMinBorders.y - 1)) {
			CanDraw = true;
		}

		PlayerCoordinates = V2clamp(PlayerCoordinates, localPlayerMaxBorders, localPlayerMinBorders);

		for (char key : BrushKeys) {
			if (OnKey(key)) {
				switch (key)
				{
				case ('1'): BrushIndex = 0; break;
				case ('B'): BrushIndex = 0; break;
				case ('2'): BrushIndex = 1; break;
				case ('L'): BrushIndex = 1; break;
				case ('3'): BrushIndex = 2; break;
				case ('R'): BrushIndex = 2; break;
				case ('4'): BrushIndex = 3; break;
				case ('O'): BrushIndex = 3; break;
				case ('5'): BrushIndex = 4; break;
				case ('C'): BrushIndex = 4; break;
				case ('6'): BrushIndex = 5; break;
				case ('I'): BrushIndex = 5; break;
				default:
					break;
				}
			}
		}

		//-----------------------------------Canvas Drawing--------------------------------------------

		if (OnKey('X')) map = ClearScreen(playerMaxBorders, playerMinBorders, GRADIENT_PALLET);

		wstring cursorMap = map;


		int temporalBrushIndex = BrushIndex;

		if (OnKey(VK_MENU)) {
			temporalBrushIndex = 5;
		}

		if (CanDraw){
			switch (temporalBrushIndex) {
			case (0):
				if (OnKey(DrawInputKeys[0])) {
					map = DrawLine(map, PlayerCoordinates, OldPlayerCoordinates, GRADIENT_PALLET[PalletIndex]);
				}
				if (OnKey(DrawInputKeys[1])) {
					map = DrawLine(map, PlayerCoordinates, OldPlayerCoordinates, ' ');
				}
			break;

			case (1):

				//--------------------To Draw Line-------------------------------
				if (OnKey(DrawInputKeys[0]) && !IsDrawObject) {
					ObjetcStartPoint = PlayerCoordinates;
					IsDrawObject = true;
				}
				if (!OnKey(DrawInputKeys[0]) && IsDrawObject) {
					map = DrawLine(map, PlayerCoordinates, ObjetcStartPoint, GRADIENT_PALLET[PalletIndex]);
					IsDrawObject = false;
				}
				//--------------------To Erase -------------------------------
				if (OnKey(DrawInputKeys[1])) {
					map = DrawLine(map, PlayerCoordinates, OldPlayerCoordinates, ' ');
				}

				if (IsDrawObject) cursorMap = DrawLine(cursorMap, PlayerCoordinates, ObjetcStartPoint, GRADIENT_PALLET[PalletIndex]);
			break;

			case (2):

				//--------------------To Draw Cube-------------------------------
				if (OnKey(DrawInputKeys[0]) && !IsDrawObject) {
					ObjetcStartPoint = PlayerCoordinates;
					IsDrawObject = true;
				}
				if (!OnKey(DrawInputKeys[0]) && IsDrawObject) {
					map = DrawBox(map, PlayerCoordinates, ObjetcStartPoint, GRADIENT_PALLET[PalletIndex]);
					IsDrawObject = false;
				}
				//--------------------To Erase -------------------------------
				if (OnKey(DrawInputKeys[1])) {
					map = DrawLine(map, PlayerCoordinates, OldPlayerCoordinates, ' ');
				}

				if (IsDrawObject) {
					cursorMap = DrawWireBox(map, PlayerCoordinates, ObjetcStartPoint, GRADIENT_PALLET[PalletIndex]);
				}
			break;
			
			case (3):

				//--------------------To Draw Circle-------------------------------
				if (OnKey(DrawInputKeys[0]) && !IsDrawObject) {
					ObjetcStartPoint = PlayerCoordinates;
					IsDrawObject = true;
				}
				if (!OnKey(DrawInputKeys[0]) && IsDrawObject) {
					map = DrawCircle(map, PlayerCoordinates.y - ObjetcStartPoint.y, ObjetcStartPoint, GRADIENT_PALLET[PalletIndex], playerMaxBorders, playerMinBorders);
					IsDrawObject = false;
				}
				//--------------------To Erase -------------------------------
				if (OnKey(DrawInputKeys[1])) {
					map = DrawLine(map, PlayerCoordinates, OldPlayerCoordinates, ' ');
				}

				if (IsDrawObject) {
					PlayerCoordinates = V2clamp(PlayerCoordinates, Vector2(ObjetcStartPoint.x, PlayerCoordinates.y), Vector2(ObjetcStartPoint.x, PlayerCoordinates.y));
					cursorMap = DrawCircle(map, PlayerCoordinates.y - ObjetcStartPoint.y, ObjetcStartPoint, GRADIENT_PALLET[PalletIndex], playerMaxBorders, playerMinBorders);
				}
			break;

			case (4):

				//---------------------To Fill-------------------------------
				if (OnKey(DrawInputKeys[0]) && !IsDrawObject) {
					map = FastFloodFill(map, SCREEN_SIZE.x, SCREEN_SIZE.y, Vector2(PlayerCoordinates.x, PlayerCoordinates.y), GRADIENT_PALLET[PalletIndex]);

					map = DrawUI(map, playerMaxBorders, playerMinBorders, GRADIENT_PALLET);

					IsDrawObject = true;
				}
				if (!OnKey(DrawInputKeys[0]) && IsDrawObject) {
					IsDrawObject = false;
				}
				//--------------------To Erase -------------------------------
				if (OnKey(DrawInputKeys[1])) {
					map = DrawLine(map, PlayerCoordinates, OldPlayerCoordinates, ' ');
				}
			break;

			case (5):

				//--------------------To Take Sample-------------------------------
				if (OnKey(DrawInputKeys[0]) && !IsDrawObject) {
					size_t index = GRADIENT_PALLET.find(map[CoordsToIndex(PlayerCoordinates.x, PlayerCoordinates.y)]);

					if (index != wstring::npos) {
						PalletIndex = index;
					}

					IsDrawObject = true;
				}
				if (!OnKey(DrawInputKeys[0]) && IsDrawObject) {
					IsDrawObject = false;
				}
				//--------------------To Erase -------------------------------
				if (OnKey(DrawInputKeys[1])) {
					map = DrawLine(map, PlayerCoordinates, OldPlayerCoordinates, ' ');
				}
			break;

			default:
			break;
			}
		}

		cursorMap = DrawPixel(cursorMap, 1, 2 + temporalBrushIndex, L'-');

		cursorMap = DrawStroke(cursorMap, L"Cursor pos: (" + FloatToWString(PlayerCoordinates.x, 1) + L", " + FloatToWString(PlayerCoordinates.y, 1) + L")", Vector2(2, SCREEN_SIZE.y - 3)); //---Cursor Position---

		cursorMap = DrawStroke(cursorMap, L"FPS: " + FloatToWString(GetFPS(), 1), Vector2(2, SCREEN_SIZE.y - 2)); //---Frames per second---

		if (!OnKey(DrawInputKeys[0]) && !OnKey(DrawInputKeys[1])) {

			cursorMap = DrawPixel(cursorMap, PlayerCoordinates.x, PlayerCoordinates.y, CursorSymbol);
			cursorMap = DrawPixel(cursorMap, playerMinBorders.x + 13 + PalletIndex, 3, DefaultCursor);

			fastPrint(cursorMap, 0, 0);
		}
		else {
			cursorMap = DrawPixel(cursorMap, PlayerCoordinates.x, PlayerCoordinates.y, CursorSymbol);
			cursorMap = DrawPixel(cursorMap, playerMinBorders.x + 13 + PalletIndex, 3, DefaultCursor);

			fastPrint(cursorMap, 0, 0);
		}

		//-----------------------------------End of loop and Delay--------------------------------------
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	return 0;
}