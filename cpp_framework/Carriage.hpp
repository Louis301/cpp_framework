#pragma once
#include <windows.h>

enum EColor
{
	EC_Black,
	EC_Dark_Blue,
	EC_Dark_Green,
	EC_Dark_Cyan,
	EC_Dark_Red,
	EC_Dark_Pink,
	EC_Dark_Yellow,
	EC_Grey,
	EC_Dark_Grey,
	EC_Blue,
	EC_Green,
	EC_Cyan,
	EC_Red,
	EC_Pink,
	EC_Yellow,
	EC_White
};

//----------------------------------------------------------------------------------------------------
class AsCarriage
{
public:
	static void Set_Coord(int x, int y);
	static void Move(int dx, int dy);
	static void Set_Color(EColor text_color, EColor bg_color);
	static void Set_Default();
	static void Hide();

	static void Set_Console_Cursor_Position();

	static int X, Y;

private:
	static EColor Bg_Color, Text_Color;

	
	static void Set_Console_Text_Color();
};


