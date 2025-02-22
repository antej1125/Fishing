#include "Engine.h"



// ASeaweed
//------------------------------------------------------------------------------------------------------------
ASeaweed::ASeaweed(int x_pos, int y_pos)
: X_Pos(x_pos), Y_Pos(y_pos)
{
}
//------------------------------------------------------------------------------------------------------------
void ASeaweed::Draw(HDC hdc, RECT &paint_area, const AColor &color)
{
	// Рисуем кривые
	Draw_Curve(hdc, color, true);
	Draw_Curve(hdc, color, false);
}

//------------------------------------------------------------------------------------------------------------
void ASeaweed::Draw_Curve(HDC hdc, const AColor &color, bool is_left)
{
	const int scale = AsConfig::Global_Scale;
	int width = 9 * scale;
	int height = 4 * scale;

	if (!is_left)
		width *= -1; // Смещение для правой части

	BeginPath(hdc);

	POINT bezier_points[] =
	{
		{ X_Pos, Y_Pos },   
		{ X_Pos + width, Y_Pos },
		{ X_Pos - width, Y_Pos / 4},
		{ X_Pos, Y_Pos }
	};

	PolyBezier(hdc, bezier_points, 4);

	EndPath(hdc);
	color.Select(hdc);
	FillPath(hdc);
}
//------------------------------------------------------------------------------------------------------------




// AsEngine
//------------------------------------------------------------------------------------------------------------
AsEngine::AsEngine()
	: Timer_ID(WM_USER + 1), Game_State(EGame_State::Enter_name), Got_Name(false), Rest_Distance(0.0), Modules{}
{
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Init_Engine(HWND hwnd)
{// Настройка игры при старте

	int index;

	SYSTEMTIME sys_time;
	FILETIME file_time;

	GetSystemTime(&sys_time);
	SystemTimeToFileTime(&sys_time, &file_time);

	srand(file_time.dwLowDateTime);

	AsConfig::Hwnd = hwnd;
	
	SetTimer(AsConfig::Hwnd, Timer_ID, 1000 / AsConfig::FPS, 0);

	Sea_Floor.Init();
	Fish_Set.Init();

	// Modules
	index = 0;

	Modules.push_back(&Border);
	Modules.push_back(&Sky);
	Modules.push_back(&Sea);
	Modules.push_back(Boat.Fisherman);
	Modules.push_back(&Boat);
	Modules.push_back(Boat.Fisherman->Fishing_Rod);
	Modules.push_back(Boat.Fisherman->Fishing_Rod->Fishing_Line);
	Modules.push_back(Boat.Fisherman->Fishing_Rod->Fishing_Line->Fishing_Hook);
	Modules.push_back(&Fish_Set);
	Modules.push_back(&Sea_Floor);
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{// Отрисовка экрана игры
	
	SetGraphicsMode(hdc, GM_ADVANCED);

	for (auto *curr_module : Modules)
		curr_module->Draw(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::On_Key(EKey_Type key_type, bool key_down)
{
	bool mirror_allowed = false;

	switch (key_type)
	{
	case EKey_Type::Left:
		Boat.Move(true, key_down);
		break;

	case EKey_Type::Right:
		Boat.Move(false, key_down);
		break;

 	case EKey_Type::Mirror:
		mirror_allowed = Boat.Fisherman->Fishing_Rod->Fishing_Line->Allowed_Mirror();

		if (! mirror_allowed)
			AsConfig::Mirror_Togle = !AsConfig::Mirror_Togle;
		break;

	case EKey_Type::Space:
		Boat.Fisherman->Fishing_Rod->Fishing_Line->Active_Line();
		break;
	}

	return 0;
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::On_Timer()
{
	++AsConfig::Current_Timer_Tick;

	switch (Game_State)
	{
	case EGame_State::Test_Ball:
		Game_State = EGame_State::Play_Level;
		break;
	}

	Act();
	//Flunction.Act();

	return 0;
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::On_Char(wchar_t symbol)
{
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Game_Over()
{
	Game_State = EGame_State::Game_Over;
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Game_Won()
{
	Game_State = EGame_State::Game_Over;
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Act()
{
	int index = 0;

	// 1. Выполняем все действия
	for (auto *curr_module : Modules)
		curr_module->Act();

	//Handle_Message();
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Handle_Message()
{
	AMessage *message;

	if (AsMessage_Manager::Get_Message(&message) )
	{
		switch (message->Message_Type)
		{
		case EMessage_Type::Mirror_Allowed:
			break;

		case EMessage_Type::Catched_Fish:
			break;

		case EMessage_Type::Level_Done:
			break;

		default:
			AsConfig::Throw();
		}
	}
}
//------------------------------------------------------------------------------------------------------------
