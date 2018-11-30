#include"MainWindow.h"
#include"GDI.hpp"
#include<sstream>
#include"Sysmets.h"
//using namespace WINAPIPP;

class HelloWin : WINAPIPP::MainWindow {
public:

	WINAPIPP::ScrollBar Horz;
	WINAPIPP::ScrollBar Vert;

	HelloWin() :
		Horz(SB_HORZ, this), Vert(SB_VERT, this) {}

    WPARAM start() {
        InitClass();
		Create(TEXT("MainWindow"), TEXT("The Hello Program"),
			WS_OVERLAPPEDWINDOW | WS_VSCROLL,
			WINAPIPP::Rectangle(CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT),
			BaseWin());

		CheckError();

		return Run();

    }
    LRESULT MessageFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

       // RECT rect;
		TEXTMETRIC tm;
		static int cxChar, cxCaps, cyChar, cxClient, cyClient, iMaxWidth;
		int i, x, y, iVertPos, iHorzPos, iPaintBeg, iPaintEnd;


        switch (message) {

		case WM_CREATE:{
			QuickDC dc(hwnd);

			GetTextMetrics(dc, &tm);
			cxChar = tm.tmAveCharWidth;
			cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2)*cxChar / 2;
			cyChar = tm.tmHeight + tm.tmExternalLeading;
			iMaxWidth = 40 * cxChar + 22 * cxCaps;
			return 0;
			}

		case WM_SIZE: {
			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);
			
			Horz.SetPage(cxClient / cxChar);
			Horz.SetRange(0, (2 + iMaxWidth / cxChar));
			Horz.SetInfo(*this,true);

			Vert.SetPage(cyClient / cyChar);
			Vert.SetRange(0, NUMLINES - 1);
			Vert.SetInfo(*this, true);

			return 0;
		}

		case WM_VSCROLL: {
			int change = Vert.CheckChange(*this, LOWORD(wParam));

			if (change) {
				ScrollWindow(hwnd, 0, cyChar*(change), NULL, NULL);
				UpdateWindow(*this);
			}


		}
						 return 0;

		case WM_HSCROLL: {
			int change = Horz.CheckChange(*this, LOWORD(wParam));

			if (change) {
				ScrollWindow(hwnd, cxChar*(change), 0, NULL, NULL);
				UpdateWindow(*this);
			}
		}

        case WM_PAINT: {
			PaintDC dc(*this);

			Vert.GetInfo(*this);
			
			iVertPos = Vert.Info.nPos;

			//TODO Horz Scrollbar
			Horz.GetInfo(*this);
			iHorzPos = Horz.Info.nPos;

			iPaintBeg = max(0, iVertPos + dc.ps.rcPaint.top / cyChar);
			iPaintEnd = min(NUMLINES - 1, iVertPos + dc.ps.rcPaint.bottom / cyChar);

			for (i = iPaintBeg; i <= iPaintEnd; ++i) {

				x = cxChar * (1 - iHorzPos);
				y = cyChar * (i - iVertPos);

				dc.TextOut({ x,y }, sysmetrics[i].szLabel);

				dc.TextOut({ x + 22 * cxCaps, y },
					sysmetrics[i].szDesc)		;

				SetTextAlign(dc, TA_RIGHT | TA_TOP);
				std::wstringstream str;
				str << sysmetrics[i].iIndex;
				dc.TextOut({ x + 22 * cxCaps + 40 * cxCaps, y },
					str.str());
				SetTextAlign(dc, TA_LEFT | TA_TOP);
			}
			return 0;
        }

		case WM_DESTROY:
			PostQuitMessage(0);
			CheckError();
			return 0;


        }
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

};

int WINAPI WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, PSTR CmdLine, int CmdShow) {
	WINAPIPP::Instance = Instance;
	WINAPIPP::CmdShow = CmdShow;

	HelloWin mainWindow;
	return mainWindow.start();

}
//START_WINDOW(HelloWin);
