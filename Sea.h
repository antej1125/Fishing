#pragma once

#include "Config.h"

class AsSea :  public AGraphics_Object
{
public:
	~AsSea();
	AsSea();

	virtual void Act();
	virtual void Draw(HDC hdc, RECT &paint_area);

private:
	void Draw_Waves(HDC hdc, RECT &paint_area);

	static const int X_Pos = AsConfig::Border_X_Offset;
	static const int Y_Pos = AsConfig::Sky_Height * AsConfig::Global_Scale;
	static const int Wave_Step = AsConfig::Global_Scale;
	static const int Wave_Count = 40;
	static const int Wave_Width = AsConfig::Sea_Widht / Wave_Count;
	static const int Wave_Height = Wave_Width;
};
//------------------------------------------------------------------------------------------------------------