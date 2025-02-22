
#include "framework.h"
#include "Main.h"

// AsFrame_DC
//------------------------------------------------------------------------------------------------------------
AsFrame_DC::~AsFrame_DC()
{
	if (Bitmap != 0)
		DeleteObject(Bitmap);

	if (DC != 0)
		DeleteObject(DC);
}
//------------------------------------------------------------------------------------------------------------
AsFrame_DC::AsFrame_DC()
	: Width(0), Height(0), DC(0), Bitmap(0)
{
}
//------------------------------------------------------------------------------------------------------------
HDC AsFrame_DC::Get_DC(HWND hwnd, HDC hdc)
{
	int dc_width, dc_height;
	RECT rect;

	GetClientRect(hwnd, &rect);

	dc_width = rect.right - rect.left;
	dc_height = rect.bottom - rect.top;

	if (dc_width != Width && dc_height != Height)
	{
		if (Bitmap != 0)
			DeleteObject(Bitmap);

		if (DC != 0)
			DeleteObject(DC);

		Width = dc_width;
		Height = dc_height;

		DC = CreateCompatibleDC(hdc);
		Bitmap = CreateCompatibleBitmap(hdc, Width, Height);
		SelectObject(DC, Bitmap);

		++rect.right;
		++rect.bottom;

		AsTools::Rect(DC, rect, AsConfig::BG_Color);
	}

	return DC;
}
//------------------------------------------------------------------------------------------------------------



// Global Variables:
AsMain_Window Main_Window;
//------------------------------------------------------------------------------------------------------------
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)

{
	return Main_Window.Main(hInstance, nCmdShow);
}
//------------------------------------------------------------------------------------------------------------




// AsMain_Window
AsMain_Window *AsMain_Window::Self = 0;
//------------------------------------------------------------------------------------------------------------
AsMain_Window::AsMain_Window()
	: Instance(0)
{
	Self = this;
}
//------------------------------------------------------------------------------------------------------------
int APIENTRY AsMain_Window::Main(HINSTANCE instance, int command_show)
{
	HACCEL accel_table;
	MSG msg;
	wchar_t str_buf[Max_String_Size];

	Instance = instance; // Store instance handle in our global variable

	// Initialize global strings
	LoadStringW(Instance, IDS_APP_TITLE, str_buf, Max_String_Size);
	Title = str_buf;

	LoadStringW(Instance, IDC_FISHING, str_buf, Max_String_Size);
	Window_Class_Name = str_buf;

	Register_Class();

	// Perform application initialization:
	if (! Init_Instance(command_show))
		return FALSE;

	accel_table = LoadAccelerators(Instance, MAKEINTRESOURCE(IDC_FISHING));


	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, accel_table, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}
//------------------------------------------------------------------------------------------------------------
ATOM AsMain_Window::Register_Class()
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = Window_Proc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = Instance;
	wcex.hIcon = LoadIcon(Instance, MAKEINTRESOURCE(IDC_FISHING));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = AsConfig::Sea_Color.Get_Brush();
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_FISHING);
	wcex.lpszClassName = Window_Class_Name.Get_Content();
	wcex.hIconSm = LoadIcon(Instance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}
