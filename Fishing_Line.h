#pragma once

#include "Fishing_Hook.h"


// AsLine_Observer
//------------------------------------------------------------------------------------------------------------
class AsLine_Observer
{
public:
	~AsLine_Observer();
	AsLine_Observer();

	virtual bool Mirror() = 0;
	virtual void Act_Line() = 0;

	void Add_Line();
	void Remove_Line();
};
//------------------------------------------------------------------------------------------------------------

enum class ELine_State : unsigned char
{
	Idle,
	Descending,
	//Broken,
	Lifting,
	Finished
};

// AsFishing_Line
//------------------------------------------------------------------------------------------------------------
class AsFishing_Line : public AGraphics_Object
{
public:
	~AsFishing_Line();
	AsFishing_Line(double x_pos, double y_pos);

	virtual void Act();
	virtual void Draw(HDC hdc, RECT &paint_area);

	bool Allowed_Mirror();
	void Active_Line();
	void Move(bool to_left);
	void Set_Position(double x_pos, double y_pos);
	double Get_Curr_Line_Lenght() const;
	ELine_State Get_Line_State();
	AsFishing_Hook *Fishing_Hook;

private:
	void Raise_Or_Lower_Hook(bool is_dropping_down);
	void Double_Draw(HDC hdc, RECT &paint_area);
	void Redraw();

	AsAngle_Math Angle_Math;
	ELine_State Line_State;

	double X_Pos;
	double Y_Pos;
	double Line_Length;
	double Speed;

	RECT Line_Rect, Prev_Line_Rect;

	static const int Start_Lenght = 6;
	static const int Line_Width = 3;
	static const int Max_Y_Pos = (AsConfig::Max_Sand_Y_Pos + Start_Lenght) * 2;
};
//------------------------------------------------------------------------------------------------------------
