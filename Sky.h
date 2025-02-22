#pragma once

#include "Config.h"

//------------------------------------------------------------------------------------------------------------
class AsSky : public AGraphics_Object
{
public:
	AsSky();
	virtual void Act();
	virtual void Draw(HDC hdc, RECT &paint_area);

private:
	int X_Pos, Y_Pos;
	static const int Width = AsConfig::Sky_Widht;
	static const int Height = AsConfig::Sky_Height;

	RECT Sky_Rect;
};
//------------------------------------------------------------------------------------------------------------