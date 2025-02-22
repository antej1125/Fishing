#pragma once

#include "Fish.h"

class AsFish_Set : public AGraphics_Object
{
public:
	~AsFish_Set();
	AsFish_Set();

	virtual void Act();
	virtual void Draw(HDC hdc, RECT &paint_area);
	
	void Init();

private:
	void Emit_Fish();

	std::vector<AFish *> Fish;

	static const int Step_Y_Pos_Fish = 70;
	static const int Max_Y_Pos = AsConfig::Sea_Height - AsConfig::Sea_Floor_Height;
};