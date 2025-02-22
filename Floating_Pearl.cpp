

#include "Floating_Pearl.h"

// AFloating_Pearl
int AFloating_Pearl::All_Letters_Popularity;
int AFloating_Pearl::Letters_Popularity[ (int)ELetter_Type::Max] = { 7, 7, 7, 7, 3, 3, 1 };
//------------------------------------------------------------------------------------------------------------
AFloating_Pearl::AFloating_Pearl(int x_pos, int y_pos)
 : Letter_Type(ELetter_Type::R), Pearl_State(EPearl_State::idle), X_Pos(x_pos), Y_Pos(y_pos), Start_Y_Pos(y_pos), Start_Tick(0), Speed(3.0), Pearl_Rect{}
{
}
//------------------------------------------------------------------------------------------------------------
void AFloating_Pearl::Act()
{
	switch (Pearl_State)
	{
	case EPearl_State::idle:
		Y_Pos = Start_Y_Pos;
		Pearl_State = EPearl_State::Birth;
		break;
	
	case EPearl_State::Birth:
		break;
	
	case EPearl_State::Floating_Up:
		Floating();
		break;

	case EPearl_State::Escape:
		Y_Pos = Max_Floating_Height;
		Pearl_State = EPearl_State::idle;
		break;

	default:
		AsConfig::Throw();
	}

	Redraw();
}
//------------------------------------------------------------------------------------------------------------
void AFloating_Pearl::Draw(HDC hdc, RECT &paint_area)
{
	RECT rect;
	RECT intersection_rect;
	ELetter_Type leter_type;

	if (! IntersectRect(&intersection_rect, &paint_area, &Pearl_Rect) )
		return;

   rect.left = X_Pos;
	rect.top = Y_Pos;
	rect.right = rect.left + Width;
	rect.bottom = rect.top + Height;

	AsTools::Ellipse(hdc, rect, AsConfig::Grey_Color);

	rect.left += 1;
	rect.top += 1;
	rect.right -= 1;
	rect.bottom -= 1;

	AsTools::Ellipse(hdc, rect, AsConfig::White_Color);

   leter_type = ELetter_Type::E;
	//leter_type = Get_Random_Letter_Type();

   Draw_Letter_Type(hdc, leter_type, rect);
}
//------------------------------------------------------------------------------------------------------------
void AFloating_Pearl::Init()
{
	int i;

	All_Letters_Popularity = 0;

	for (i = 0; i < (int)ELetter_Type::Max; i++)
		All_Letters_Popularity += Letters_Popularity[i];
}
//------------------------------------------------------------------------------------------------------------
bool AFloating_Pearl::Is_Finished()
{
	if (Y_Pos <= Max_Floating_Height)
		return true;
	else
		return false;
}
//------------------------------------------------------------------------------------------------------------
void AFloating_Pearl::Set_Pearl_State(EPearl_State new_state)
{
	Pearl_State = new_state;
}
//------------------------------------------------------------------------------------------------------------
EPearl_State AFloating_Pearl::Get_State()
{
	return Pearl_State;
}
//------------------------------------------------------------------------------------------------------------
ELetter_Type AFloating_Pearl::Get_Random_Letter_Type()
{
	int letter;
	letter = AsTools::Rand(8);

	return (ELetter_Type)letter;
}
//------------------------------------------------------------------------------------------------------------
void AFloating_Pearl::Draw_Letter_Type(HDC hdc, ELetter_Type letter_type, RECT &rect)
{
   const int scale = AsConfig::Global_Scale;

   if (letter_type != ELetter_Type::R)
      Letter_Type = letter_type;

   switch (Letter_Type)
   {
   case ELetter_Type::C: // Speed
		Draw_C(hdc, rect, AsConfig::Red_Line);
      break;

   case ELetter_Type::G: // Golden
		Draw_C(hdc, rect, AsConfig::Golden_Color);
		AsConfig::Golden_Color_Pen.Select_Pen(hdc);
		MoveToEx(hdc, rect.right - 6, rect.bottom - 1, 0);
		LineTo(hdc, rect.right - 6, rect.bottom - 5);
		LineTo(hdc, rect.right - 4, rect.bottom - 5);
		LineTo(hdc, rect.right - 10, rect.bottom - 5);
      break;

   case ELetter_Type::H: // Поднимает рыб любого веса
		AsConfig::BG_Pen.Select_Pen(hdc);
		MoveToEx(hdc, rect.right - 7, rect.bottom - 6, 0);
		LineTo(hdc, rect.right - 7, rect.top + 5);
		MoveToEx(hdc, rect.left + 6, rect.bottom - 6, 0);
		LineTo(hdc, rect.left + 6, rect.top + 5);
		MoveToEx(hdc, rect.left + 6, rect.top + 11, 0);
		LineTo(hdc, rect.right - 7, rect.top + 11);
		break;

   case ELetter_Type::A: // возможность ловить акул
		AsConfig::Grey_Color_Lettter_Pen.Select_Pen(hdc);
		MoveToEx(hdc, rect.left + Width / 2 - 1, rect.top + 2, 0);
		LineTo(hdc, rect.right - 5, rect.bottom - 6);
		MoveToEx(hdc, rect.left + Width / 2 - 1, rect.top + 2, 0);
		LineTo(hdc, rect.left + 5, rect.bottom - 6);
		MoveToEx(hdc, rect.left + 7, rect.top + 11, 0);
		LineTo(hdc, rect.right - 7, rect.top + 11);
      break;

   case ELetter_Type::E: // Электрошок
		AsConfig::Violet_Color_Pen.Select_Pen(hdc);
		// 1. Прямая вертикальная линия
		MoveToEx(hdc, rect.left + 7, rect.bottom - 6, 0);
		LineTo(hdc, rect.left + 7, rect.top + 6);
		// 2. Верхняя горизонтальная линия
		MoveToEx(hdc, rect.left + 7, rect.top + 6, 0);
		LineTo(hdc, rect.left + Width / 2 + 1, rect.top + 6);
		// 3. Средняя горизонтальная линия
		MoveToEx(hdc, rect.left + 7, rect.top + 11, 0);
		LineTo(hdc, rect.left + Width / 2 + 1, rect.top + 11);
		// 4. Нижняя горизонтальная линия
		MoveToEx(hdc, rect.left + 7, rect.bottom - 6, 0);
		LineTo(hdc, rect.left + Width / 2 + 1, rect.bottom - 6);
      break;

   case ELetter_Type::O: // отменяет эффекты
		Draw_O(hdc, rect, AsConfig::Shell_Color);
      break;

   case ELetter_Type::L: // Жизнь, дополнительный крючек
      AsConfig::Green_Letter_Color_Pen.Select(hdc);
      MoveToEx(hdc, rect.left + Width / 3, rect.top + 5, 0);
      LineTo(hdc, rect.left + Width / 3, rect.top + Height / 2 + 4);
      LineTo(hdc, rect.left + Width / 2 + 2, rect.top + Height / 2 + 4); 
      break;

   case ELetter_Type::R: // обычное состояние, без эффектов
      break;

   default:
      AsConfig::Throw();
   }
}
//------------------------------------------------------------------------------------------------------------
void AFloating_Pearl::Draw_O(HDC hdc, RECT &rect, const AColor &color)
{
	const int scale = AsConfig::Global_Scale;

	rect.left += scale;
	rect.top += scale;
	rect.right -= scale;
	rect.bottom -= scale;

	AsTools::Ellipse(hdc, rect, color);

	rect.left += scale;
	rect.top += scale;
	rect.right -= scale;
	rect.bottom -= scale;

	AsTools::Ellipse(hdc, rect, AsConfig::White_Color);
}
//------------------------------------------------------------------------------------------------------------
void AFloating_Pearl::Draw_C(HDC hdc, RECT &rect, const AColor &color)
{
	const int scale = AsConfig::Global_Scale;

	rect.left += scale;
	rect.top += scale;
	rect.right -= scale;
	rect.bottom -= scale;

	AsTools::Ellipse(hdc, rect, color);
	
	rect.left += scale;
	rect.top += scale;
	rect.right += scale - 1;
	rect.bottom -= scale;

	AsTools::Ellipse(hdc, rect, AsConfig::White_Color);
}
//------------------------------------------------------------------------------------------------------------
void AFloating_Pearl::Floating()
{
	if (Y_Pos >= Max_Floating_Height)
		Y_Pos -= Speed;	// вещественный тип к инту, пока преобразовывать не будем, т.к. скорость может меняться, но нужно исправить в будущем!
	else
		Pearl_State = EPearl_State::Escape;
}
//------------------------------------------------------------------------------------------------------------
void AFloating_Pearl::Update_Y_Pos()
{
	Y_Pos = Start_Y_Pos;
}
//------------------------------------------------------------------------------------------------------------
//void AFloating_Pearl::Draw_Line_To(HDC hdc, int x, int y)
//{
//	int end_y = y * AsConfig::Global_Scale - Height / 2;
//
//	if (y == 6)
//		--end_y;
//
//	LineTo(hdc, x * AsConfig::Global_Scale + 1, end_y);
//}
//------------------------------------------------------------------------------------------------------------
void AFloating_Pearl::Redraw()
{
	Pearl_Rect.left = X_Pos;
	Pearl_Rect.top = Y_Pos;
	Pearl_Rect.right = Pearl_Rect.left + Width;
	Pearl_Rect.bottom = Pearl_Rect.top + Height + AsConfig::Global_Scale;

	AsTools::Invalidate_Rect(Pearl_Rect);
}
//------------------------------------------------------------------------------------------------------------
