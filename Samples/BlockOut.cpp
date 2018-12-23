
#include"Header/WINAPIPP.h"
#include<vector>


class HelloWin : public WINAPIPP::CustomApplication {
	CLASS_PROPERTIES(HelloWin, CS_HREDRAW | CS_VREDRAW, NULL)
		DECLARE_MESSAGE_MAP();
public:

	HelloWin() {
		CreateWin(std::wstring(), WS_OVERLAPPEDWINDOW, Helpers::CPPRectangle(CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT));
	}

	WPARAM start() { return Run(); }
	void DrawBoxOutline() {
		WINAPIPP::QuickDC dc(*this);
		SetROP2(dc, R2_NOT);
		dc.AttachStockObject(NULL_BRUSH);
		Rectangle(dc, ptBeg.x, ptBeg.y, ptEnd.x, ptEnd.y);
	}

	int MouseMove(WPARAM wParam, LPARAM lParam) {
		if (fBlocking) {
			SetCursor(LoadCursor(NULL, IDC_CROSS));

			DrawBoxOutline();

			ptEnd.x = LOWORD(lParam);
			ptEnd.y = HIWORD(lParam);

			DrawBoxOutline();
		}return 0;
	}

	int LButtonDown(WPARAM wParam, LPARAM lParam) {
		ptBeg.x = ptEnd.x = LOWORD(lParam);
		ptBeg.y = ptEnd.y = HIWORD(lParam);

		DrawBoxOutline();

		SetCursor(LoadCursor(NULL, IDC_CROSS));

		fBlocking = true;
		return 0;
	}

	int LButtonUp(WPARAM wParam, LPARAM lParam) {
		if (fBlocking) {
			DrawBoxOutline();

			ptBoxBeg = ptBeg;
			ptBoxEnd.x = LOWORD(lParam);
			ptBoxEnd.y = HIWORD(lParam);

			SetCursor(LoadCursor(NULL, IDC_ARROW));

			fBlocking = false;
			fValidBox = true;

			InvalidateRect(*this, NULL, true);
		}
		return 0;
	}

	int Char(WPARAM wParam, LPARAM lParam) {
		if (fBlocking & wParam == '\x1B') {
			DrawBoxOutline();
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			fBlocking = false;
		}
		return 0;
	}

	int Paint() {
		WINAPIPP::PaintDC dc(*this);

		if (fValidBox) {
			dc.AttachStockObject(BLACK_BRUSH);
			Rectangle(dc, ptBoxBeg.x, ptBoxBeg.y, ptBoxEnd.x, ptBoxEnd.y);
		}
		if (fBlocking) {
			SetROP2(dc, R2_NOT);
			dc.AttachStockObject(NULL_BRUSH);
		}
		return 0;
	}
private:
	int cxBlock, cyBlock;
	bool fBlocking = false, fValidBox = false;
	POINT ptBeg, ptEnd, ptBoxBeg, ptBoxEnd;
};

MESSAGE_MAP_BEGIN(HelloWin)
MESSAGE_MAP_ENTRY_PARAMS(MouseMove, WM_MOUSEMOVE)
MESSAGE_MAP_ENTRY_PARAMS(LButtonDown, WM_LBUTTONDOWN)
MESSAGE_MAP_ENTRY_PARAMS(LButtonUp, WM_LBUTTONUP)
MESSAGE_MAP_ENTRY_PARAMS(Char, WM_CHAR)
MESSAGE_MAP_ENTRY(Paint, WM_PAINT)
MESSAGE_MAP_ENTRY_SINGLE(PostQuitMessage(0), 0, WM_DESTROY)
MESSAGE_MAP_END()

ENTRY_APP(HelloWin);