#pragma once

#include "Fisherman.h"

enum class EBoat_Moving_State : unsigned char
{
	Stopping,
	Stop,
	Moving_Left,
	Moving_Right,
	Swing_Boat
};
//------------------------------------------------------------------------------------------------------------
enum class EFluctuation_State : unsigned char
{
	idle,

	//Fluctuation_Transition,
	Fluctuation_Up,
	Fluctuation_Down
};
//------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------
class AsFluctuation
{
public:
	AsFluctuation();

	double Act_Fluctuation();
	double Fluctuation_On_Waves(bool lift_up);
	void Set_State(EFluctuation_State new_state);
	double Set_For_Pos(double flunction_pos);
	double Get_Pos();

private:
	//void Transition_Set_State(EFluctuation_State curr_state);

	int Start_Tick;
	double Fluctuation_Height;
	double Fluctuation_Pos;

	EFluctuation_State Fluctuation;

	static const int Fluctuation_Timeot = AsConfig::FPS / 2;
};
//------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------
class AsBoat : public AGraphics_Object
{
public:
	~AsBoat();
	AsBoat();

	virtual void Act();
	virtual void Draw(HDC hdc, RECT &paint_area);
	
	void Move(bool to_left, bool key_down);

	AsFisherman *Fisherman;

private:
	void Draw_Boat_Base(HDC hdc, RECT &paint_area);
	void Draw_Half_Ellipse(HDC hdc, RECT &paint_area, const AColor &color);
	void Draw_Asymmetric_Trapezoid(HDC hdc, RECT &paint_area, const AColor &color);
	void Draw_Gunwale(HDC hdc, RECT &paint_area);
	void Draw_Boards(HDC hdc, RECT &paint_area);
	void Draw_Horizontal_Boards(HDC hdc, RECT &paint_area);
	void Draw_Vertical_Boards(HDC hdc, RECT &paint_area);
	bool Check_Boat_Bounds();
	void Apply_Flunction();

	void Redraw();

	bool Left_Key_Down, Right_Key_Down;
	double X_Pos, Y_Pos;
	double Speed;

	EBoat_Moving_State Moving;
	AsFluctuation Fluctuation;

	RECT Boat_Rect;

	static const int Width = 28;
	static const int Height = 11;
	static const int Waterboarding = 4 * AsConfig::Global_Scale - 1;
	static const int Fisher_X_Pos = 3 * AsConfig::Global_Scale;
	static const int Fisher_Y_Pos = 7 * AsConfig::Global_Scale + 1;
	static const int Boat_Elipse_Radius = Height;
	static const int Gunwale_Height = 2;
	static const int Gunwale_Width = 55;
	static const int Vertical_Board_Width = Width - 1;
	static const int Vertical_Board_Height = Height - 1;
	static const int Horizontal_Board_Height = 2;
	static const int Start_Y_Pos = AsConfig::Sky_Height + Waterboarding;
	static const double Swing_Height;
};
//------------------------------------------------------------------------------------------------------------