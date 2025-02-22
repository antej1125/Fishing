#include "Config.h"


// AColor
//------------------------------------------------------------------------------------------------------------
AColor::~AColor()
{
	if (Pen != 0)
		DeleteObject(Pen);

	if (Brush != 0)
		DeleteObject(Brush);
}
//------------------------------------------------------------------------------------------------------------
AColor::AColor()
	: R(0), G(0), B(0), Pen(0), Brush(0)
{
}
//------------------------------------------------------------------------------------------------------------
AColor::AColor(unsigned char r, unsigned char g, unsigned char b)
	: R(r), G(g), B(b), Pen(0), Brush(0)
{
	Pen = CreatePen(PS_SOLID, 0, RGB(r, g, b) );
	Brush = CreateSolidBrush(RGB(r, g, b) );
}
//------------------------------------------------------------------------------------------------------------
AColor::AColor(const AColor &color, int pen_size)
	: R(color.R), G(color.G), B(color.B), Pen(0), Brush(0)
{
	Pen = CreatePen(PS_SOLID, pen_size, color.Get_RGB() );
}
//------------------------------------------------------------------------------------------------------------
AColor::AColor(unsigned char r, unsigned char g, unsigned char b, int pen_size)
	: R(r), G(g), B(b), Pen(0), Brush(0)
{
	Pen = CreatePen(PS_SOLID, pen_size, RGB(r, g, b) );
}
//------------------------------------------------------------------------------------------------------------
AColor::AColor(const AColor &pen_color, const AColor &brush_color, int pen_size)
	: R(0), G(0), B(0), Pen(0), Brush(0)
{
	Pen = CreatePen(PS_SOLID, pen_size, pen_color.Get_RGB() );
	Brush = CreateSolidBrush(brush_color.Get_RGB() );
}
//------------------------------------------------------------------------------------------------------------
AColor::AColor(COLORREF color_ref, int /*color_count*/)
	: R(GetRValue(color_ref) ), G(GetGValue(color_ref) ), B(GetBValue(color_ref) ), Pen(0), Brush(0)
{
	Pen = CreatePen(PS_SOLID, 0, color_ref);
	Brush = CreateSolidBrush(color_ref);
}
//------------------------------------------------------------------------------------------------------------
AColor::AColor(const AColor &start_color, const AColor &end_color, int gradient_position, int gradient_size)
	: R(0), G(0), B(0), Pen(0), Brush(0)
{
	// gradient_size = std::max(1, gradient_size);

	gradient_size = (gradient_size > 1) ? gradient_size : 1;

	// Calculate color step for interpolation
	float step_r = static_cast<float>(end_color.R - start_color.R) / gradient_size;
	float step_g = static_cast<float>(end_color.G - start_color.G) / gradient_size;
	float step_b = static_cast<float>(end_color.B - start_color.B) / gradient_size;

	// Calculate color at given position in the gradient
	R = static_cast<unsigned char>(start_color.R + step_r * gradient_position);
	G = static_cast<unsigned char>(start_color.G + step_g * gradient_position);
	B = static_cast<unsigned char>(start_color.B + step_b * gradient_position);

	// Create Pen and Brush objects
	Pen = CreatePen(PS_SOLID, 0, RGB(R, G, B));
	Brush = CreateSolidBrush(RGB(R, G, B));
}
//------------------------------------------------------------------------------------------------------------
void AColor::operator = (const AColor &another)
{
	AsConfig::Throw();  // При присваивании нужно создать копии карандаша и кисти. Вместо этого следует их пересоздать методом Set_As().
}
//------------------------------------------------------------------------------------------------------------
void AColor::Set_As(unsigned char r, unsigned char g, unsigned char b)
{
	R = r;
	G = g;
	B = b;

	if (Pen != 0)
		DeleteObject(Pen);

	if (Brush != 0)
		DeleteObject(Brush);

	Pen = CreatePen(PS_SOLID, 0, RGB(R, G, B) );
	Brush = CreateSolidBrush(RGB(R, G, B) );
}
//------------------------------------------------------------------------------------------------------------
int AColor::Get_RGB() const
{
	return RGB(R, G, B);
}
//------------------------------------------------------------------------------------------------------------
void AColor::Select(HDC hdc) const
{
	SelectObject(hdc, Pen);
	SelectObject(hdc, Brush);
}
//------------------------------------------------------------------------------------------------------------
void AColor::Select_Pen(HDC hdc) const
{
	SelectObject(hdc, Pen);
}
//------------------------------------------------------------------------------------------------------------
HBRUSH AColor::Get_Brush() const
{
	return Brush;
}
//------------------------------------------------------------------------------------------------------------




