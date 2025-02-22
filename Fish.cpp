#include "Fish.h"

// AFish
//------------------------------------------------------------------------------------------------------------
AFish::AFish(int x_pos, int y_pos)
 : X_Pos(x_pos), Y_Pos(y_pos), Speed (3.8), Fish_Rect{},Colliion_Rect{}, Fish_State(EFish_State::Emmiting), Prev_Fish_State(EFish_State::Idle)
{
}
//------------------------------------------------------------------------------------------------------------
void AFish::Act()
{
	switch (Fish_State)
	{
	case EFish_State::Idle:
		break;

	case EFish_State::Emmiting:
		Set_Direction();
		break;
	
	case EFish_State::Move_Left:
		X_Pos -= Speed;
		break;
	
	case EFish_State::Move_Right:
		X_Pos += Speed;
		break;
	
	case EFish_State::On_Hook:
		break;
	
	case EFish_State::Escape:
		break;

	default:
		AsConfig::Throw();
	}

	if (Is_Finished() )
		Update_Move_Diraction(Fish_State);

	Redraw();
}
//------------------------------------------------------------------------------------------------------------
void AFish::Draw(HDC hdc, RECT &paint_area)
{
	RECT rect, intersection_rect;
	XFORM xform, origin_xform;

	rect.left = X_Pos;
	rect.top = Y_Pos;
	rect.right = rect.left + Width;
	rect.bottom = rect.top + Height;

	// 1. Сохранение текущей матрицы трансформации
	GetWorldTransform(hdc, &origin_xform);

	// 2. Настройка матрицы для отражения по горизонтали
	xform.eM11 = -1.0f; // Отражение по горизонтали
	xform.eM12 = 0.0f;
	xform.eM21 = 0.0f;
	xform.eM22 = 1.0f;
	xform.eDx = (float)(rect.left * 2 + Width); // Корректируем смещение
	xform.eDy = 0.0f;

	if (! IntersectRect(&intersection_rect, &paint_area, &rect) )
		return;

	if (Fish_State == EFish_State::Move_Right)
	//if (Fish_State == EFish_State::Move_Right)
		SetWorldTransform(hdc, &xform);
	
	Draw_Fish(hdc, rect);
	
	SetWorldTransform(hdc, &origin_xform);
}
//------------------------------------------------------------------------------------------------------------
bool AFish::Is_Finished()
{	
	if (Fish_State == EFish_State::Move_Left && X_Pos <= AsConfig::Border_X_Offset - Width)
		return true;

	if (Fish_State == EFish_State::Move_Right && X_Pos >= AsConfig::Max_X_Pos - AsConfig::Border_X_Offset)
		return true;

	return false;
}
//------------------------------------------------------------------------------------------------------------
void AFish::Set_State(EFish_State new_state)
{
	Fish_State = new_state;
}
//------------------------------------------------------------------------------------------------------------
void AFish::Set_Direction()
{
	int direction;

	Prev_Fish_State = Fish_State;

	direction = AsTools::Rand(2);

	if (Prev_Fish_State == EFish_State::Move_Left || Prev_Fish_State == EFish_State::Move_Right)
		return;

	switch (direction)
	{
	case 0:
		Fish_State = EFish_State::Move_Left;
		break;

	case 1: 
		Fish_State = EFish_State::Move_Right;
		break;

	default:
		AsConfig::Throw();
	}
}
//------------------------------------------------------------------------------------------------------------
void AFish::Draw_Fish(HDC hdc, RECT &rect)
{
	HRGN region;
	XFORM xform, origin_xform;

	xform.eM11 = 0.0f;  // cos(90°)
	xform.eM12 = 1.0f; // -sin(90°)
	xform.eM21 = -1.0f;  // sin(90°)
	xform.eM22 = 0.0f;  // cos(90°)
	xform.eDx = (float)(rect.left - (Width / 3 - AsConfig::Global_Scale + 1) ) * 2.0f; // Коррекция смещения
	xform.eDy = -(float)(Y_Pos / 4);

	GetWorldTransform(hdc, &origin_xform);

	if (Fish_State == EFish_State::On_Hook)
		SetWorldTransform(hdc, &xform);

	// 1. рисуем плавники: спины, живота, брюшной (возле глаза) 
	Draw_Fin(hdc, rect);

	// 2. Рисуем тело рыбки
	AsConfig::Yelow_Fish_Color.Select(hdc);
	AsTools::Round_Rect(hdc, rect, 10);

	// 3. Рисуем оставшиеся части
	Draw_Pelvic_Fin_Front(hdc, rect);
	Draw_Eye(hdc, rect);
	Draw_Mouth(hdc, rect);
	Draw_Tail(hdc, rect);
	Draw_Gills(hdc, rect);

	SetWorldTransform(hdc, &origin_xform);
}
//------------------------------------------------------------------------------------------------------------
void AFish::Draw_Mouth(HDC hdc, RECT &rect)
{
	// 3. Рисуем рот
	rect.left = X_Pos - Mouth_Size / 2 - 1;
	rect.top = Y_Pos + Half_Height - Mouth_Size / 2;
	rect.right = rect.left + Mouth_Size;
	rect.bottom = rect.top + Mouth_Size;

	AsConfig::Yellow_Fish_Element_Color.Select(hdc);
	Chord(hdc, rect.left, rect.top, rect.right, rect.bottom, rect.left + Mouth_Size / 2, rect.bottom, rect.left + Mouth_Size / 2, rect.top);
}
//------------------------------------------------------------------------------------------------------------
void AFish::Draw_Tail(HDC hdc, RECT &rect)
{
	// рисуем хвост
	rect.left = X_Pos + Width - 1;
	rect.top = Y_Pos + Offset / 2;
	rect.right = rect.left + Tail_Size;
	rect.bottom = rect.top + Tail_Size;

	AsConfig::Yellow_Fish_Element_Color.Select(hdc);
	Chord(hdc, rect.left, rect.top - 1, rect.right, rect.bottom, rect.left + Tail_Size / 2 + Offset / 2, rect.top, rect.left + Tail_Size / 2 + Offset / 2, rect.bottom);

	rect.left += Tail_Size / 2;
	rect.top += Offset;
	rect.right = rect.left + Offset + 1;
	rect.bottom = rect.top + Offset;

	AsTools::Ellipse(hdc, rect, AsConfig::Sea_Color);
}
//------------------------------------------------------------------------------------------------------------
void AFish::Draw_Eye(HDC hdc, RECT &rect)
{
	// 1. Рисуем веко
	rect.left = X_Pos + Offset + 1;
	rect.top = Y_Pos + Half_Height / 2;
	rect.right = rect.left + Eye_Size;
	rect.bottom = rect.top + Eye_Size;

	AsTools::Ellipse(hdc, rect, AsConfig::Yellow_Fish_Element_Color);

	for (int i = 1; i <= 2; i++)
	{
		rect.left += 1;
		rect.top += 1;
		rect.right -= 1;
		rect.bottom -= 1;
		
		if (i == 1)
			AsTools::Ellipse(hdc, rect, AsConfig::White_Color); 	// 2. Глазное яблоко
		else
			AsTools::Ellipse(hdc, rect, AsConfig::Black_Color);	// 3. Зрачок
	}

	// 4. Блик на глазу
	rect.left += 2;
	rect.right = rect.left + 3;
	rect.bottom = rect.top + 2;
	AsTools::Rect(hdc, rect, AsConfig::White_Color);
}
//------------------------------------------------------------------------------------------------------------
void AFish::Draw_Fin(HDC hdc, RECT rect)
{
	Draw_Back_Fin(hdc, rect);
	Draw_Belly_Fin(hdc, rect);
	Draw_Pelvic_Fin_Back(hdc, rect);
}
//------------------------------------------------------------------------------------------------------------
void AFish::Draw_Gills(HDC hdc, RECT rect)
{
	int x_offset_pos = Offset * AsConfig::Global_Scale;

	rect.left = X_Pos + x_offset_pos;
	rect.top = Y_Pos + 3;
	rect.right = rect.left + 5;
	rect.bottom = rect.top + 13;

	AsConfig::Grey_Color.Select(hdc);

	Arc(hdc, rect.left, rect.top + 1, rect.right, rect.bottom - 1,
		rect.left + 2, rect.bottom - 4,
		rect.left + 2, rect.top + 4);
}
//------------------------------------------------------------------------------------------------------------
void AFish::Draw_Belly_Fin(HDC hdc, RECT &rect)
{
	int x_pos = X_Pos + Width / 2 + Belly_Fin_Size / 2;
	int y_pos = Y_Pos + Belly_Fin_Size - Offset;

	rect.left = x_pos;
	rect.top = y_pos;
	rect.right = rect.left + Belly_Fin_Size;
	rect.bottom = rect.top + Belly_Fin_Size;

	AsTools::Ellipse(hdc, rect, AsConfig::Yellow_Fish_Element_Color);
}
//------------------------------------------------------------------------------------------------------------
void AFish::Draw_Pelvic_Fin_Front(HDC hdc, RECT &rect)
{
	rect.left += Pelvic_Fin_Size;
	rect.top += Pelvic_Fin_Size ;
	rect.right = rect.left + Pelvic_Fin_Size;
	rect.bottom = rect.top + Pelvic_Fin_Size;

	AsConfig::Yellow_Fish_Element_Color.Select(hdc);
	Chord(hdc, rect.left, rect.top, rect.right, rect.bottom,
		rect.left + (Offset - 1), rect.top + (Offset - 1),
		rect.right - (Offset - 1), rect.bottom - (Offset - 1));
}
//------------------------------------------------------------------------------------------------------------
void AFish::Draw_Pelvic_Fin_Back(HDC hdc, RECT &rect)
{
	rect.left = X_Pos + Pelvic_Fin_Size / 2;
	rect.top = Y_Pos + Pelvic_Fin_Size;
	rect.right = rect.left + Pelvic_Fin_Size;
	rect.bottom = rect.top + Pelvic_Fin_Size;

	AsConfig::Yellow_Fish_Element_Color.Select(hdc);
	Chord(hdc, rect.left, rect.top, rect.right, rect.bottom,
		rect.left + Pelvic_Fin_Size / 2, rect.top,
		rect.right - Pelvic_Fin_Size / 2, rect.bottom);
}
//------------------------------------------------------------------------------------------------------------
void AFish::Draw_Back_Fin(HDC hdc, RECT &rect)
{
	rect.left += Width / 3 - 2;
	rect.top -= Fin_Size / 2;
	rect.right = rect.left + Fin_Size;
	rect.bottom = rect.top + Fin_Size;

	AsTools::Ellipse(hdc, rect, AsConfig::Yellow_Fish_Element_Color);
}
//------------------------------------------------------------------------------------------------------------
void AFish::Update_Move_Diraction(EFish_State state)
{
	if (state == EFish_State::Move_Left)
		Fish_State = EFish_State::Move_Right;
	else if (state == EFish_State::Move_Right)
		Fish_State = EFish_State::Move_Left;
	else
		return;
}
//------------------------------------------------------------------------------------------------------------
void AFish::Redraw()
{
	Fish_Rect.left = X_Pos - Tail_Size;
	Fish_Rect.top = Y_Pos - Fin_Size;
	Fish_Rect.right = Fish_Rect.left + Pelvic_Fin_Size + Width + Fin_Size;
	Fish_Rect.bottom = Fish_Rect.top + Fin_Size + Height + Pelvic_Fin_Size + Eye_Size;

	AsTools::Invalidate_Rect(Fish_Rect);
}
//------------------------------------------------------------------------------------------------------------
