#pragma once

#include "Sky.h"
#include "Sea.h"
#include "Sea_Floor.h"
#include "Border.h"
#include "Boat.h" 
#include "Fish_Set.h"



//------------------------------------------------------------------------------------------------------------
enum class EKey_Type: unsigned char
{
	Left,
	Right,
	Space,
	Mirror
};
//------------------------------------------------------------------------------------------------------------
enum class EGame_State: unsigned char
{
	Test_Ball,

	Enter_name,
	Play_Level,
	Restart_Level,
	Finish_Level,
	Game_Over,
	Game_Won
};
//------------------------------------------------------------------------------------------------------------
class ASeaweed
{
public:
	ASeaweed(int x_pos, int y_pos);

	void Draw(HDC hdc, RECT &paint_area, const AColor &color);

private:
	void Draw_Curve(HDC hdc, const AColor &color, bool is_left);

	int X_Pos;
	int Y_Pos;
};
//------------------------------------------------------------------------------------------------------------
class AsEngine
{
public:
	AsEngine();

	void Init_Engine(HWND hwnd);
	void Draw_Frame(HDC hdc, RECT &paint_area);
	int On_Key(EKey_Type key_type, bool key_down);
	int On_Timer();
	void On_Char(wchar_t symbol);

	const int Timer_ID;

private:
	void Game_Over();
	void Game_Won();
	void Act();
	void Handle_Message();

	EGame_State Game_State;
	bool Got_Name;
	double Rest_Distance;

	AsBorder Border;
	AsSky Sky;
	AsSea Sea;
	AsBoat Boat;
	AsSea_Floor Sea_Floor;
	AsFish_Set Fish_Set;
	AsFluctuation Flunction;

	std::vector<AGraphics_Object *> Modules;  // UNO; Главные графические объекты (модули) игры
};
//------------------------------------------------------------------------------------------------------------