//#define INCLUDE_CPPS
//#include"Himani.h"
//
//#include"resource.h"
//
//#define CF_TCHAR CF_UNICODETEXT
//const TCHAR DefaultText[] = TEXT("Defalt Text - UNICODE Version");
//const TCHAR Caption[] = TEXT("Clipboard Text Transfers - Unicode Version");
//
//
//class MyMenu :public Himani::HMenu {
//public:
//	MyMenu() :HMenu(IDR_MENU1) {}
//
//	int callback(int ID)override {
//		
//	}
//};
//
//class ClipText :public Himani::HCustomApplication {
//public:
//	//IconDemo():HCustomApplication(L"IconDemo",WS_OVERLAPPEDWINDOW) {}
//
//	ClipText() 
//	/*:Menu(IDR_MENU1)*/{
//		CreateWin(L"Temp", WS_OVERLAPPEDWINDOW, Helpers::HRect(CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT));
//
//
//	}
//
//	CLASS_PROPERTIES(ClipText,CS_VREDRAW|CS_HREDRAW,NULL)
//
//	int OnSize(WPARAM wParam, LPARAM lParam) {
//		cxClient = LOWORD(lParam);
//		cyClient = HIWORD(lParam);
//		cxIcon = GetSystemMetrics(SM_CXICON);
//		cyIcon = GetSystemMetrics(SM_CYICON);
//		
//		return 0;
//	}
//
//	int OnPaint() {
//		Himani::PaintDC dc(*this);
//
//		HICON Icon = LoadIcon(Himani::Instance(), MAKEINTRESOURCEW(IDI_ICON));
//		for (int x = 0; x < cxClient; x+=cxIcon) {
//			for (int y = 0; y < cyClient; y+=cyIcon) {
//				DrawIcon(dc, x, y, Icon);
//			}
//		}
//		return 0;
//	}
//
//	int OnCommand(WPARAM wParam, LPARAM lParam) {
//		//Menu was used
//		//if (lParam == NULL)
//		//	Menu.callback(LOWORD(wParam));
//		//else child sent a message
//		return 0;
//	}
//	DECLARE_MESSAGE_MAP();
//private:
//	int cxClient, cyClient;
//	int cxIcon, cyIcon;
//	//Himani::HMenu Menu;
//};
//
//MESSAGE_MAP_BEGIN(ClipText)
//MESSAGE_MAP_ENTRY_PARAMS(OnSize, WM_SIZE)
//MESSAGE_MAP_ENTRY(OnPaint, WM_PAINT)
//MESSAGE_MAP_ENTRY_PARAMS(OnCommand, WM_COMMAND)
//MESSAGE_MAP_ENTRY_SINGLE(PostQuitMessage(0), 0, WM_DESTROY)
//MESSAGE_MAP_END(Himani::HCustomApplication)
//
//ENTRY_APP(ClipText)

/*----------------------------------------
   BTNLOOK.C -- Button Look Program
				(c) Charles Petzold, 1998
  ----------------------------------------*/

#include <windows.h>

struct
{
	int     iStyle;
	const TCHAR* szText;
}
button[] =
{
	 BS_PUSHBUTTON,      TEXT("PUSHBUTTON"),
	 BS_DEFPUSHBUTTON,   TEXT("DEFPUSHBUTTON"),
	 BS_CHECKBOX,        TEXT("CHECKBOX"),
	 BS_AUTOCHECKBOX,    TEXT("AUTOCHECKBOX"),
	 BS_RADIOBUTTON,     TEXT("RADIOBUTTON"),
	 BS_3STATE,          TEXT("3STATE"),
	 BS_AUTO3STATE,      TEXT("AUTO3STATE"),
	 BS_GROUPBOX,        TEXT("GROUPBOX"),
	 BS_AUTORADIOBUTTON, TEXT("AUTORADIO"),
	 BS_OWNERDRAW,       TEXT("OWNERDRAW")
};

#define NUM (sizeof button / sizeof button[0])

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("BtnLook");
	HWND         hwnd;
	MSG          msg;
	WNDCLASS     wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This program requires Windows NT!"),
			szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName, TEXT("Button Look"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND  hwndButton[NUM];
	static RECT  rect;
	static TCHAR szTop[] = TEXT("message            wParam       lParam"),
		szUnd[] = TEXT("_______            ______       ______"),
		szFormat[] = TEXT("%-16s%04X-%04X    %04X-%04X"),
		szBuffer[50];
	static int   cxChar, cyChar;
	HDC          hdc;
	PAINTSTRUCT  ps;
	int          i;

	switch (message)
	{
	case WM_CREATE:
		cxChar = LOWORD(GetDialogBaseUnits());
		cyChar = HIWORD(GetDialogBaseUnits());

		for (i = 0; i < NUM; i++)
			hwndButton[i] = CreateWindow(TEXT("button"),
				button[i].szText,
				WS_CHILD | WS_VISIBLE | button[i].iStyle,
				cxChar, cyChar * (1 + 2 * i),
				20 * cxChar, 7 * cyChar / 4,
				hwnd, (HMENU)i,
				((LPCREATESTRUCT)lParam)->hInstance, NULL);
		return 0;

	case WM_SIZE:
		rect.left = 24 * cxChar;
		rect.top = 2 * cyChar;
		rect.right = LOWORD(lParam);
		rect.bottom = HIWORD(lParam);
		return 0;

	/*case WM_PAINT:
		InvalidateRect(hwnd, &rect, TRUE);

		hdc = BeginPaint(hwnd, &ps);
		SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
		SetBkMode(hdc, TRANSPARENT);

		TextOut(hdc, 24 * cxChar, cyChar, szTop, lstrlen(szTop));
		TextOut(hdc, 24 * cxChar, cyChar, szUnd, lstrlen(szUnd));

		EndPaint(hwnd, &ps);
		return 0;
*/
	case WM_DRAWITEM:
	case WM_COMMAND:
		ScrollWindow(hwnd, 0, -cyChar, &rect, &rect);

		hdc = GetDC(hwnd);
		SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));

		TextOut(hdc, 24 * cxChar, cyChar * (rect.bottom / cyChar - 1),
			szBuffer,
			wsprintf(szBuffer, szFormat,
				message == WM_DRAWITEM ? TEXT("WM_DRAWITEM") :
				TEXT("WM_COMMAND"),
				HIWORD(wParam), LOWORD(wParam),
				HIWORD(lParam), LOWORD(lParam)));

		ReleaseDC(hwnd, hdc);
		ValidateRect(hwnd, &rect);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

