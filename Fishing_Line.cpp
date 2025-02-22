#include "Fishing_Line.h"


// AsLine_Observer
//------------------------------------------------------------------------------------------------------------
AsLine_Observer::~AsLine_Observer()
{
}
//------------------------------------------------------------------------------------------------------------
AsLine_Observer::AsLine_Observer()
{
}
//------------------------------------------------------------------------------------------------------------
bool AsLine_Observer::Mirror()
{
   return false;
}
//------------------------------------------------------------------------------------------------------------
void AsLine_Observer::Act_Line()
{
}
//------------------------------------------------------------------------------------------------------------
void AsLine_Observer::Add_Line()
{
}
//------------------------------------------------------------------------------------------------------------
void AsLine_Observer::Remove_Line()
{
}
//------------------------------------------------------------------------------------------------------------





// AsFishing_Line
//------------------------------------------------------------------------------------------------------------
AsFishing_Line::~AsFishing_Line()
{
   if (Fishing_Hook)
   {
      delete Fishing_Hook;
      Fishing_Hook = 0;
   }
}
//------------------------------------------------------------------------------------------------------------
AsFishing_Line::AsFishing_Line(double x_pos, double y_pos)
   : Fishing_Hook(0), X_Pos(x_pos), Y_Pos(y_pos), Line_Length( (double)Start_Lenght), Speed(3.0),
   Line_State(ELine_State::Idle), Line_Rect{}, Prev_Line_Rect{}
{
	Fishing_Hook = new AsFishing_Hook(X_Pos, Y_Pos + (double)Get_Curr_Line_Lenght() );
}
//------------------------------------------------------------------------------------------------------------
void AsFishing_Line::Act()
{
   switch (Line_State)
   {
   case ELine_State::Idle:
      break;

   case ELine_State::Descending:
      Raise_Or_Lower_Hook(true);
      break;

   case ELine_State::Lifting:
      Raise_Or_Lower_Hook(false);
      break;

   case ELine_State::Finished:
      Line_State = ELine_State::Idle;
      break;

   default:
      AsConfig::Throw();
   }

   Redraw();
}
//------------------------------------------------------------------------------------------------------------
void AsFishing_Line::Draw(HDC hdc, RECT &paint_area)
{
   RECT intersection_rect;

   Double_Draw(hdc, paint_area);   // Используем для ументшения мигания при смещении всей лодки

   if (! IntersectRect(&intersection_rect, &paint_area, &Line_Rect) )
      return;

   AsConfig::White_Color.Select(hdc);
   MoveToEx(hdc, (int)X_Pos, (int)Y_Pos, 0);
   LineTo(hdc, (int)X_Pos, (int)Y_Pos + Get_Curr_Line_Lenght());
}
//------------------------------------------------------------------------------------------------------------
bool AsFishing_Line::Allowed_Mirror()
{
   return (Line_State == ELine_State::Descending || Line_State == ELine_State::Lifting);
}
//------------------------------------------------------------------------------------------------------------
void AsFishing_Line::Active_Line()
{
   //Start_Timer_Tick = AsConfig::Current_Timer_Tick;

   if (Line_State == ELine_State::Descending)
      Line_State = ELine_State::Lifting;
   else
      if (Line_State != ELine_State::Lifting && Line_State == ELine_State::Idle)
         Line_State = ELine_State::Descending;
}
//------------------------------------------------------------------------------------------------------------
void AsFishing_Line::Move(bool to_left)
{
   Fishing_Hook->Move(to_left);

   Redraw();
}
//------------------------------------------------------------------------------------------------------------
void AsFishing_Line::Set_Position(double x_pos, double y_pos)
{
   X_Pos = x_pos;
   Y_Pos = y_pos;

   Fishing_Hook->Set_Position(X_Pos, Y_Pos + Line_Length);
}
//------------------------------------------------------------------------------------------------------------
double AsFishing_Line::Get_Curr_Line_Lenght() const
{
   return Line_Length;
}
//------------------------------------------------------------------------------------------------------------
ELine_State AsFishing_Line::Get_Line_State()
{
   return Line_State;
}
//------------------------------------------------------------------------------------------------------------
void AsFishing_Line::Raise_Or_Lower_Hook(bool is_dropping_down)
{
   if (is_dropping_down)  // Опускаем крючок
   {
      if (Line_Length < Max_Y_Pos)
      {
         Line_Length += Speed;
         Fishing_Hook->Set_Position(X_Pos, Y_Pos + Line_Length);
      }

      if (Line_Length >= Max_Y_Pos)  // Достигли нижней границы
         Line_State = ELine_State::Lifting;
   }
   else  // Поднимаем крючок
   {
      if (Line_Length > Start_Lenght)
      {
         Line_Length -= Speed;
         Fishing_Hook->Set_Position(X_Pos, Y_Pos + Line_Length);
      }

      if (Line_Length <= Start_Lenght)  // Достигли верхней границы
         Line_State = ELine_State::Finished;
   }
}
//------------------------------------------------------------------------------------------------------------
void AsFishing_Line::Redraw()
{
   Line_Rect.left = (int)X_Pos;
   Line_Rect.top = (int)Y_Pos;
   Line_Rect.right = Line_Rect.left + 1;
   Line_Rect.bottom = Line_Rect.top + Get_Curr_Line_Lenght() + 1;

   AsTools::Invalidate_Rect(Prev_Line_Rect);
   AsTools::Invalidate_Rect(Line_Rect);
}
////------------------------------------------------------------------------------------------------------------
void AsFishing_Line::Double_Draw(HDC hdc, RECT &paint_area)
{  // Для избежания мерцаний, двойная буферизация не справляется с количеством перерисовок
   Prev_Line_Rect = Line_Rect;

   RECT intersection_rect;

   if (! IntersectRect(&intersection_rect, &paint_area, &Prev_Line_Rect) )
      return;

   AsTools::Rect(hdc, Prev_Line_Rect, AsConfig::White_Color);
}
//------------------------------------------------------------------------------------------------------------