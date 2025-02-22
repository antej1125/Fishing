#include "Border.h"

//AsBorder
//------------------------------------------------------------------------------------------------------------
AsBorder::AsBorder()
 : X_Pos(0), Y_Pos(0)
{
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Act()
{
	// not used
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Draw(HDC hdc, RECT &paint_area)
{
	int i;
	RECT rect, intersection_rect;

	rect.left = X_Pos;
	rect.top = Y_Pos;
	rect.right = rect.left + AsConfig::Border_X_Offset * AsConfig::Global_Scale;
	rect.bottom = rect.top + AsConfig::Max_Y_Pos * AsConfig::Global_Scale;

	for (i = 0; i < 2; i++)
	{
		rect.left = i * (AsConfig::Max_X_Pos - AsConfig::Global_Scale);
		rect.right = rect.left + AsConfig::Border_X_Offset * AsConfig::Global_Scale;
	
		if (! IntersectRect(&intersection_rect, &paint_area, &rect))
			return;

		AsTools::Rect(hdc, rect, AsConfig::Border_Grean);
	}
}
//------------------------------------------------------------------------------------------------------------

