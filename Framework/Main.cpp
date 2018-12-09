#include"Header/WINAPIPP.h"
#include<sstream>
#include"Header/Sysmets.h"
//using namespace WINAPIPP;


#define NUM 1000
#define TWOPI (2*3.14159)



class HelloWin : public WINAPIPP::Application {
	int cxClient, cyClient;
public:
	

    WPARAM start() {
        InitClass();
		CreateWin(TEXT("MainWindow"), TEXT("The Hello Program"),
			WS_OVERLAPPEDWINDOW ,
			WINAPIPP::Rectangle(CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT),
			BaseWin());

		return Run();

    }
	void Idle()override {
		if (cxClient == 0 || cyClient == 0) {
			return;
		}

		WINAPIPP::Brush brush(RGB(rand() % 256, rand() % 256, rand() % 256));

		WINAPIPP::QuickDC dc(*this);

		WINAPIPP::Rectangle rect = WINAPIPP::Rectangle::Random(cxClient,cyClient);
		FillRect(dc, &(rect.rect), brush);

		//DeleteObject(brush);


	}

    LRESULT MessageFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

		TEXTMETRIC tm;
		//static int cxChar, cxCaps, cyChar, cxClient, cyClient, iMaxWidth;
	


        switch (message) {

		//case WM_CREATE:{
		//	WINAPIPP::QuickDC dc(hwnd);
		//
		//	GetTextMetrics(dc, &tm);
		//	cxChar = tm.tmAveCharWidth;
		//	cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2)*cxChar / 2;
		//	cyChar = tm.tmHeight + tm.tmExternalLeading;
		//	iMaxWidth = 40 * cxChar + 22 * cxCaps;
		//	return 0;
		//	}

		case WM_SIZE: {
			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);
			
			//Horz.SetPage(cxClient / cxChar);
			//Horz.SetRange(0, (2 + iMaxWidth / cxChar));
			//Horz.SetInfo(true);
			//
			//Vert.SetPage(cyClient / cyChar);
			//Vert.SetRange(0, NUMLINES - 1);
			//Vert.SetInfo(true);

			return 0;
		}

		/*case WM_VSCROLL: {
			int change = Vert.CheckChange(LOWORD(wParam));
		
			if (change) {
				ScrollWindow(hwnd, 0, cyChar*(change), NULL, NULL);
				UpdateWindow(*this);
			}
		
		}return 0;*/
		//
		//case WM_HSCROLL: {
		//	int change = Horz.CheckChange( LOWORD(wParam));
		//
		//	if (change) {
		//		ScrollWindow(hwnd, cxChar*(change), 0, NULL, NULL);
		//		UpdateWindow(*this);
		//	}
		//}return 0;

       /* case WM_PAINT: {
			PaintDC dc(*this);

			int iVertPos = Vert.GetCurrentPos();

			int iHorzPos = Horz.GetCurrentPos();

			int iPaintBeg = max(0, iVertPos + dc.ps.rcPaint.top / cyChar);
			int iPaintEnd = min(NUMLINES - 1, iVertPos + dc.ps.rcPaint.bottom / cyChar);

			for (int i = iPaintBeg; i <= iPaintEnd; ++i) {

				int x = cxChar * (1 - iHorzPos);
				int y = cyChar * (i - iVertPos);

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
        }*/

		//case WM_PAINT: {
		//	POINT apt[NUM];
		//	WINAPIPP::PaintDC dc(*this);
		//	MoveToEx(dc, 0, cyClient / 2, NULL);
		//	LineTo(dc, cxClient, cyClient / 2);
		//	{
		//		WINAPIPP::Pen p(PS_DASH, 1, RGB(255, 255, 0));
		//		//WINAPIPP::Pen p2(p);
		//		//dc.Attach(p);
		//		dc.Attach(WINAPIPP::Pen(PS_DASH, 1, RGB(0, 0, 255)));
		//		dc.Attach(WINAPIPP::Pen(PS_DASH, 1, RGB(0, 255, 255)));
		//		dc.Attach(WINAPIPP::Pen(PS_DASH, 1, RGB(0, 255, 0)));

		//	}
		//	for (int i = 0; i < NUM; ++i) {
		//		apt[i].x = i * cxClient / NUM;
		//		apt[i].y = (int)(cyClient / 2 * (1 - sin(TWOPI * i / NUM)));
		//	}

		//	Polyline(dc, apt, NUM);
		//	
		//}return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
			CheckError();
			return 0;
        }
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

};

ENTRY_APP(HelloWin)