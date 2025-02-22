#include "Fishing_Hook.h"

// AsAngle_Calculator
const double AsAngle_Math::Max_Angle = M_PI - M_PI / 11.0;
const double AsAngle_Math::Min_Angle = M_PI - M_PI / 2.2;
//------------------------------------------------------------------------------------------------------------
APoint AsAngle_Math::Calculate_Rotation(double elem_lenght, double curr_x_pos, double curr_y_pos)
{
   APoint points;
   // Вычисляем координаты начала и конца сегмента удочки с учетом угла
   points.x_pos = curr_x_pos + elem_lenght * sin(Current_Angle() );
   points.y_pos = curr_y_pos + elem_lenght * cos(Current_Angle() );

   return points;
}
////------------------------------------------------------------------------------------------------------------
double AsAngle_Math::Current_Angle()
{
   //return (Max_Angle) - Min_Angle / 2.0;
   //return (Max_Angle) - Min_Angle / 6.0;
   return (Min_Angle);
   //return (Max_Angle);
}
//------------------------------------------------------------------------------------------------------------




// AsFishing_Hook
AsFishing_Hook::~AsFishing_Hook()
{
}
//------------------------------------------------------------------------------------------------------------
AsFishing_Hook::AsFishing_Hook(double x_pos, double y_pos)
	:X_Pos(x_pos), Y_Pos(y_pos), Hook_Rect{}, Prev_Hook_Rect{}
{
}
//------------------------------------------------------------------------------------------------------------
void AsFishing_Hook::Set_Position(double x_pos, double y_pos)
{
   X_Pos = x_pos;
   Y_Pos = y_pos;
}
//------------------------------------------------------------------------------------------------------------
void AsFishing_Hook::Move(bool to_left)
{
  Redraw();
}
//------------------------------------------------------------------------------------------------------------
void AsFishing_Hook::Act()
{
   // Need to done
   Redraw();
}
//------------------------------------------------------------------------------------------------------------
void AsFishing_Hook::Draw(HDC hdc, RECT &paint_area)
{
   RECT hook_rect;
   RECT intersection_rect;
   XFORM original_xform, mirror_xform;

   // Сохранение текущей матрицы трансформации
   GetWorldTransform(hdc, &original_xform);

   hook_rect.left = (int)X_Pos - 1;
   hook_rect.top = (int)Y_Pos;
   hook_rect.right = hook_rect.left + Hook_Radius / 2;
   hook_rect.bottom = hook_rect.top + Hook_Radius / 2;

   mirror_xform.eM11 = -1.0f; // Отражение по горизонтали
   mirror_xform.eM12 = 0.0f;
   mirror_xform.eM21 = 0.0f;
   mirror_xform.eM22 = 1.0f;
   mirror_xform.eDx = float(2.0 * X_Pos - double(Hook_Radius / 3)); // Корректируем смещение    // !!!
   mirror_xform.eDy = 0.0f;

   if (! IntersectRect(&intersection_rect, &paint_area, &Hook_Rect) )
      return;

   // Применение зеркального отражения
   if (AsConfig::Mirror_Togle)
      SetWorldTransform(hdc, &mirror_xform);

   AsTools::Ellipse(hdc, hook_rect, AsConfig::Grey_Color); // 1. Место крепления крючка к леске
   Draw_Shank(hdc, paint_area, hook_rect);  // 2. Цевье
   Draw_Hook(hdc, paint_area, hook_rect);   // 3. Крючек

   SetWorldTransform(hdc, &original_xform); // Восстановление оригинальной матрицы трансформации
}
//------------------------------------------------------------------------------------------------------------
bool AsFishing_Hook::Is_Mirror_Allowed()
{
   return false;  // Заглушка, метод не используется
}
//------------------------------------------------------------------------------------------------------------
void AsFishing_Hook::Draw_Shank(HDC hdc, RECT &paint_area, RECT &hook_rect)
{
   AsConfig::Grey_Color_Pen.Select_Pen(hdc);
   MoveToEx(hdc, (int)X_Pos, hook_rect.bottom, 0);
   LineTo(hdc, (int)X_Pos, hook_rect.bottom + Shank_Length);
}
//------------------------------------------------------------------------------------------------------------
void AsFishing_Hook::Draw_Hook(HDC hdc, RECT &paint_area, RECT &hook_rect)
{
   int x_pos = hook_rect.left - Hook_Radius + 1;
   int y_pos = hook_rect.bottom + Shank_Length - Hook_Radius / 2 - 1;

   Arc(hdc, x_pos, y_pos,
      x_pos + Hook_Radius, y_pos + Hook_Radius,
      x_pos + Hook_Radius / 2 - 1, y_pos + Hook_Radius / 2,
      x_pos + Hook_Radius, y_pos);
}
//------------------------------------------------------------------------------------------------------------
void AsFishing_Hook::Redraw()
{
   int offset;

   Prev_Hook_Rect = Hook_Rect;

   Hook_Rect.left = (int)X_Pos - Hook_Radius;
   Hook_Rect.top = (int)Y_Pos;
   Hook_Rect.right = Hook_Rect.left + Hook_Radius * 2 + 1;
   Hook_Rect.bottom = Hook_Rect.top + Shank_Length + Hook_Radius + 1;

   AsTools::Invalidate_Rect(Prev_Hook_Rect);
   AsTools::Invalidate_Rect(Hook_Rect);
}
////------------------------------------------------------------------------------------------------------------
