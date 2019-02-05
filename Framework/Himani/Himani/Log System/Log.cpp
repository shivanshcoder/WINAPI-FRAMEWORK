#include "Hpch.h"
#include "Log.h"
#include<fstream>

namespace LogSystem {

	void FileLog::Refresh()
	{
		if (!Entries.size())return;

		std::wofstream File;
		File.open(FileName, std::wofstream::app);

		for (int i = 0; i < Entries.size(); ++i) {
			File << Entries[i].Level << L": " << Entries[i].Entry << L"\n";
		}
		//Entries.clear();
	}


	WindowLog::WindowLog() {
		InitWindow();
	}
	void WindowLog::Refresh()
	{
		HDC	hdc = GetDC(LogWindowHandle);
		SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));

		ScrollWindow(LogWindowHandle, 0, -(cyChar * Entries.size()), &rect, &rect);
		
		for (int i = Entries.size() - 1; i >= 0 ; --i) {
			UpdateWindow(LogWindowHandle);
			std::wstringstream Stream;
			Stream << Entries[i].Level << L": " << Entries[i].Entry << L"\n";

			std::wstring s = Stream.str();
			TextOut(hdc, 24 * cxChar, cyChar * ((rect.bottom / cyChar - 1) - i), s.c_str(), s.size());
		}
		ReleaseDC(LogWindowHandle, hdc);
		ValidateRect(LogWindowHandle, &rect);
	}
	void WindowLog::DefaultColors() {
		TextColor[LogLevels::Normal] = RGB(0, 150, 150);
		TextColor[LogLevels::Moderate] = RGB(150, 150, 0);
		TextColor[LogLevels::High] = RGB(150, 0, 0);
	}
	void WindowLog::InitWindow() {
		WNDCLASS wndclass = {};
		wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wndclass.hCursor = LoadCursor(NULL, IDI_APPLICATION);
		//TODO maybe remove this later on?
		wndclass.hInstance = GetModuleHandle(NULL);
		wndclass.lpfnWndProc = &WindowLog::WndProc;
		wndclass.lpszClassName = TEXT("LOGWINCLASS");
		wndclass.style = CS_HREDRAW | CS_VREDRAW;
		//wndclass.cbWndExtra = 4;

		RegisterClass(&wndclass);

		LogWindowHandle = CreateWindow(TEXT("LOGWINCLASS"), TEXT("Log Window"), WS_OVERLAPPEDWINDOW | WS_VSCROLL, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, GetModuleHandle(NULL), NULL);
		//SetWindowLongPtr(LogWindowHandle, GWLP_USERDATA, (LONG)this);
		UpdateWindow(LogWindowHandle);
		ShowWindow(LogWindowHandle, SW_SHOW);

	}
	LRESULT WindowLog::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		HDC          hdc;
		PAINTSTRUCT  ps;
		int          i;
		SCROLLINFO si;

		static int xClient;     // width of client area 
		static int yClient;     // height of client area 
		static int xClientMax;  // maximum width of client area 

		static int xChar;       // horizontal scrolling unit 
		static int yChar;       // vertical scrolling unit 
		static int xUpper;      // average width of uppercase letters 

		static int xPos;        // current horizontal scrolling position 
		static int yPos;        // current vertical scrolling position 


		int FirstLine;          // first line in the invalidated area 
		int LastLine;           // last line in the invalidated area 

		switch (message)
		{
		case WM_CREATE:
			cxChar = LOWORD(GetDialogBaseUnits());
			cyChar = HIWORD(GetDialogBaseUnits());

			return 0;

		case WM_SIZE:
			rect.left = 0; // 24 * cxChar;
			rect.top = 0;//2 * cyChar;
			rect.right = LOWORD(lParam);
			rect.bottom = HIWORD(lParam);
			return 0;


		case WM_VSCROLL: {

		}return 0;

		case WM_HSCROLL: {
			// Get all the vertial scroll bar information.
			si.cbSize = sizeof(si);
			si.fMask = SIF_ALL;

			// Save the position for comparison later on.
			GetScrollInfo(hwnd, SB_HORZ, &si);
			xPos = si.nPos;
			switch (LOWORD(wParam))
			{
				// User clicked the left arrow.
			case SB_LINELEFT:
				si.nPos -= 1;
				break;

				// User clicked the right arrow.
			case SB_LINERIGHT:
				si.nPos += 1;
				break;

				// User clicked the scroll bar shaft left of the scroll box.
			case SB_PAGELEFT:
				si.nPos -= si.nPage;
				break;

				// User clicked the scroll bar shaft right of the scroll box.
			case SB_PAGERIGHT:
				si.nPos += si.nPage;
				break;

				// User dragged the scroll box.
			case SB_THUMBTRACK:
				si.nPos = si.nTrackPos;
				break;

			default:
				break;
			}
		}return 0;

		case WM_PAINT: {

		}return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	int WindowLog::cxChar = 0;
	int WindowLog::cyChar = 0;
	RECT WindowLog::rect = {};

}
