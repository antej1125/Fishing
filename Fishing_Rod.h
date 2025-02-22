#pragma once

#include "Fishing_Line.h"

enum class ERod_State : unsigned char
{
	Idle,
	Hooking,
	Pull_Fish,
	Reset,
	Finishing
};

// Fishing_Rod
//------------------------------------------------------------------------------------------------------------
class AFishing_Rod : public AGraphics_Object
{
public:
	~AFishing_Rod();
	AFishing_Rod(double x_pos, double y_pos);

	virtual void Act();
	virtual void Draw(HDC hdc, RECT &paint_area);
	
	void Move(bool to_left);
	void Set_X_Pos(double x_pos);

	AsFishing_Line *Fishing_Line;

private:
	void Draw_Rod(HDC hdc, RECT &paint_area);
	void Apply_Coordinates(APoint &curr_point, XFORM &curr_xform);
	void Update_Line_Pos(XFORM &curr_xform);
	void Redraw();

	AsAngle_Math Angle_Math;

	double X_Pos;
	double Y_Pos;

	RECT Rod_Rect;

	static const int Length = 27 * AsConfig::Global_Scale;
	static const int Redraw_Length = 40 * AsConfig::Global_Scale;
};
//------------------------------------------------------------------------------------------------------------
