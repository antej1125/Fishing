#include "Boat.h"


// AsFluctuation
//------------------------------------------------------------------------------------------------------------
AsFluctuation::AsFluctuation()
	: Start_Tick(0), Fluctuation_Height(2.0), Fluctuation_Pos(0.0), Fluctuation(EFluctuation_State::idle)
{
}
//------------------------------------------------------------------------------------------------------------
double AsFluctuation::Set_For_Pos(double fluctuation_pos)
{
	Fluctuation_Pos = fluctuation_pos;
	Start_Tick = AsConfig::Current_Timer_Tick; // Обновляем тик начала
	//Fluctuation = EFluctuation_State::Fluctuation_Up; // Стартуем с движения вверх

	return Act_Fluctuation();
}
//------------------------------------------------------------------------------------------------------------
double AsFluctuation::Act_Fluctuation()
{
	int time_offset = AsConfig::Current_Timer_Tick - Start_Tick;

	double ratio = (double)time_offset / (double)Fluctuation_Timeot;

	// 2.0 * M_PI - полный период синусоиды для плавных колебаний между положительными и отрицательными значениями
	Fluctuation_Pos += Fluctuation_Height * sin(2.0 * M_PI * ratio);

	// Циклически обновляем фазу, чтобы продолжать колебания
	if (time_offset >= Fluctuation_Timeot)
		Start_Tick = AsConfig::Current_Timer_Tick; // Перезапускаем цикл колебаний

	return Fluctuation_Pos / 5.0;
}//------------------------------------------------------------------------------------------------------------




