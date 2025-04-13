#include "Carriage.hpp"

int AsCarriage::X = 0;
int AsCarriage::Y = 0;
EColor AsCarriage::Text_Color = EC_White;
EColor AsCarriage::Bg_Color = EC_Black;

//----------------------------------------------------------------------------------------------------
void AsCarriage::Set_Coord(int x, int y)
{
	if (x >= 0 && y >= 0)
	{
		X = x;
		Y = y;
		Set_Console_Cursor_Position();
	}
}

//----------------------------------------------------------------------------------------------------
void AsCarriage::Move(int dx, int dy)
{
	if ((X + dx >= 0) && (Y + dy >= 0))
	{
		X += dx;
		Y += dy;
		Set_Console_Cursor_Position();
	}
}

//----------------------------------------------------------------------------------------------------
void AsCarriage::Set_Color(EColor text_color, EColor bg_color)
{
	Text_Color = text_color;
	Bg_Color = bg_color;
	Set_Console_Text_Color();
}

//----------------------------------------------------------------------------------------------------
void AsCarriage::Set_Color(EColor text_color)
{
	Text_Color = text_color;
	Set_Console_Text_Color();
}

//----------------------------------------------------------------------------------------------------
void AsCarriage::Set_Default()
{
	//	X = Y = 0;
	Hide();

	Text_Color = EC_White;
	Bg_Color = EC_Black;

	Set_Console_Cursor_Position();
	Set_Console_Text_Color();
}

//----------------------------------------------------------------------------------------------------
void AsCarriage::Hide()
{
	CONSOLE_CURSOR_INFO curs = { 0 };
	curs.dwSize = sizeof(curs);
	curs.bVisible = FALSE;
	::SetConsoleCursorInfo(::GetStdHandle(STD_OUTPUT_HANDLE), &curs);
}

//----------------------------------------------------------------------------------------------------
void AsCarriage::Set_Console_Cursor_Position()
{
	COORD coord;
	coord.X = X;
	coord.Y = Y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//----------------------------------------------------------------------------------------------------
void AsCarriage::Set_Console_Text_Color()
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((Bg_Color << 4) | Text_Color));
}
