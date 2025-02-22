#include "Fishing_Rod.h"

// AFishing_Rod
//------------------------------------------------------------------------------------------------------------
AFishing_Rod::~AFishing_Rod()
{
	if (Fishing_Line)
	{
		delete Fishing_Line;
		Fishing_Line = 0;
	}
}
//------------------------------------------------------------------------------------------------------------
AFishing_Rod::AFishing_Rod(double x_pos, double y_pos)
 : Fishing_Line(0), X_Pos(x_pos), Y_Pos(y_pos), Rod_Rect{}
{
	APoint line_points = Angle_Math.Calculate_Rotation((double)Length, X_Pos, Y_Pos);
	Fishing_Line = new AsFishing_Line(line_points.x_pos, line_points.y_pos);
}
//------------------------------------------------------------------------------------------------------------
void AFishing_Rod::Act()
{
	// Need to done !!!
	Redraw();
}
//------------------------------------------------------------------------------------------------------------
void AFishing_Rod::Draw(HDC hdc, RECT &paint_area)
{
	float offset = 4.0f;
	RECT intersection_rect;
	XFORM original_xform{}, mirror_xform{};

	// Сохранение текущей матрицы трансформации
	GetWorldTransform(hdc, &original_xform);

	mirror_xform.eM11 = -1.0f; // Отражение по горизонтали
	mirror_xform.eM12 = 0.0f;
	mirror_xform.eM21 = 0.0f;
	mirror_xform.eM22 = 1.0f;
	mirror_xform.eDx = (float)(2.0 * X_Pos) - (float)(Length / 3); // Корректируем смещение
	mirror_xform.eDy = 0.0f;

	if (AsConfig::Mirror_Togle)
	{
		SetWorldTransform(hdc, &mirror_xform);	// Применение матрицы трансформации
		Update_Line_Pos(mirror_xform);
	}
	else
		Update_Line_Pos(original_xform);

	if (IntersectRect(&intersection_rect, &paint_area, &Rod_Rect) )
		return;

	Draw_Rod(hdc, paint_area);

	// Восстановление оригинальной матрицы трансформации
	SetWorldTransform(hdc, &original_xform);
}
//------------------------------------------------------------------------------------------------------------
//bool AFishing_Rod::Is_Mirror_Allowed()
//{
//	return false;	// Заглушка, метод не используется
//}
//------------------------------------------------------------------------------------------------------------
void AFishing_Rod::Draw_Rod(HDC hdc, RECT &paint_area)
{
	int i;
	int pen_size = 4;
	double rod_elem_len = (double)(Length / 3);
	double x_pos = X_Pos;
	double y_pos = Y_Pos;
	APoint end_points;

	for (i = 0; i < 3; i++)
	{
		// 1. На кажой итерации увеличиваем толщину карандаша (создавть вилдимости спининга)
		AColor fichin_rod_pen(AsConfig::Grey_Color, pen_size - i );
		fichin_rod_pen.Select_Pen(hdc);

		// 2. Высчитываем угол наклона удочки
		end_points = Angle_Math.Calculate_Rotation(rod_elem_len, x_pos, y_pos);

		MoveToEx(hdc, (int)(x_pos), (int)(y_pos), 0);
		LineTo(hdc, (int)(end_points.x_pos), (int)(end_points.y_pos));

		x_pos = end_points.x_pos;
		y_pos = end_points.y_pos;
	}
}
//------------------------------------------------------------------------------------------------------------
void AFishing_Rod::Move(bool to_left)
{
	double speed = AsConfig::Speed_Move * AsConfig::D_Global_Scale;

	if (to_left)
		X_Pos -= speed;
	else
		X_Pos += speed;

	Fishing_Line->Move(to_left);

	Redraw();
}
//------------------------------------------------------------------------------------------------------------
void AFishing_Rod::Set_X_Pos(double x_pos)
{
	X_Pos = x_pos * AsConfig::D_Global_Scale;
}
//------------------------------------------------------------------------------------------------------------
void AFishing_Rod::Apply_Coordinates(APoint &transformed_point, XFORM &curr_xform)
{
	transformed_point = Angle_Math.Calculate_Rotation((double)Length, X_Pos, Y_Pos);

	// Применение трансформации
	transformed_point.x_pos = curr_xform.eM11 * transformed_point.x_pos + curr_xform.eDx;
	transformed_point.y_pos = curr_xform.eM22 * transformed_point.y_pos + curr_xform.eDy;
}
//------------------------------------------------------------------------------------------------------------
void AFishing_Rod::Update_Line_Pos(XFORM &curr_xform)
{
	APoint curr_point{};
	curr_point.x_pos = X_Pos;
	curr_point.y_pos = Y_Pos;

	Apply_Coordinates(curr_point, curr_xform);

	Fishing_Line->Set_Position(curr_point.x_pos, curr_point.y_pos);
}
//------------------------------------------------------------------------------------------------------------
void AFishing_Rod::Redraw() // !!! Исправить большой диапазон перерисовки
{

	// Диапазон перерисовки удочки
	Rod_Rect.left = (int)X_Pos - Redraw_Length;
	Rod_Rect.top = (int)Y_Pos + AsConfig::Global_Scale; // добавляем 1 игровой пиксель, чтобы избежать артефактов от удочки и руки рыбака.
	Rod_Rect.right = Rod_Rect.left + (Redraw_Length - 15) * 2;
	Rod_Rect.bottom = Rod_Rect.top - Length;

	AsTools::Invalidate_Rect(Rod_Rect);
}
//------------------------------------------------------------------------------------------------------------
