#include "Fisherman.h"

// AsFisherman
//------------------------------------------------------------------------------------------------------------
AsFisherman::~AsFisherman()
{
	if (Fishing_Rod)
	{
		delete Fishing_Rod;
		Fishing_Rod = 0;
	}
}
//------------------------------------------------------------------------------------------------------------
AsFisherman::AsFisherman(double x_pos, double y_pos)
: Fishing_Rod(0), X_Pos(x_pos), Y_Pos(y_pos), Moving(EFisherman_Move_State::Stop),
 Fisherman_Rect{}
{
	int x_rod_pos, y_rod_pos;

	x_rod_pos = (int)X_Pos * AsConfig::Global_Scale + Rod_X_Offset;
	y_rod_pos = (int)Y_Pos * AsConfig::Global_Scale + Rod_Y_Offset;

	Fishing_Rod = new AFishing_Rod( (double)(x_rod_pos), (double)(y_rod_pos) );
}
//------------------------------------------------------------------------------------------------------------
void AsFisherman::Act()
{
	// need to done !!!
	Redraw();
}
//------------------------------------------------------------------------------------------------------------
void AsFisherman::Draw(HDC hdc, RECT &paint_area)
{
	RECT rect, intersection_rect;
	XFORM mirror_xform, original_xform;

	rect.left = (int)X_Pos * AsConfig::Global_Scale;
	rect.top = (int)Y_Pos * AsConfig::Global_Scale;
	rect.right = rect.left + Body_Width;
	rect.bottom = rect.top + Body_Height;

	// 1. Сохранение текущей матрицы трансформации
	GetWorldTransform(hdc, &original_xform);

	// 2. Настройка матрицы для отражения по горизонтали
	mirror_xform.eM11 = -1.0f; // Отражение по горизонтали
	mirror_xform.eM12 = 0.0f;
	mirror_xform.eM21 = 0.0f;
	mirror_xform.eM22 = 1.0f;
	mirror_xform.eDx = (float)(2 * rect.right - Body_Width); // Корректируем смещение
	//mirror_xform.eDx = (float)(2 * Fisherman_Rect.right - (Body_Width ) * 2); // Корректируем смещение
	mirror_xform.eDy = 0.0f;

	if (! IntersectRect(&intersection_rect, &paint_area, &rect) )
	//if (! IntersectRect(&intersection_rect, &paint_area, &Fisherman_Rect) )
		return;

	// Применение матрицы трансформации
	if (AsConfig::Mirror_Togle)
	{
		SetWorldTransform(hdc, &mirror_xform);
		Fishing_Rod->Set_X_Pos(X_Pos + Hand_Height);
	}
	else
		Fishing_Rod->Set_X_Pos(X_Pos + (double)(Hand_Width));

	Draw_Body(hdc, rect);
	Draw_Head(hdc, rect);
	Draw_Hand(hdc, rect);

	// 3. Восстановление оригинальной матрицы трансформации
	SetWorldTransform(hdc, &original_xform);
}
//------------------------------------------------------------------------------------------------------------
void AsFisherman::Set_X_Pos(double x_pos)
{
	X_Pos = x_pos;

	Fishing_Rod->Set_X_Pos(x_pos + double(Hand_Width) );
}
//------------------------------------------------------------------------------------------------------------
void AsFisherman::Move(bool to_left)
{
	if (to_left)
		X_Pos -= AsConfig::Speed_Move;
	else
		X_Pos += AsConfig::Speed_Move;

	Fishing_Rod->Move(to_left);

	Fishing_Rod->Set_X_Pos(X_Pos + double(Hand_Width) );
	
	Redraw();
}
//------------------------------------------------------------------------------------------------------------
void AsFisherman::Draw_Body(HDC hdc, RECT &rect)
{
	int x_start, y_start;
	int x_end, y_end;
	//RECT rect;


	AsConfig::Red_Color.Select(hdc);

	x_start = rect.right;
	y_start = rect.top + Body_Height;
	x_end = rect.left;
	y_end = y_start;

	Chord(hdc, rect.left, rect.top, rect.right, rect.bottom, x_start, y_start, x_end, y_end);
}
//------------------------------------------------------------------------------------------------------------
void AsFisherman::Draw_Head(HDC hdc, RECT &rect)
{
	int head_y_pos = (int)Y_Pos * AsConfig::Global_Scale - Body_Height / 2 - Offset;
	RECT head_rect = rect;

	// 1. Голова
	head_rect.left += 1;
	head_rect.top -= Head_Height - Offset;
	head_rect.right = head_rect.left + Head_Width - 1;
	head_rect.bottom = head_rect.top + Head_Height - 1;

	AsTools::Ellipse(hdc, head_rect, AsConfig::Brown_Color);
	Draw_Face_Part(hdc, head_rect);

	Draw_Cap(hdc, head_rect);
	Draw_Glasses(hdc, head_rect);
}
//------------------------------------------------------------------------------------------------------------
void AsFisherman::Draw_Cap(HDC hdc, RECT &head_rect)
{
	int x_start, y_start;
	int x_end, y_end;

	// 1. Кепка
	head_rect.left -= Offset;
	head_rect.top -= Offset;
	head_rect.right += Offset;
	head_rect.bottom += Offset;

	x_start = head_rect.right;
	y_start = head_rect.top + (head_rect.bottom - head_rect.top) / 2;
	x_end = head_rect.left;
	y_end = y_start;

	AsConfig::Red_Color.Select(hdc);
	Chord(hdc, head_rect.left, head_rect.top, head_rect.right, head_rect.bottom, x_start, y_start, x_end, y_end);

	// 2. Картуз
	AsConfig::Red_Line.Select(hdc);
	MoveToEx(hdc, head_rect.right, y_start - 1, 0);
	LineTo(hdc, head_rect.right + Cap_Width, y_start - 1);
}
//------------------------------------------------------------------------------------------------------------
void AsFisherman::Draw_Glasses(HDC hdc, RECT &head_rect)
{
	int glasses_temple_length = 7;
	int y_pos = head_rect.top + (head_rect.bottom - head_rect.top) / 2 + 1;
	RECT glasses_rect = {};

	// 4. Очки
	// 4.1. Линзы
	glasses_rect.left = head_rect.right - (Glasses_Width + Offset) + 1;
	glasses_rect.top = y_pos;
	glasses_rect.right = glasses_rect.left + Glasses_Width;
	glasses_rect.bottom = glasses_rect.top + Glasses_Height;

	AsConfig::Grey_Color.Select(hdc);
	AsTools::Round_Rect(hdc, glasses_rect);

	// 4.1. Дужки
	MoveToEx(hdc, glasses_rect.left, glasses_rect.top + Offset - 1, 0);
	LineTo(hdc, glasses_rect.left - glasses_temple_length, glasses_rect.top + Offset - 1);
	MoveToEx(hdc, glasses_rect.left - glasses_temple_length, glasses_rect.top + Offset, 0);
	LineTo(hdc, glasses_rect.left - glasses_temple_length, glasses_rect.top + Offset * 2);
}
//------------------------------------------------------------------------------------------------------------
void AsFisherman::Draw_Face_Part(HDC hdc, RECT &head_rect)
{
	int ofsset_x_pos = (head_rect.right - head_rect.left) / 3;
	int ofsset_y_pos = (head_rect.bottom - head_rect.top) / 3;

	AsConfig::Skin_Color.Select(hdc);
	Chord(hdc, head_rect.left, head_rect.top, head_rect.right - 1, head_rect.bottom - 1,
		head_rect.left + ofsset_x_pos, head_rect.bottom - ofsset_y_pos,
		head_rect.right - ofsset_x_pos, head_rect.top + ofsset_y_pos);
}
//------------------------------------------------------------------------------------------------------------
void AsFisherman::Draw_Hand(HDC hdc, RECT &rect)
{
	rect.left += Body_Width;
	rect.top += Hand_Y_Offset;
	rect.right = rect.left + Hand_Width;
	rect.bottom = rect.top - Hand_Height;

	AsConfig::Skin_Color.Select(hdc);
	AsTools::Round_Rect(hdc, rect);
}
//------------------------------------------------------------------------------------------------------------
void AsFisherman::Redraw()
{
	//int x_offset = 7;

	//Fisherman_Rect.left = ((int)X_Pos - x_offset) * AsConfig::Global_Scale;
	//Fisherman_Rect.top = (int)Y_Pos * AsConfig::Global_Scale;
	//Fisherman_Rect.right = Fisherman_Rect.left + Body_Width + Hand_Width * AsConfig::Global_Scale + Offset;
	//Fisherman_Rect.bottom = Fisherman_Rect.top + Body_Height;
	//int x_offset = 7;

	Fisherman_Rect.left = (int)X_Pos * AsConfig::Global_Scale;
	Fisherman_Rect.top = (int)Y_Pos * AsConfig::Global_Scale;
	Fisherman_Rect.right = Fisherman_Rect.left + Body_Width + Hand_Width * AsConfig::Global_Scale;
	Fisherman_Rect.bottom = Fisherman_Rect.top + Body_Height;

	AsTools::Invalidate_Rect(Fisherman_Rect);
}
//------------------------------------------------------------------------------------------------------------
void AsFisherman::Clear(HDC hdc)
{
	//AsTools::Rect(hdc, Fisherman_Rect, AsConfig::Sky_Color);
}
//------------------------------------------------------------------------------------------------------------
