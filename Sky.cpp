#include "Sky.h"

// AsSky
//------------------------------------------------------------------------------------------------------------
AsSky::AsSky()
	: X_Pos(0), Y_Pos(0), Sky_Rect{}
{
	Sky_Rect.left = X_Pos + AsConfig::Border_X_Offset;
	Sky_Rect.top = Y_Pos;
	Sky_Rect.right = Sky_Rect.left + Width * AsConfig::Global_Scale;
	Sky_Rect.bottom = Sky_Rect.left + Height * AsConfig::Global_Scale;
}
//------------------------------------------------------------------------------------------------------------
void AsSky::Act()
{
	// not used
}
//------------------------------------------------------------------------------------------------------------
void AsSky::Draw(HDC hdc, RECT &paint_area)
{
	RECT intersection_rect;

	if (!IntersectRect(&intersection_rect, &paint_area, &Sky_Rect) )
		return;

	AsTools::Rect(hdc, Sky_Rect, AsConfig::Sky_Color);
}
//------------------------------------------------------------------------------------------------------------