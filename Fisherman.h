#pragma once

#include "Fishing_Rod.h"

enum class EFisherman_Move_State : unsigned char
{
	Stopping,
	Stop,
	Moving_Left,
	Moving_Right
};

//------------------------------------------------------------------------------------------------------------
class AsFisherman : public AGraphics_Object
{
public:
	~AsFisherman();
	AsFisherman(double x_pos, double y_pos);

	virtual void Act();
	virtual void Draw(HDC hdc, RECT &paint_area);
	
	void Move(bool to_left);
	
	void Set_X_Pos(double x_pos);
	void Clear(HDC hdc);

	AFishing_Rod *Fishing_Rod;

private:
	void Draw_Body(HDC hdc, RECT &rect);
	void Draw_Head(HDC hdc, RECT &rect);
	void Draw_Hand(HDC hdc, RECT &rect);
	void Draw_Cap(HDC hdc, RECT &head_rect);
	void Draw_Glasses(HDC hdc, RECT &head_rect);
	void Draw_Face_Part(HDC hdc, RECT &head_rect);
	void Redraw();

	double X_Pos, Y_Pos;

	EFisherman_Move_State Moving;
	
	RECT Fisherman_Rect;

	static const int Body_Width = 8 * AsConfig::Global_Scale;
	static const int Body_Height = 12 * AsConfig::Global_Scale;
	static const int Head_Width = 8 * AsConfig::Global_Scale + 1;
	static const int Head_Height = 7 * AsConfig::Global_Scale + 1;
	static const int Hand_Width = 3 * AsConfig::Global_Scale + 1;
	static const int Hand_Height = 2 * AsConfig::Global_Scale + 1;
	static const int Hand_Y_Offset = Body_Width - 1;
	static const int Cap_Width = 4;
	static const int Glasses_Width = 2 * AsConfig::Global_Scale + 1;
	static const int Glasses_Height = AsConfig::Global_Scale + 1;
	static const int Face_Width = 7 * AsConfig::Global_Scale - 1;
	static const int Face_Height = 5 * AsConfig::Global_Scale - 1;
	static const int Offset = 2;
	static const int Rod_X_Offset = Body_Width + Hand_Width / 2;
	static const int Rod_Y_Offset = Body_Height / 2 + Hand_Height / 2;
};
//------------------------------------------------------------------------------------------------------------
