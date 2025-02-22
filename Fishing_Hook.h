#pragma once

#include "Config.h"

// APoint
//------------------------------------------------------------------------------------------------------------
struct APoint
{
	double x_pos;
	double y_pos;
};
//------------------------------------------------------------------------------------------------------------

// AsAngle_Math
//------------------------------------------------------------------------------------------------------------
class AsAngle_Math
{
public:
	APoint Calculate_Rotation(double elem_lenght, double curr_x_pos, double curr_y_pos);

private:
	double Current_Angle();

	static const double Max_Angle;
	static const double Min_Angle;
};
//------------------------------------------------------------------------------------------------------------

// AsFishing_Hook
//------------------------------------------------------------------------------------------------------------
class AsFishing_Hook : public AGraphics_Object
{
public:
	~AsFishing_Hook();
	AsFishing_Hook(double x_pos, double y_pos);

	virtual void Act();
	virtual void Draw(HDC hdc, RECT &paint_area);
	
	virtual bool Is_Mirror_Allowed();

	void Set_Position(double x_pos, double y_pos);
	void Move(bool is_left);

private:
	void Draw_Shank(HDC hdc, RECT &paint_area, RECT &hook_rect);
	void Draw_Hook(HDC hdc, RECT &paint_area, RECT &hook_rect);

	//void Clear(HDC hdc, RECT &paint_area);

	void Redraw();

	double X_Pos;
	double Y_Pos;

	RECT Hook_Rect, Prev_Hook_Rect;

	static const int Shank_Length = 2 * AsConfig::Global_Scale;
	static const int Hook_Radius = Shank_Length;
	static const int Sea_Line = AsConfig::Sky_Height * AsConfig::Global_Scale;
};
//------------------------------------------------------------------------------------------------------------
