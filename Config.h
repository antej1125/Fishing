#pragma once

#include "Common.h"

//------------------------------------------------------------------------------------------------------------
class AColor
{
public:
	~AColor();
	AColor();
	AColor(unsigned char r, unsigned char g, unsigned char b);
	AColor(const AColor &color, int pen_size);
	AColor(unsigned char r, unsigned char g, unsigned char b, int pen_size);
	AColor(const AColor &pen_color, const AColor &brush_color, int pen_size);
	AColor(COLORREF color_ref, int /*color_count*/);
	AColor(const AColor &start_color, const AColor &end_color, int gradient_position, int gradient_size);

	void operator = (const AColor &another);

	void Set_As(unsigned char r, unsigned char g, unsigned char b);
	int Get_RGB() const;
	void Select(HDC hdc) const;
	void Select_Pen(HDC hdc) const;
	HBRUSH Get_Brush() const;

	unsigned char R, G, B;

private:
	HPEN Pen;
	HBRUSH Brush;
};
//------------------------------------------------------------------------------------------------------------
class AColor_Fade
{
public:
	~AColor_Fade();
	AColor_Fade(const AColor &color, int max_fade_step);
	AColor_Fade(const AColor &origin_color, const AColor &base_color, int max_fade_step);

	AColor *Get_Color(int fade_step);

private:
	std::vector<AColor *> Fading_Colors;
};
//------------------------------------------------------------------------------------------------------------
class AFont
{
public:
	~AFont();
	AFont(int height, int weight, int family, const wchar_t *face_name, bool is_italic = false);

	void Select(HDC hdc) const;

private:
	HFONT Font_Handle;
};
//------------------------------------------------------------------------------------------------------------
class AsConfig
{
public:
	static void Throw();

	static bool Level_Has_Floor;
	static bool Mirror_Togle;
	static int Current_Timer_Tick;
	static HWND Hwnd;

	static const AColor Red_Color, Blue_Color, White_Color, Black_Color, Grey_Color;
	static const AColor Golden_Color;
	static const AColor Violet_Color;
	// Board Color
	static const AColor Sky_Color, Night_Sky_Color;
	static const AColor Brown_Color, Boat_Gunwale_Color, Wooden_Board;
	// Sea Color
	static const AColor Sea_Color;
	static const AColor BG_Color;
	static const AColor Sea_Sand_Light, Sea_Sand_Dark;
	static const AColor Border_Grean, Seaweed_Grean_Light;
	static const AColor Skin_Color;
	// Shell Color
	static const AColor Shell_Color, Shell_Cap_Color, Shell_Cream_Color, Shell_Muscle_Color;
	// Pearl Color
	static const AColor Grey_Color_Pen, Brown_Color_Pen, Wooden_Board_Pen, White_Color_Pen, BG_Pen, Grey_Color_Lettter_Pen;
	static const AColor Green_Letter_Color_Pen;
	static const AColor BG_Outline_Color, Explosion_Red_Color, Explosion_Blue_Color, Red_Line, Golden_Color_Pen, Violet_Color_Pen;
	// Widgets color
	static const AColor Shadow_Color, Highlight_Color;
	static const AFont Name_Font, Score_Font, Logo_Pop_Font, Logo_Corn_Font, Game_Over_Font;
	// Fish Color
	static const AColor Yelow_Fish_Color, Yelow_Fish_Belly_Color, Yellow_Fish_Element_Color, Yellow_Fish_Element_Color_Dark;


	static const double D_Global_Scale;
	static const double Speed_Move;

	static const int Global_Scale = 3;
	static const int Border_X_Offset = 3;
	static const int Border_Y_Offset = 6;
	static const int Max_X_Pos = 400 * Global_Scale;
	static const int Max_Y_Pos = 110 * Global_Scale;
	static const int Sky_Widht = 400;
	static const int Sky_Height = Max_Y_Pos - 80 * Global_Scale;
	static const int Sea_Widht = Sky_Widht;
	static const int Sea_Height = 300;
	static const int Max_Sand_Y_Pos = Max_Y_Pos - 40;
	static const int Pearl_Size = 8 * Global_Scale;
	static const int Sea_Floor_Height = 17 * AsConfig::Global_Scale - 1;

	static const int FPS = 20;
};
//------------------------------------------------------------------------------------------------------------
class AsTools
{
public:
	static int Rand(int range);
	static void Round_Rect(HDC hdc, RECT &rect, int corner_radius = 2);
	static void Rect(HDC hdc, RECT &rect, const AColor &color);
	static void Rect(HDC hdc, int x, int y, int width, int height, const AColor &color);
	static void Ellipse(HDC hdc, RECT &rect, const AColor &color);
	static void Invalidate_Rect(RECT &rect);
	static unsigned char Get_Fading_Channel(unsigned char color, unsigned char bg_color, int step, int max_step);
	static AColor *Get_Fading_Color(const AColor &origin_color, int step, int max_step);
	static AColor *Get_Fading_Color(const AColor &origin_color, const AColor &base_color, int step, int max_step);
};
//------------------------------------------------------------------------------------------------------------
