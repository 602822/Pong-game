// WindowsProject111.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "WindowsProject111.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
POINT position{ 0,0 };
POINT direction{ 1,1 };
RECT rect;
int positionX = 0;
int score = 0;
int fontSize = 40;


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WINDOWSPROJECT111, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT111));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT111));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT111);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;


	case WM_KEYDOWN:
	{
		switch (wParam) {
			//F�r Rectangelet til � bevege seg til Venstre
		case VK_LEFT:
			if (positionX + 540 != rect.left) {
				positionX -= 10;
			}
			break;
			//F�r Rectangelet til � bevege seg til H�gre
		case VK_RIGHT:
			if (positionX + 700 != rect.right) {
				positionX += 10;
			}
			break;

		}
	}
	break;



	case WM_TIMER:
	{
		//Oppdaterer posisjonen til ballen
		position.x += 10 * direction.x;
		position.y += 10 * direction.y;

		//F�r ballen til � sprette n�r du treffer venstre eller h�gre side av vinduet
		if (position.x <= rect.left || position.x >= rect.right) direction.x *= -1;
		if (position.y <= rect.top) {
			direction.y *= -1;
		}


		// F�r ballen til � sprette n�r du treffer bunnen av vindet og resetter scoren
		if (position.y >= rect.bottom) {
			direction.y *= -1;
			score = 0;
		}


		// F�r ballen til � sprette n�r du treffer Rectangellet og �ker scoren
		if (position.x == positionX && position.y == 570) direction.y *= -1;
		if (position.x >= 540 + positionX && position.x <= 700 + positionX && position.y >= 560) {
			direction.y *= -1;
			score++;

		}



		InvalidateRect(hWnd, NULL, TRUE);


	}
	break;

	case WM_CREATE:
	{
		SetTimer(hWnd, 0, 100, NULL);
	}
	break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		//For � tegne ballen
		HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
		HGDIOBJ horg = SelectObject(hdc, redBrush);
		Ellipse(hdc, position.x - 10, position.y - 10, position.x + 10, position.y + 10);


		//For � tegne Rectangelet
		HBRUSH blueBrush = CreateSolidBrush(RGB(0, 0, 255));
		SelectObject(hdc, blueBrush);
		Rectangle(hdc, 540 + positionX, 570, 700 + positionX, 600);




		//For � vise scoren p� skjermen
		HFONT font = CreateFont(fontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
		SelectObject(hdc, font);
		wchar_t buffer[10];
		wsprintf(buffer, L"%d", score);
		TextOut(hdc, 100, 100, buffer, wcslen(buffer));





		GetClientRect(hWnd, &rect);


		//Cleanup
		SelectObject(hdc, horg);
		DeleteObject(redBrush);
		DeleteObject(blueBrush);
		DeleteObject(font);






		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
