#pragma once

#include "Config.h"

enum class ELetter_Type : unsigned char
{
	C,	// Speed - ускорение для лески
	G, // Golden - золотая леска, которую нельзя порвать
	H, // Поднимает рыб любого веса
	A, // возможность ловить акул
	E, //(Electrocuting) – Электрошок: Обеспечивает шанс оглушить рыбу на короткое время, увеличивая шанс её поимки.
	O, // отменяет эффекты 
	L, // Жизнь, дополнительный крючек
	R,	// обычное состояние, без эффектов
	Max
};

enum class EPearl_State : unsigned char
{
	idle,
	Birth, // появляется, раcтёт от центра
	Floating_Up,
	Escape
};
//------------------------------------------------------------------------------------------------------------
class AFloating_Pearl : public AGraphics_Object
{
public:
	AFloating_Pearl(int x_pos, int y_pos);

	virtual void Act();
	virtual void Draw(HDC hdc, RECT &paint_area);

	static void Init();
	bool Is_Finished();
	void Set_Pearl_State(EPearl_State new_state);
	EPearl_State Get_State();

private:
	void Draw_Letter_Type(HDC hdc, ELetter_Type letter_type, RECT &rect);
	void Draw_O(HDC hdc, RECT &rect, const AColor &color);
	void Draw_C(HDC hdc, RECT &rect, const AColor &color);
	//void Draw_Line_To(HDC hdc, int x, int y);
	void Floating();
	void Update_Y_Pos();
	ELetter_Type Get_Random_Letter_Type();
	void Redraw();

	EPearl_State Pearl_State;
	ELetter_Type Letter_Type;

	int X_Pos, Y_Pos;
	int Start_Y_Pos;
	int Start_Tick;
	double Speed;

	RECT Pearl_Rect;

	static int All_Letters_Popularity;
	static int Letters_Popularity[ (int)ELetter_Type::Max];  // "Вес" каждой буквы
	static const int Width = AsConfig::Pearl_Size;
	static const int Height = Width;
	static const int Waiting_Timeout = AsConfig::FPS * 2;
	static const int Max_Floating_Height = AsConfig::Sky_Height * AsConfig::Global_Scale;
};
//------------------------------------------------------------------------------------------------------------
