#pragma once

#include "Config.h";

enum class EFish_State : unsigned char
{
	Idle,

	Move_Left,
	Move_Right,
	Emmiting,
	On_Hook,
	Escape
};

class AFish /*: public ACollision*/
{
public:
	 AFish(int x_pos, int y_pos);
	 
	 virtual void Act();
	 virtual void Draw(HDC hdc, RECT &paint_area);
	 
	 void Set_State(EFish_State new_state);
	 void Set_Direction();

private:
	bool Is_Finished();
	void Draw_Fish(HDC hdc, RECT &rect);
	void Draw_Mouth(HDC hdc, RECT &rect);
	void Draw_Eye(HDC hdc, RECT &rect);
	void Draw_Tail(HDC hdc, RECT &rect);
	void Draw_Gills(HDC hdc, RECT rect);
	void Draw_Fin(HDC hdc, RECT rect);
	void Draw_Back_Fin(HDC hdc, RECT &rect);
	void Draw_Belly_Fin(HDC hdc, RECT &rect);
	void Draw_Pelvic_Fin_Front(HDC hdc, RECT &rect);
	void Draw_Pelvic_Fin_Back(HDC hdc, RECT &rect);
	void Update_Move_Diraction(EFish_State state);
	void Redraw();

	int X_Pos;
	int Y_Pos;
	double Speed;

	RECT Fish_Rect, Colliion_Rect;
	EFish_State Fish_State, Prev_Fish_State;

	static const int Width = 15 * AsConfig::Global_Scale;
	static const int Height = 20;
	static const int Half_Height = Height / 2;
	static const int Mouth_Size = 6;
	static const int Eye_Size = 9;
	static const int Tail_Size = 15;
	static const int Offset = 5;
	static const int Fin_Size = 17;
	static const int Gills_Height = 14;
	static const int Belly_Fin_Size = Gills_Height;
	static const int Pelvic_Fin_Size = 12;
};