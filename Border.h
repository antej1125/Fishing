#pragma once

#include "Config.h"

//------------------------------------------------------------------------------------------------------------
class AsBorder : public AGraphics_Object
{
public:
	AsBorder();

	virtual void Act();
	virtual void Draw(HDC hdc, RECT &paint_are);

private:
	int X_Pos;
	int Y_Pos;
};
//------------------------------------------------------------------------------------------------------------