// AsBoat
//------------------------------------------------------------------------------------------------------------
AsBoat::~AsBoat()
{
	if (Fisherman)
	{
		delete Fisherman;
		Fisherman = 0;
	}
}
//------------------------------------------------------------------------------------------------------------
AsBoat::AsBoat()
 : Fisherman(0), Left_Key_Down(false), Right_Key_Down(false), 
	X_Pos((double)(AsConfig::Sea_Widht / 2 - Width / 2)), Y_Pos((double)Start_Y_Pos),
	Speed(0.0), Moving(EBoat_Moving_State::Stop), Boat_Rect{}
{
	Fisherman = new AsFisherman(X_Pos + Fisher_X_Pos, Y_Pos - Fisher_Y_Pos);
}
//------------------------------------------------------------------------------------------------------------
void AsBoat::Act()
{
	switch (Moving)
	{
	case EBoat_Moving_State::Stopping:
		break;

	case EBoat_Moving_State::Stop:
		break;

	case EBoat_Moving_State::Moving_Left:
		break;

	case EBoat_Moving_State::Moving_Right:
		break;

	case EBoat_Moving_State::Swing_Boat:
		Y_Pos = (double)(Start_Y_Pos - 1) + Fluctuation.Act_Fluctuation();
		break;

	default:
		AsConfig::Throw();
	}

	Redraw();
}
//------------------------------------------------------------------------------------------------------------
void AsBoat::Draw(HDC hdc, RECT &paint_area)
{
	RECT intersection_rect;

	//Moving = EBoat_Moving_State::Swing_Boat;

	if (IntersectRect(&intersection_rect, &paint_area, &Boat_Rect))
		return;

	Draw_Boat_Base(hdc, paint_area);
	Draw_Half_Ellipse(hdc, paint_area, AsConfig::Brown_Color);	// рисуем полуэлипс справа
	Draw_Asymmetric_Trapezoid(hdc, paint_area, AsConfig::Brown_Color);	// Асиметрическая трапеция для изображения угла лодки слева
	Draw_Boards(hdc, paint_area);
	Draw_Gunwale(hdc, paint_area);	// фальшборт
}
//------------------------------------------------------------------------------------------------------------
void AsBoat::Move(bool to_left, bool key_down)
{
	if (to_left)
		Left_Key_Down = key_down;
	else
		Right_Key_Down = key_down;

	if ( (Left_Key_Down && Right_Key_Down) || (!Left_Key_Down && !Right_Key_Down)) // Обе клавиши нажаты или отжаты - мы ничего не делаем
		return;

	if (to_left)
		X_Pos -= AsConfig::Speed_Move;
	else
		X_Pos += AsConfig::Speed_Move;

	if (Check_Boat_Bounds() )	// Проверяем границы допустимые для лодки
		return;

	Fisherman->Move(to_left);
	Fisherman->Set_X_Pos(X_Pos + Fisher_X_Pos); // корректируем сразу позицию рыбака после его движения, чтобы он не смещался по Х позиции лодки

	Redraw();
}
//------------------------------------------------------------------------------------------------------------
void AsBoat::Draw_Boat_Base(HDC hdc, RECT &paint_area)
{
	const int scale = AsConfig::Global_Scale;
	RECT rect;

	rect.left = (int)X_Pos * scale;
	rect.top = (int)Y_Pos * scale;
	rect.right = rect.left + Width * scale;
	rect.bottom = rect.top - Height * scale;

	AsTools::Rect(hdc, rect, AsConfig::Brown_Color);		// прямоугольник основы лодки по средине
}
//------------------------------------------------------------------------------------------------------------
void AsBoat::Draw_Half_Ellipse(HDC hdc, RECT &paint_area, const AColor &color)
{
	const int scale = AsConfig::Global_Scale;
	int x_pos, y_pos;

	x_pos = ( (int)X_Pos + (Width - Boat_Elipse_Radius) ) * scale;
	y_pos = ( (int)Y_Pos - Boat_Elipse_Radius * 2) * scale - 1;

	color.Select(hdc);
	Chord(hdc,
		x_pos, y_pos, // Верхняя левая точка прямоугольника
		x_pos + Boat_Elipse_Radius * 2 * scale, y_pos + Boat_Elipse_Radius * 2 * scale, // Нижняя правая точка прямоугольника
		x_pos, y_pos + Boat_Elipse_Radius * scale, // Начальная точка дуги
		x_pos + Boat_Elipse_Radius * scale, y_pos + Boat_Elipse_Radius * scale // Конечная точка дуги
	);
}
//------------------------------------------------------------------------------------------------------------
void AsBoat::Draw_Asymmetric_Trapezoid(HDC hdc, RECT &paint_area, const AColor &color)
{
	const int scale = AsConfig::Global_Scale;
	const int vertices_trapezoid = 4;
	int x_pos, y_pos;
	
	x_pos = (int)X_Pos * scale;
	y_pos = (int)Y_Pos * scale;

	POINT trapezoid_points[] = {
		{x_pos - (Boat_Elipse_Radius) * scale, y_pos - Boat_Elipse_Radius * scale},
		{x_pos, y_pos - Boat_Elipse_Radius * scale},
		{x_pos, y_pos},
		{x_pos - (Boat_Elipse_Radius - scale + 1) * scale, y_pos - scale}
	};

	color.Select(hdc);
	Polygon(hdc, trapezoid_points, vertices_trapezoid);
}
//------------------------------------------------------------------------------------------------------------
void AsBoat::Draw_Gunwale(HDC hdc, RECT &paint_area)
{	
	const int scale = AsConfig::Global_Scale;
	int x_offset = scale - 1;
	RECT gunwale_rect;

	gunwale_rect.left = ( (int)X_Pos - Boat_Elipse_Radius - x_offset) * scale;
	gunwale_rect.top = ( (int)Y_Pos - Boat_Elipse_Radius) * scale;
	gunwale_rect.right = gunwale_rect.left + Gunwale_Width * scale - x_offset;
	gunwale_rect.bottom = gunwale_rect.top - Gunwale_Height * scale;

	AsConfig::Boat_Gunwale_Color.Select(hdc);
	AsTools::Round_Rect(hdc, gunwale_rect, 2);
}
//------------------------------------------------------------------------------------------------------------
void AsBoat::Draw_Boards(HDC hdc, RECT &paint_area)
{
	Draw_Horizontal_Boards(hdc, paint_area);
	Draw_Vertical_Boards(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
void AsBoat::Draw_Horizontal_Boards(HDC hdc, RECT &paint_area)
{
	const int scale = AsConfig::Global_Scale;
	int i;
	int x_pos, y_pos;
	int board_width;
	int offset = 3 * scale;

	x_pos = ( (int)X_Pos - Boat_Elipse_Radius) * scale + offset;
	y_pos = ( (int)Y_Pos - Boat_Elipse_Radius) * scale + offset - scale;

	board_width = Width * scale + (Boat_Elipse_Radius * 2) * scale - (offset * 2);

	AsConfig::Wooden_Board_Pen.Select_Pen(hdc);
	for (i = 0; i < 3; i++)
	{
		MoveToEx(hdc, x_pos + i * scale, y_pos + i * offset, 0);
		LineTo(hdc, x_pos + board_width - offset * i, y_pos + i * offset);
	}
}
//------------------------------------------------------------------------------------------------------------
void AsBoat::Draw_Vertical_Boards(HDC hdc, RECT &paint_area)
{
	const int scale = AsConfig::Global_Scale;
	int x_pos = (int)X_Pos * scale;
	int y_pos = (int)Y_Pos * scale;

	// Верткальные чтобы разделить горизтальные доски, создавая лёгкий эффект заокругления
	AsConfig::Brown_Color_Pen.Select_Pen(hdc);
	MoveToEx(hdc, x_pos + scale, y_pos - scale, 0);
	LineTo(hdc, x_pos + scale, y_pos - Vertical_Board_Height * scale);

	MoveToEx(hdc, x_pos + Vertical_Board_Width * scale, y_pos - scale, 0);
	LineTo(hdc, x_pos + Vertical_Board_Width * scale, y_pos - Vertical_Board_Height * scale);
}
//------------------------------------------------------------------------------------------------------------
bool AsBoat::Check_Boat_Bounds()
{
	bool got_correction = false;
	int rod_correction = 4;
	double min_x_pos = (double)(AsConfig::Border_X_Offset + Boat_Elipse_Radius + rod_correction);
	double max_x_pos = (double)AsConfig::Sea_Widht - (AsConfig::Border_X_Offset +  Width + Boat_Elipse_Radius + rod_correction);

	if (X_Pos >= max_x_pos)
	{
		X_Pos = max_x_pos;
		got_correction = true;
	}

	if (X_Pos <= min_x_pos)
	{
		X_Pos = min_x_pos;
		got_correction = true;
	}

	if (got_correction)
		Fisherman->Set_X_Pos(X_Pos + Fisher_X_Pos);


	return got_correction;
}
//------------------------------------------------------------------------------------------------------------
void AsBoat::Apply_Flunction()
{
	Fluctuation.Set_For_Pos(Y_Pos);
}
//------------------------------------------------------------------------------------------------------------
void AsBoat::Redraw()
{
	const int scale = AsConfig::Global_Scale;
	int gunwale_piece = 5;
	int x_offset = Boat_Elipse_Radius + gunwale_piece;

	Boat_Rect.left = ( (int)X_Pos - x_offset) * scale;
	Boat_Rect.top = (int)Y_Pos * scale + gunwale_piece;
	Boat_Rect.right = Boat_Rect.left + (Gunwale_Width + gunwale_piece) * scale + 1;
	Boat_Rect.bottom = Boat_Rect.top - (Boat_Elipse_Radius + gunwale_piece) * scale;

	AsTools::Invalidate_Rect(Boat_Rect);
}
//------------------------------------------------------------------------------------------------------------
