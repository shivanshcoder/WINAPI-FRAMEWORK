#include"Header/WINAPIPP.h"
#include"Header/CustomWinClass.h"
#define TWOPI (2*3.14159)

template< int Message, typename T,typename T2>
std::pair<T, T2> Unpack(int Message, WPARAM wParam,LPARAM lParam) {
	//std::pair<T, T2>
}

class HelloWin : public WINAPIPP::Application {
	int cxClient, cyClient;
	WINAPIPP::Region RgnClip;
public:


	WPARAM start() {
		InitClass();
		CreateWin( TEXT("The Hello Program"),
			WS_OVERLAPPEDWINDOW,
			WINAPIPP::Rectangle(CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT),
			BaseWin());

		return Run();
	}
	DECLARE_MESSAGE_MAP();

	int OnSize(WPARAM wParam, LPARAM lParam) {
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
	int OnPaint() {
		WINAPIPP::PaintDC dc(*this);

		SetViewportOrgEx(dc, cxClient / 2, cyClient / 2, NULL);
		dc.SelectRegion(RgnClip);
		float fRadius = _hypot(cxClient / 2.0, cyClient / 2.0);

		for (float fAngle = 0.0; fAngle < TWOPI; fAngle += TWOPI / 360) {
			MoveToEx(dc, 0, 0, NULL);
			LineTo(dc, (int)(fRadius *cos(fAngle) + 0.5),
				(int)(-fRadius * sin(fAngle) + 0.5));
		}
		return 0;
	}

	int OnDestroy() {
		PostQuitMessage(0);
		return 0;
	}
	

};


MESSAGE_MAP_BEGIN(HelloWin)
MESSAGE_MAP_ENTRY_PARAMS(OnSize, WM_SIZE)
MESSAGE_MAP_ENTRY(OnPaint, WM_PAINT)
MESSAGE_MAP_ENTRY(OnDestroy, WM_DESTROY)
MESSAGE_MAP_END()

ENTRY_APP(HelloWin)