#pragma once

#include "Shell.h"

//------------------------------------------------------------------------------------------------------------
class AsSea_Floor : public AGraphics_Object
{
public:
	~AsSea_Floor();
	AsSea_Floor();

	virtual void Act();
	virtual void Draw(HDC hdc, RECT &paint_area);

	void Init();

private:
	void Draw_Layers(HDC hdc, RECT &paint_area);
	void Draw_Layer_Sand(HDC hdc, int x_pos, int y_pos, bool is_light_color);
	void Get_Sea_Floor_Rect(int x_pos, int y_pos, RECT &rect);
	void Redraw();

	std::vector<AShell *> Shell;
	RECT Sea_Floor_Rect;
	HRGN Region;

	//std::vector<ASeaweed *> Seaweeds;

	int X_Pos;
	static const int Y_Pos = AsConfig::Max_Sand_Y_Pos * AsConfig::Global_Scale - 10;
	static const int Width = AsConfig::Sea_Widht;
	static const int Height = 14 * AsConfig::Global_Scale;
	static const int Distance_Betw_Shells = 80 * AsConfig::Global_Scale;
	static const int Shell_Y_Pos = 11 * AsConfig::Global_Scale;
};
//------------------------------------------------------------------------------------------------------------