//------------------------------------------------------------------------------------------------------------
BOOL AsMain_Window::Init_Instance(int command_show)
{
	HWND hwnd;
	RECT window_rect;

	window_rect.left = 0;
	window_rect.top = 0;
	window_rect.right = 540 * 3;
	window_rect.bottom = 320 * 3;

	AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW, TRUE);

	hwnd = CreateWindowW(Window_Class_Name.Get_Content(), Title.Get_Content(), WS_OVERLAPPEDWINDOW, 0, 0, window_rect.right - window_rect.left, window_rect.bottom - window_rect.top, 0, 0, Instance, 0);

	if (hwnd == 0)
		return FALSE;

	Engine.Init_Engine(hwnd);

	ShowWindow(hwnd, command_show);
	UpdateWindow(hwnd);

	return TRUE;
}
//------------------------------------------------------------------------------------------------------------
void AsMain_Window::On_Paint(HWND hwnd)
{
	HDC hdc, frame_dc;
	PAINTSTRUCT ps;

	hdc = BeginPaint(hwnd, &ps);
	frame_dc = Frame_DC.Get_DC(hwnd, hdc);
	Engine.Draw_Frame(frame_dc, ps.rcPaint);

	BitBlt(hdc, 0, 0, Frame_DC.Width, Frame_DC.Height, frame_dc, 0, 0, SRCCOPY);

	EndPaint(hwnd, &ps);
}
//------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK AsMain_Window::Window_Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(Self->Instance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;


	case WM_PAINT:
		Self->On_Paint(hWnd);
		//return DefWindowProc(hWnd, message, wParam, lParam);
		break;


	case WM_DESTROY:
		PostQuitMessage(0);
		break;


	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			return Self->Engine.On_Key(EKey_Type::Left, true);

		case VK_RIGHT:
			return Self->Engine.On_Key(EKey_Type::Right, true);

		case 0x53:
			return Self->Engine.On_Key(EKey_Type::Mirror, true);

		case VK_SPACE:
			return Self->Engine.On_Key(EKey_Type::Space, true);
		}
		break;

	case WM_KEYUP:
		switch (wParam)
		{
		case VK_LEFT:
			return Self->Engine.On_Key(EKey_Type::Left, false);

		case VK_RIGHT:
			return Self->Engine.On_Key(EKey_Type::Right, false);

		//case VK_SPACE:
		//	return Self->Engine.On_Key(EKey_Type::Space, false);
		}
		break;


	case WM_CHAR:
		//Self->Engine.On_Char(wParam);
		break;

	case WM_TIMER:
		if (wParam == Self->Engine.Timer_ID)
			return Self->Engine.On_Timer();
		break;


	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
//------------------------------------------------------------------------------------------------------------
INT_PTR CALLBACK AsMain_Window::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;


	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}

	return (INT_PTR)FALSE;
}
//------------------------------------------------------------------------------------------------------------



/*
План:

1. Уровень
+1.1. Море
+1.2. Лодка
1.3. Водоросли или коралы
+1.4. Рыбак
+1.5. Удочка
-1.6. Ведро
+1.7. Песок с градиентом
1.8. коралы
+1.9. леска

2. Рыбы
2.1. Мелкие рыбы 4х типов
2.2. Средние рыбы 3х типов
2.3. Большие рыбы 2х типов: кит и тунец
2.4. Хищная рыба акула

3. Удочка
+3.1. риование
+3.1.1. риование крючка
+3.1.2. движение крючка вверх вниз
3.1.3. Пересечение крючка с рыбами и ловля рыб состояние "Cached"

4. Ракушки с эффектами 3-4 штуки
4.1. Жемчужина с буквой или символом серый цвет с "хайлайтом" 
4.1.1. "Золотая леска" I - эту луску не прокусит аккула
4.1.2. "Толстая леска" Т - Поднимает рыб вес которых превышает допустимый 30 скунд
4.1.3. "Двусторонний коючок" К - ловит несколько рыб которы не превышают допустимый вес
4.1.4. "Скорость" С - Увеличивает скорость с которой опускается Крючек и поднимается
4.1.5. "Заморозка" X - на короткое время оставнавливает всех рыб
4.1.5. "Акула" А - Возможность ловить акулу

5. Счетчик перехода по уровням
5.1. Виджет количества мелких рыб 1: 10, 2: 20,3: 30, 4: 40,   5: 40		6: 60		7: 80
5.1. Виджет количества средних рыб 1: 0, 2: 3,	3: 7, 4: 10,	5: 20		6: 30		7: 50	
5.1. Виджет количества больших рыб 1: 0, 2: 3,	3: 0, 4: 3,		5: 10		6: 20		7: 30

6. Анимация Перехода по уровням
6.1. Переход выполнить взрывающимися шариками
6.2. Изменение цвета удочки и панамки игрока
6.3. С каждым уровнем удочка может поднимать рыбы тяжелее

7 Трансформация
+7.1. зеркальное отражение для главных обьектов при нажатии клавиши S	





*/