// AColor_Fade
//------------------------------------------------------------------------------------------------------------
AColor_Fade::~AColor_Fade()
{
	for (auto *color: Fading_Colors)
		delete color;

	Fading_Colors.erase(Fading_Colors.begin(), Fading_Colors.end() );
}
//------------------------------------------------------------------------------------------------------------
AColor_Fade::AColor_Fade(const AColor &color, int max_fade_step)
{
	int i;
	AColor *curr_color;

	for (i = 0; i < max_fade_step; i++)
	{
		curr_color = AsTools::Get_Fading_Color(color, i, max_fade_step);
		Fading_Colors.push_back(curr_color);
	}
}
//------------------------------------------------------------------------------------------------------------
AColor_Fade::AColor_Fade(const AColor &origin_color, const AColor &base_color, int max_fade_step)
{
	int i;
	AColor *curr_color;

	for (i = 0; i < max_fade_step; i++)
	{
		curr_color = AsTools::Get_Fading_Color(origin_color, base_color, i, max_fade_step);
		Fading_Colors.push_back(curr_color);
	}
}
//------------------------------------------------------------------------------------------------------------
AColor *AColor_Fade::Get_Color(int fade_step)
{
	if (fade_step < 0 || fade_step >= (int)Fading_Colors.size() )
		AsConfig::Throw();

	return Fading_Colors[fade_step];
}
//------------------------------------------------------------------------------------------------------------



// AFont
//------------------------------------------------------------------------------------------------------------
AFont::~AFont()
{
	if (Font_Handle != 0)
		DeleteObject(Font_Handle);
}
//------------------------------------------------------------------------------------------------------------
AFont::AFont(int height, int weight, int family, const wchar_t *face_name, bool is_italic)
{
	LOGFONT log_font{};

	log_font.lfHeight = height;
	log_font.lfWeight = weight;
	log_font.lfOutPrecision = 3;
	log_font.lfClipPrecision = 2;
	log_font.lfQuality = 1;
	log_font.lfPitchAndFamily = family;
	wcscpy_s(log_font.lfFaceName, face_name);

	if (is_italic)
		log_font.lfItalic = 255;

	Font_Handle = CreateFontIndirect(&log_font);
}
//------------------------------------------------------------------------------------------------------------
void AFont::Select(HDC hdc) const
{
	SelectObject(hdc, Font_Handle);
}
//------------------------------------------------------------------------------------------------------------




// AsConfig
bool AsConfig::Mirror_Togle = false;
int AsConfig::Current_Timer_Tick = 0;
HWND AsConfig::Hwnd;

//const AColor AsConfig::BG_Color(15, 63, 31);
//Fisher colors
const AColor AsConfig::Red_Color(255, 85, 85);
const AColor AsConfig::Blue_Color(85, 255, 255);
const AColor AsConfig::White_Color(255, 255, 255);
const AColor AsConfig::Black_Color(0, 0, 0);
const AColor AsConfig::Grey_Color(127, 127, 127);

const AColor AsConfig::Sky_Color(213, 255, 248);
const AColor AsConfig::Night_Sky_Color(25, 25, 112);

const AColor AsConfig::Brown_Color(226, 143, 71);
const AColor AsConfig::Boat_Gunwale_Color(230, 118, 65);
const AColor AsConfig::Wooden_Board(231, 157, 74);
// Sea elements colors
const AColor AsConfig::Sea_Color(100, 230, 250);
const AColor AsConfig::Sea_Sand_Light(243, 202, 136);
const AColor AsConfig::Sea_Sand_Dark(215, 162, 86);
// Border and fisherman skin
const AColor AsConfig::Border_Grean(34, 178, 76);
const AColor AsConfig::Seaweed_Grean_Light(162, 220, 39); // Delete
const AColor AsConfig::Skin_Color(224, 214, 179);
// Shell
const AColor AsConfig::Shell_Color(200, 90, 90);
const AColor AsConfig::Shell_Cap_Color(229, 108, 107);
const AColor AsConfig::Shell_Cream_Color(246, 225, 215);
const AColor AsConfig::Shell_Muscle_Color(255, 209, 221);
// Pearl's Letter
const AColor AsConfig::Golden_Color(255, 215, 0);
const AColor AsConfig::Violet_Color(211, 5, 215);
// Yellow Fish
const AColor AsConfig::Yelow_Fish_Color(255, 201, 97);
//const AColor AsConfig::Yelow_Fish_Color(255, 234, 152);
//const AColor AsConfig::Yelow_Fish_Belly_Color(249, 149, 80);
const AColor AsConfig::Yellow_Fish_Element_Color(177, 79, 37);
const AColor AsConfig::Yellow_Fish_Element_Color_Dark(149, 65, 25);


