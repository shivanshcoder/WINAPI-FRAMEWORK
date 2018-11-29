#include"MainWindow.h"
#include"GDI.hpp"
#include<sstream>
//using namespace WINAPIPP;

class HelloWin : WINAPIPP::MainWindow {
public:

    WPARAM start() {
        InitClass();
		Create(TEXT("MainWindow"), TEXT("The Hello Program"),
			WS_OVERLAPPEDWINDOW,
			WINAPIPP::Rectangle(CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT),
			BaseWin());

		CheckError();

		return Run();

    }
    LRESULT MessageFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
        HDC hdc;
        PAINTSTRUCT ps;
        RECT rect;

		static int cxChar, cxCaps, cyChar, cxClient, cyClient, iMaxWidth;
		int i, x, y, iVertPos, iHorzPos, iPaintBeg, iPaintEnd;

        switch (message) {

		case WM_SIZE: {
			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);
			
			WINAPIPP::ScrollBar Horz(SB_HORZ);
			Horz.SetPage(cxClient / cxChar);
			Horz.SetRange(0, (2 + iMaxWidth / cxChar));
			Horz.SetInfo(*this,true);

			WINAPIPP::ScrollBar Vert(SB_VERT);
			Horz.SetPage(cyClient / cyChar);
			Horz.SetRange(0, NUMLINES - 1);


		}

        case WM_PAINT: {
			PaintDC dc(*this);
            GetClientRect(hwnd, &rect);

			std::wstringstream stream;
			stream << "Hello Wolrd " << 213 << " Bye";
			dc.TextOut({ 50,50 }, stream.str());
			//DrawText(dc, stream.str().c_str(), -1, &rect,
			//	DT_SINGLELINE | DT_CENTER | DT_VCENTER);

			CheckError();
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
