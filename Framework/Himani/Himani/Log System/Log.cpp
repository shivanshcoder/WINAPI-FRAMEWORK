#include "Hpch.h"
#include "Log.h"
#include<fstream>
namespace LogSystem {

	void FileLog::Refresh()
	{
		if (!Entries.size())return;

		std::wofstream File;
		File.open(FileName, std::wofstream::app);
		

		for (int i = lastIndex; i < Entries.size(); ++i) {
			File << Entries[i].Level << L": " << Entries[i].Entry << L"\n";
		}
		lastIndex = Entries.size();
	}


	void WindowLog::Refresh()
	{
		InvalidateRect(LogWindowHandle, NULL, TRUE);
		UpdateWindow(LogWindowHandle);
	
	}

	void WindowLog::PushWinErrors(int ErrorCode)	{
		wchar_t* buf;
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, ErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPWSTR)& buf, sizeof(buf), NULL);
		Log::Push(1, std::wstring(buf));

		//URGENT should it be deleted??
		delete buf;

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
		wndclass.cbWndExtra = 4;

		RegisterClass(&wndclass);

		LogWindowHandle = CreateWindow(TEXT("LOGWINCLASS"), TEXT("Log Window"), WS_OVERLAPPEDWINDOW | WS_VSCROLL |WS_HSCROLL, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, GetModuleHandle(NULL), NULL);

		SetWindowLongPtr(LogWindowHandle, GWLP_USERDATA, (LONG)&Entries);

		UpdateWindow(LogWindowHandle);
		ShowWindow(LogWindowHandle, SW_SHOW);


	}

	
	LRESULT WindowLog::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		HDC          hdc;
		PAINTSTRUCT  ps;
		SCROLLINFO si;
		TEXTMETRIC tm;

		// These variables are required to display text. 
		static int xClient;     // width of client area 
		static int yClient;     // height of client area 
		static int xClientMax;  // maximum width of client area 

		static int xChar;       // horizontal scrolling unit 
		static int yChar;       // vertical scrolling unit 
		static int xUpper;      // average width of uppercase letters 

		static int xPos;        // current horizontal scrolling position 
		static int yPos;        // current vertical scrolling position 

		int i;                  // loop counter 
		int x, y;               // horizontal and vertical coordinates

		int FirstLine;          // first line in the invalidated area 
		int LastLine;           // last line in the invalidated area 
		HRESULT hr;
		size_t abcLength;        // length of an abc[] item 

		switch (message)
		{
		case WM_CREATE:
			// Get the handle to the client area's device context. 
			hdc = GetDC(hwnd);

			// Extract font dimensions from the text metrics. 
			GetTextMetrics(hdc, &tm);
			xChar = tm.tmAveCharWidth;
			xUpper = (tm.tmPitchAndFamily & 1 ? 3 : 2) * xChar / 2;
			yChar = tm.tmHeight + tm.tmExternalLeading;

			// Free the device context. 
			ReleaseDC(hwnd, hdc);

			// Set an arbitrary maximum width for client area. 
			// (xClientMax is the sum of the widths of 48 average 
			// lowercase letters and 12 uppercase letters.) 
			xClientMax = 48 * xChar + 12 * xUpper;



			return 0;

		case WM_SIZE: {

			//Get number of entries
			auto Entries = (std::vector< LogEntries>*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			int LINES = Entries->size();

			// Retrieve the dimensions of the client area. 
			yClient = HIWORD(lParam);
			xClient = LOWORD(lParam);

			// Set the vertical scrolling range and page size
			si.cbSize = sizeof(si);
			si.fMask = SIF_RANGE | SIF_PAGE;
			si.nMin = 0;
			si.nMax = LINES - 1;
			si.nPage = yClient / yChar;
			SetScrollInfo(hwnd, SB_VERT, &si, TRUE);

			// Set the horizontal scrolling range and page size. 
			si.cbSize = sizeof(si);
			si.fMask = SIF_RANGE | SIF_PAGE;
			si.nMin = 0;
			si.nMax = 2 + xClientMax / xChar;
			si.nPage = xClient / xChar;
			SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
		}
					  return 0;
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

			// Set the position and then retrieve it.  Due to adjustments
			// by Windows it may not be the same as the value set.
			si.fMask = SIF_POS;
			SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
			GetScrollInfo(hwnd, SB_HORZ, &si);

			// If the position has changed, scroll the window.
			if (si.nPos != xPos)
			{
				ScrollWindow(hwnd, xChar * (xPos - si.nPos), 0, NULL, NULL);
			}
		}
			return 0;

		case WM_VSCROLL: {
			// Get all the vertial scroll bar information.
			si.cbSize = sizeof(si);
			si.fMask = SIF_ALL;
			GetScrollInfo(hwnd, SB_VERT, &si);

			// Save the position for comparison later on.
			yPos = si.nPos;
			switch (LOWORD(wParam))
			{

				// User clicked the HOME keyboard key.
			case SB_TOP:
				si.nPos = si.nMin;
				break;

				// User clicked the END keyboard key.
			case SB_BOTTOM:
				si.nPos = si.nMax;
				break;

				// User clicked the top arrow.
			case SB_LINEUP:
				si.nPos -= 1;
				break;

				// User clicked the bottom arrow.
			case SB_LINEDOWN:
				si.nPos += 1;
				break;

				// User clicked the scroll bar shaft above the scroll box.
			case SB_PAGEUP:
				si.nPos -= si.nPage;
				break;

				// User clicked the scroll bar shaft below the scroll box.
			case SB_PAGEDOWN:
				si.nPos += si.nPage;
				break;

				// User dragged the scroll box.
			case SB_THUMBTRACK:
				si.nPos = si.nTrackPos;
				break;

			default:
				break;
			}

			// Set the position and then retrieve it.  Due to adjustments
			// by Windows it may not be the same as the value set.
			si.fMask = SIF_POS;
			SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
			GetScrollInfo(hwnd, SB_VERT, &si);

			// If the position has changed, scroll window and update it.
			if (si.nPos != yPos)
			{
				ScrollWindow(hwnd, 0, yChar * (yPos - si.nPos), NULL, NULL);
				UpdateWindow(hwnd);
			}
		}
			return 0;

		case WM_PAINT: {
			// Prepare the window for painting.
			hdc = BeginPaint(hwnd, &ps);

			// Get vertical scroll bar position.
			si.cbSize = sizeof(si);
			si.fMask = SIF_POS;
			GetScrollInfo(hwnd, SB_VERT, &si);
			yPos = si.nPos;

			// Get horizontal scroll bar position.
			GetScrollInfo(hwnd, SB_HORZ, &si);
			xPos = si.nPos;


			auto Entries = (std::vector< LogEntries>*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			int LINES = Entries->size();


			// Find painting limits.
			FirstLine = max(0, yPos + ps.rcPaint.top / yChar);
			LastLine = min(LINES - 1, yPos + ps.rcPaint.bottom / yChar);

			for (i = FirstLine; i <= LastLine; i++)
			{
				x = xChar * (1 - xPos);
				y = yChar * (i - yPos);

				//Prints out the Entries of the log!
				TextOut(hdc, x, y, (*Entries)[i].Entry.c_str(), (*Entries)[i].Entry.size());
			}

			EndPaint(hwnd, &ps);
		}
					   return 0;

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
