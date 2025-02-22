#include "Sea_Floor.h"

// AsSea_Floor
//------------------------------------------------------------------------------------------------------------
AsSea_Floor::~AsSea_Floor()
{
	for (auto *shell : Shell)
		delete shell;

	Shell.erase(Shell.begin(), Shell.end());
	
	if (Region)
		DeleteObject(Region);
}
//------------------------------------------------------------------------------------------------------------
AsSea_Floor::AsSea_Floor()
	: X_Pos(0), Sea_Floor_Rect{}, Region(0)
{
}
//------------------------------------------------------------------------------------------------------------
void AsSea_Floor::Init()
{
	const int scale = AsConfig::Global_Scale;

	Shell.push_back(new AShell(X_Pos + Distance_Betw_Shells / scale + 1, Y_Pos + Shell_Y_Pos) );
	Shell.push_back(new AShell(X_Pos + Distance_Betw_Shells + 139, Y_Pos + Shell_Y_Pos) );
	Shell.push_back(new AShell(X_Pos + Distance_Betw_Shells * 3 - 42, Y_Pos + Shell_Y_Pos) );
	Shell.push_back(new AShell(X_Pos + Distance_Betw_Shells * 4 + 23, Y_Pos + Shell_Y_Pos) );
}
//------------------------------------------------------------------------------------------------------------
void AsSea_Floor::Act()
{
	for (auto *shell : Shell)	// раскоментировать рабочий код
		shell->Act();

	//for (auto *pearl : Shell)	// в скором можно убрать если в shell->Act() будет вызывать проблемы. Хотя там его держать логичнее
	//	pearl->Pearl->Act();

	Redraw();
}
//------------------------------------------------------------------------------------------------------------
void AsSea_Floor::Draw(HDC hdc, RECT &paint_area)
{
	RECT intersection_rect;
	RECT sea_floor_rect;

	Get_Sea_Floor_Rect(X_Pos, Y_Pos, sea_floor_rect);

	if (! IntersectRect(&intersection_rect, &paint_area, &sea_floor_rect) )
		return;

	Region = CreateRectRgn(sea_floor_rect.left, sea_floor_rect.top, sea_floor_rect.right, sea_floor_rect.bottom);
	
	if (Region)	// !!! чуйка на ощибку
		SelectClipRgn(hdc, Region);

	Draw_Layers(hdc, paint_area);
	SelectClipRgn(hdc, 0);

	for ( auto &shell : Shell)
		shell->Draw(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
void AsSea_Floor::Get_Sea_Floor_Rect(int x_pos, int y_pos, RECT &rect)
{
	const int scale = AsConfig::Global_Scale;

	rect.left = x_pos + AsConfig::Border_X_Offset;
	rect.top = y_pos;
	rect.right = rect.left + Width * scale;
	rect.bottom = rect.top + Height * scale;
}
//------------------------------------------------------------------------------------------------------------
void AsSea_Floor::Draw_Layers(HDC hdc, RECT &paint_area)
{
	bool is_light = true;
	int i;
	int x_offset = 11 * AsConfig::Global_Scale;
	const int max_layers = 7;

	for(i = 0; i < max_layers; i++)
	{
		Draw_Layer_Sand(hdc, X_Pos - x_offset, AsConfig::Max_Sand_Y_Pos + i * 4, is_light);
		is_light = !is_light;
	}
}
//------------------------------------------------------------------------------------------------------------
void AsSea_Floor::Draw_Layer_Sand(HDC hdc, int x_pos, int y_pos, bool is_light_color)
{
	bool is_offset = false;
	int i;
	const int scale = AsConfig::Global_Scale;
	int count = 8;
	int x_offset, y_offset;
	int layer_width = AsConfig::Max_X_Pos / 12;
	int layer_thickness = 30;

	RECT rect;

	for ( i = 0; i < count; i++)
	{
		if (is_offset)
		{
			x_offset = 0;
			y_offset = 0;
		}
		else
		{
			x_offset = 4;
			y_offset = 3;
		}

		rect.left = x_pos - x_offset + i * 50 * scale;
		rect.top = (y_pos - y_offset) * scale;

		is_offset = !is_offset;

		if (is_light_color)
		{
			rect.right = rect.left + layer_width * scale;
			rect.bottom = rect.top + (AsConfig::Sea_Floor_Height - layer_thickness) * scale;
			AsTools::Ellipse(hdc, rect, AsConfig::Sea_Sand_Light);
		}
		else
		{
			rect.right = rect.left + layer_width * scale;
			rect.bottom = rect.top + (AsConfig::Sea_Floor_Height + layer_thickness) * scale;
			AsTools::Ellipse(hdc, rect, AsConfig::Sea_Sand_Dark);
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void AsSea_Floor::Redraw()
{
	Get_Sea_Floor_Rect(X_Pos, Y_Pos, Sea_Floor_Rect);
	AsTools::Invalidate_Rect(Sea_Floor_Rect);
}
//------------------------------------------------------------------------------------------------------------
