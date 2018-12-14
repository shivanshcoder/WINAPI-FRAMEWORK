#include"Header/WINAPIPP.h"

#define TWOPI (2*3.14159)

class HelloWin : public WINAPIPP::Application {
	int cxClient, cyClient;
	WINAPIPP::Region RgnClip;
public:


	WPARAM start() {
		InitClass();
		CreateWin(TEXT("MainWindow"), TEXT("The Hello Program"),
			WS_OVERLAPPEDWINDOW,
			WINAPIPP::Rectangle(CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT),
			BaseWin());

		return Run();
  }


	LRESULT MessageFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

		switch (message) {

		case WM_SIZE: {
			using namespace WINAPIPP;
			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);

			HCURSOR Cursor = SetCursor(LoadCursor(NULL, IDC_WAIT));
			ShowCursor(TRUE);

			RgnClip.clear();

			Region Reg1(
				Region(true, WINAPIPP::Rectangle(0, cyClient / 3, cxClient / 2, 2 * cyClient / 3)),
				Region(true, WINAPIPP::Rectangle(cxClient / 2, cyClient / 3, cxClient, 2 * cyClient / 3)),
				RGN_OR
			);;
			
			Region Reg2(
				Region(true, WINAPIPP::Rectangle(cxClient / 3, 0, 2 * cxClient / 3, cyClient / 2)),
				Region(true, WINAPIPP::Rectangle(cxClient / 3, cyClient / 2, 2 * cxClient / 3, cyClient)),
				RGN_OR
			);

			RgnClip.Combine(Reg1, Reg2, RGN_XOR);

			SetCursor(Cursor);
			ShowCursor(FALSE);

			return 0;
		}

		case WM_PAINT: {
			WINAPIPP::PaintDC dc(*this);

			SetViewportOrgEx(dc, cxClient / 2, cyClient / 2, NULL);
			dc.SelectRegion(RgnClip);
			float fRadius = _hypot(cxClient / 2.0, cyClient / 2.0);

			for (float fAngle = 0.0; fAngle < TWOPI; fAngle += TWOPI / 360) {
				MoveToEx(dc, 0, 0, NULL);
				LineTo(dc, (int)(fRadius *cos(fAngle) + 0.5),
					(int)(-fRadius * sin(fAngle) + 0.5));
			}
		}return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
			CheckError();
			return 0;
		}
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

};

ENTRY_APP(HelloWin)