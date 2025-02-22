#include "Sea.h"

// AsSea
//------------------------------------------------------------------------------------------------------------
AsSea::~AsSea()
{
}
//------------------------------------------------------------------------------------------------------------
AsSea::AsSea()
{
}
//------------------------------------------------------------------------------------------------------------
void AsSea::Act()
{
}
//------------------------------------------------------------------------------------------------------------
void AsSea::Draw(HDC hdc, RECT &paint_area)
{
	RECT sea_rect;
	RECT intersection_rect;

	sea_rect.left = X_Pos;
	sea_rect.top = Y_Pos + 1;
	sea_rect.right = sea_rect.left + AsConfig::Sea_Widht * AsConfig::Global_Scale;
	sea_rect.bottom = sea_rect.top + AsConfig::Sea_Height * AsConfig::Global_Scale;

	if (! IntersectRect(&intersection_rect, &paint_area, &sea_rect) )
		return;

	AsTools::Rect(hdc, sea_rect, AsConfig::Sea_Color);
	Draw_Waves(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
void AsSea::Draw_Waves(HDC hdc, RECT &paint_area)
{
	int i;
	const int scale = AsConfig::Global_Scale;
	int radius = Wave_Width / 2;
	int y_pos = Y_Pos - Wave_Height - radius;
	RECT wave_rect;

	for (i = 0; i < Wave_Count; i++)
	{
		wave_rect.left = X_Pos + Wave_Width * i * Wave_Step;
		wave_rect.top = y_pos;
		wave_rect.right = wave_rect.left + Wave_Width * scale;
		wave_rect.bottom = y_pos + Wave_Height * scale;

		AsTools::Ellipse(hdc, wave_rect, AsConfig::Sky_Color);
	}
}
//------------------------------------------------------------------------------------------------------------