const AColor AsConfig::BG_Color(Sea_Color);

const AColor AsConfig::BG_Pen(Sea_Color, 3);
const AColor AsConfig::White_Color_Pen(White_Color, 1);
const AColor AsConfig::Grey_Color_Pen(Grey_Color, 1);
const AColor AsConfig::Grey_Color_Lettter_Pen(Grey_Color, 3);
const AColor AsConfig::Brown_Color_Pen(Brown_Color, Global_Scale * 2 - 1);
const AColor AsConfig::Wooden_Board_Pen(231, 157, 74, Global_Scale * 2 + 1);
const AColor AsConfig::BG_Outline_Color(BG_Color, Global_Scale * 2 / 3);
const AColor AsConfig::Red_Line(Red_Color, Global_Scale);
const AColor AsConfig::Golden_Color_Pen(Golden_Color, Global_Scale);
const AColor AsConfig::Violet_Color_Pen(Violet_Color, Global_Scale);
const AColor AsConfig::Green_Letter_Color_Pen(Seaweed_Grean_Light, Global_Scale);

const AColor AsConfig::Explosion_Red_Color(White_Color, Red_Color, 0);
const AColor AsConfig::Explosion_Blue_Color(White_Color, Blue_Color, 0);
const AColor AsConfig::Shadow_Color(BG_Color, Global_Scale);
const AColor AsConfig::Highlight_Color(White_Color, Global_Scale);

const AFont AsConfig::Name_Font(-48, 700, 49, L"Consolas");
const AFont AsConfig::Score_Font(-44, 700, 49, L"Consolas");
const AFont AsConfig::Logo_Pop_Font(-128, 900, 34, L"Arial Black");
const AFont AsConfig::Logo_Corn_Font(-96, 900, 34, L"Arial Black");
const AFont AsConfig::Game_Over_Font(-60, 700, 66, L"Comic Sans MS", true);

const double AsConfig::D_Global_Scale = (double)Global_Scale;
const double AsConfig::Speed_Move = 3.0;
//------------------------------------------------------------------------------------------------------------
void AsConfig::Throw()
{
	throw 13;
}
//------------------------------------------------------------------------------------------------------------




// AsTools
//------------------------------------------------------------------------------------------------------------
int AsTools::Rand(int range)
{// Вычисляет псевдослучайное число в диапазоне [0, .. range - 1]

	return rand() * range / (RAND_MAX + 1);
}
//------------------------------------------------------------------------------------------------------------
void AsTools::Round_Rect(HDC hdc, RECT &rect, int corner_radius)
{
	int radius = corner_radius * AsConfig::Global_Scale;

	RoundRect(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1, radius, radius);
}
//------------------------------------------------------------------------------------------------------------
void AsTools::Rect(HDC hdc, RECT &rect, const AColor &color)
{
	color.Select(hdc);
	Rectangle(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1);
}
//------------------------------------------------------------------------------------------------------------
void AsTools::Rect(HDC hdc, int x, int y, int width, int height, const AColor &color)
{
	const int scale = AsConfig::Global_Scale;

	color.Select(hdc);
	Rectangle(hdc, x * scale, y * scale, (x + width) * scale - 1, (y + height) * scale - 1);
}
//------------------------------------------------------------------------------------------------------------
void AsTools::Ellipse(HDC hdc, RECT &rect, const AColor &color)
{
	color.Select(hdc);
	::Ellipse(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1);
}
//------------------------------------------------------------------------------------------------------------
void AsTools::Invalidate_Rect(RECT &rect)
{
	InvalidateRect(AsConfig::Hwnd, &rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------
unsigned char AsTools::Get_Fading_Channel(unsigned char color, unsigned char bg_color, int step, int max_step)
{
	return color - step * (color - bg_color) / (max_step - 1);
}
//------------------------------------------------------------------------------------------------------------
AColor *AsTools::Get_Fading_Color(const AColor &origin_color, int step, int max_step)
{
	return Get_Fading_Color(origin_color, AsConfig::BG_Color, step, max_step);
}
//------------------------------------------------------------------------------------------------------------
AColor *AsTools::Get_Fading_Color(const AColor &origin_color, const AColor &base_color, int step, int max_step)
{
	unsigned char r, g, b;
	AColor *result_color;

	r = Get_Fading_Channel(origin_color.R, base_color.R, step, max_step);
	g = Get_Fading_Channel(origin_color.G, base_color.G, step, max_step);
	b = Get_Fading_Channel(origin_color.B, base_color.B, step, max_step);

	result_color = new AColor(r, g, b);

	return result_color;
}
//------------------------------------------------------------------------------------------------------------
