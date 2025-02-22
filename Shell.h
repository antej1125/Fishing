#pragma once

#include "Floating_Pearl.h"

enum class EShell_State : unsigned char
{
	Empty,

	Open,
	Opening,
	Create_Pearl,
	Emit_Pearl,
	Closing,
	Closed,
};

//------------------------------------------------------------------------------------------------------------
class AShell : public AGraphics_Object
{
public:
	~AShell();
	AShell(int x_pos, int y_pos);

	virtual void Act();
	virtual void Draw(HDC hdc, RECT &paint_area);
	
	AFloating_Pearl *Pearl;

private:
	void Draw_Shell(HDC hdc, RECT &paint_area);
	void Draw_Base_Shell(HDC hdc, RECT &paint_area);
	void Draw_Cap(HDC hdc, RECT &paint_area);
	void Draw_Pearl(HDC hdc, RECT &paint_area);
	void Draw_Shell_Curves(HDC hdc, RECT &rect, const AColor &color, bool use_mirror);
	void Delay_State_Change();
	void Emit_At_Pearl();
	void Update_State(EShell_State shell_state);
	void Redraw();

	EShell_State Shell_State, Prev_Shell_State;
	int X_Pos, Y_Pos;
	int Start_Tick;
	double Cap_Height;
	double Shell_Cap_Angle;
	RECT Shell_Rect;

	static const int Offset = 4;
	static const int Base_Width = 21 * AsConfig::Global_Scale;
	static const int Base_Height = 10 * AsConfig::Global_Scale;
	static const int Cap_Width = 19 * AsConfig::Global_Scale;
	static const int Max_Closing_Height = 5 * AsConfig::Global_Scale;
	static const int Curve_Size = 7;
	static const int Shell_Timeout = AsConfig::FPS * 2;
	static const int Waiting_Timeout = AsConfig::FPS * 4;
	
	static const double Max_Cap_Height;
};
//------------------------------------------------------------------------------------------------------------
