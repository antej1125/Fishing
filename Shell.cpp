#include "Shell.h"

// AShell
const double AShell::Max_Cap_Height = 4.0 * AsConfig::D_Global_Scale + 1.0;
//------------------------------------------------------------------------------------------------------------
AShell::~AShell()
{
	if (Pearl != 0)
		delete Pearl;
}
//------------------------------------------------------------------------------------------------------------
AShell::AShell(int x_pos, int y_pos)
 :X_Pos(x_pos), Y_Pos(y_pos), Pearl(0),Start_Tick(0), Shell_Cap_Angle(0.0), Cap_Height(Max_Cap_Height), Shell_Rect{},
	Shell_State(EShell_State::Closed), Prev_Shell_State(EShell_State::Empty)
{
	Pearl = new AFloating_Pearl(X_Pos + Base_Width / 3 - 1, Y_Pos - Base_Height  / 3);
}
//------------------------------------------------------------------------------------------------------------
void AShell::Act()
{
	int time_offset = AsConfig::Current_Timer_Tick - Start_Tick;
	double ratio = 0.0;
	double angle_range = M_PI_2;

	if (time_offset <= Shell_Timeout)
		ratio = (double)time_offset / (double)Shell_Timeout;

	switch (Shell_State)
	{
	case EShell_State::Empty:
		break;

	case EShell_State::Open:
		Cap_Height = Max_Cap_Height;
		
		if (Cap_Height >= Max_Cap_Height)
			Shell_State = EShell_State::Closing;
		break;

	case EShell_State::Opening:
		Cap_Height = 0.0;
		Shell_Cap_Angle = cos(ratio * angle_range);
		Cap_Height = Max_Cap_Height * ratio;
		
		if (Cap_Height >= Max_Cap_Height)
			Shell_State = EShell_State::Open;
		break;

	case EShell_State::Create_Pearl:
		break;

	case EShell_State::Emit_Pearl:
		Emit_At_Pearl();
		break;

	case EShell_State::Closing:
		Shell_Cap_Angle = sin(ratio * angle_range);
		Cap_Height = Max_Cap_Height * (1.0 - ratio);
		
		if (Cap_Height <= 0.0)
			Shell_State = EShell_State::Closed;
		break;

	case EShell_State::Closed:
		Cap_Height = 0.0;
		Delay_State_Change();
		Pearl->Set_Pearl_State(EPearl_State::Birth);
		Shell_State = EShell_State::Opening;
		break;

	default:
		AsConfig::Throw();
	}

	Delay_State_Change();

	if (Pearl != 0)
		Pearl->Act();

	Redraw();
}
//------------------------------------------------------------------------------------------------------------
void AShell::Draw(HDC hdc, RECT &paint_area)
{
	RECT intersection_rect;

	if (IntersectRect(&intersection_rect, &paint_area, &Shell_Rect) )
		return;

	Draw_Shell(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
void AShell::Draw_Shell(HDC hdc, RECT &paint_area)
{
	Draw_Base_Shell(hdc, paint_area);
	Draw_Cap(hdc, paint_area);
	Draw_Pearl(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
void AShell::Draw_Base_Shell(HDC hdc, RECT &paint_area)
{
	RECT base_shell_rect;

	base_shell_rect.left = X_Pos;
	base_shell_rect.top = Y_Pos;
	base_shell_rect.right = base_shell_rect.left + Base_Width;
	base_shell_rect.bottom = base_shell_rect.top + Base_Height;

	AsTools::Ellipse(hdc, base_shell_rect, AsConfig::Shell_Color);

	// Внутренняя часть
	base_shell_rect.left = X_Pos + Offset;
	base_shell_rect.top = Y_Pos + Offset;
	base_shell_rect.right = base_shell_rect.left + Base_Width - Offset * 2;
	base_shell_rect.bottom = base_shell_rect.top + Base_Height / 2 + 1;

	AsTools::Ellipse(hdc, base_shell_rect, AsConfig::Shell_Muscle_Color);

	Draw_Shell_Curves(hdc, base_shell_rect, AsConfig::Shell_Color, true);
}
//------------------------------------------------------------------------------------------------------------
void AShell::Draw_Cap(HDC hdc, RECT &paint_area)
{
	RECT cap_rect;

	// Основание ракушки
	cap_rect.left = X_Pos + (Offset - 1);
	cap_rect.top = Y_Pos + (Max_Cap_Height * Shell_Cap_Angle) + 1;
	cap_rect.right = cap_rect.left + Cap_Width;
	cap_rect.bottom = cap_rect.top - (Max_Cap_Height + Cap_Height);
	
	// Рисуем капсу, включая внутренние части
	AsTools::Ellipse(hdc, cap_rect, AsConfig::Shell_Cap_Color);

	// Внутренняя часть капсу
	cap_rect.left += Offset;
	cap_rect.top -= Offset / 2;
	cap_rect.right -= Offset;
	cap_rect.bottom += (Max_Cap_Height - Cap_Height / 2.0);

	AsTools::Ellipse(hdc, cap_rect, AsConfig::Shell_Cream_Color);

	// Рисуем кривые для верхней или нижней дуги, в зависимости от состояния
	if (Cap_Height  >= Max_Cap_Height / 2.0)
		Draw_Shell_Curves(hdc, cap_rect, AsConfig::Shell_Cap_Color, false);
	else
		Draw_Shell_Curves(hdc, cap_rect, AsConfig::Shell_Cap_Color, true);
}
//------------------------------------------------------------------------------------------------------------
void AShell::Draw_Pearl(HDC hdc, RECT &paint_area)
{
	int pearl_x_pos = X_Pos + Base_Width / 3 - 1;
	int pearl_y_pos = Y_Pos + Base_Height / 3 + Offset - 1;
	HRGN region;

	// Создаём регион обрезки
	region = CreateEllipticRgn(pearl_x_pos, pearl_y_pos, pearl_x_pos + AsConfig::Pearl_Size + 1, pearl_y_pos - (Cap_Height * 2.0) + 1 );
	
	if (Pearl->Get_State() == EPearl_State::Floating_Up)
		SelectClipRgn(hdc, 0);
	else
		SelectClipRgn(hdc, region);

	// при закрытии ракушки у на будет закрываться жемчужина
	if (Shell_State != EShell_State::Closing)
		Pearl->Draw(hdc, paint_area);

	SelectClipRgn(hdc, 0);
	DeleteObject(region);	// удаляем регион
}
//------------------------------------------------------------------------------------------------------------
void AShell::Draw_Shell_Curves(HDC hdc, RECT &rect, const AColor &color, bool use_mirror)
{	// рисуем кривые изгибы ракушки на концах
	int i;
	int max_height = rect.bottom - (use_mirror ? 7 : 2);
	int size = Curve_Size + (use_mirror ? 1 : 0);
	int offset_direction = use_mirror ? 1 : -1;
	int top_offset;

	if (Shell_State == EShell_State::Closing || Shell_State == EShell_State::Closed || Shell_State == EShell_State::Opening)
		size -= 1;

	for (i = 0; i < size; i++)
	{
		// Сдвиг rect.left на основе индекса
		rect.left += (i <= 1) ? 1 : Curve_Size + 1;
		// Вычисление rect.top в зависимости от того, на какой итерации цикла мы находимся
		top_offset = (i >= 4) ? (Curve_Size - i) : i;
		rect.top = max_height + offset_direction * top_offset;

		// Установка остальных размеров
		rect.right = rect.left + Curve_Size;
		rect.bottom = rect.top + Curve_Size;

		// Рисуем эллипс
		AsTools::Ellipse(hdc, rect, color);
	}
}
//------------------------------------------------------------------------------------------------------------
void AShell::Update_State(EShell_State shell_state)
{
	//// Включаем ракушку
	//if (Shell_State == EShell_State::Empty)
	//	Shell_State = EShell_State::Open;

	// Выпускаем жемчужину
	if (Shell_State == EShell_State::Open && Prev_Shell_State == EShell_State::Opening)
		Shell_State = EShell_State::Emit_Pearl;

	// Переход на закрытие
	//if (Cap_Height <= 0.0 && Shell_State == EShell_State::Closing)
	//	Shell_State = EShell_State::Closed;
	
	// Переход на открытие
	//if (Cap_Height >= Max_Cap_Height && Shell_State == EShell_State::Opening)
	//	Shell_State = EShell_State::Open;

	Prev_Shell_State = Shell_State;
}
//------------------------------------------------------------------------------------------------------------
void AShell::Emit_At_Pearl()
{
	if (Shell_State == EShell_State::Emit_Pearl)
	{
		Pearl->Set_Pearl_State(EPearl_State::Floating_Up);

		if (Pearl->Is_Finished() )
			Shell_State = EShell_State::Closing;
	}
}
//------------------------------------------------------------------------------------------------------------
void AShell::Delay_State_Change()
{
	int delay_time = AsConfig::Current_Timer_Tick - Start_Tick;
	//int rand_shell_state = 1; // !!! decide what to do later
	double ratio = 0.0;

	if (delay_time <= Waiting_Timeout)
		ratio = (double)delay_time / (double)Waiting_Timeout;
	else
	{
		//if (Shell_State == EShell_State::Open)
		//	Shell_State = EShell_State::Closing;

		//if (Shell_State == EShell_State::Closed)
		//	Shell_State = EShell_State::Opening;

		Start_Tick = AsConfig::Current_Timer_Tick;
	}

	Update_State(Shell_State);
}
//------------------------------------------------------------------------------------------------------------
void AShell::Redraw()
{
	Shell_Rect.left = X_Pos;
	Shell_Rect.top = Y_Pos + Base_Height;
	Shell_Rect.right = Shell_Rect.left + Base_Width;
	Shell_Rect.bottom = Shell_Rect.top - Base_Height * 2;

	AsTools::Invalidate_Rect(Shell_Rect);
}
//------------------------------------------------------------------------------------------------------------
