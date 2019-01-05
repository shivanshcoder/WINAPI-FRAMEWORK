#define AUTO_ENTRY
#include"HIMANI.h"

class Graph :public HIMANI::HCustomApplication {
	CLASS_PROPERTIES(Graph, CS_VREDRAW | CS_HREDRAW, NULL)
		DECLARE_MESSAGE_MAP();

public:
	Graph() {
		CreateWin(L"", WS_OVERLAPPEDWINDOW, Helpers::Rect(CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT));
	}
	WPARAM start() {
		return Run();
	}

	int Paint();

	int MouseDown(WPARAM wParam, LPARAM lParam) {
		MouseHeld = true;
		lastPoint.x = LOWORD(lParam);
		lastPoint.y = HIWORD(lParam);
		SetCursor(LoadCursor(NULL, IDC_HAND));
		return 0;
	}

	int MouseMove(WPARAM wParam, LPARAM lParam) {
		if (MouseHeld) {
			SetCursor(LoadCursor(NULL, IDC_HAND));
			POINT temp;
			temp.x = LOWORD(lParam);
			temp.y = HIWORD(lParam);

			newView.x += (temp.x - lastPoint.x);
			newView.y += (temp.y - lastPoint.y);

			lastPoint = temp;
			::InvalidateRect(*this, NULL, TRUE);
		}
		return 0;
	}
	int MouseUp(WPARAM wParam, LPARAM lParam) {
		MouseHeld = false;
		return 0;
	}
private:
	POINT lastPoint = {};
	POINT newView = {};
	bool MouseHeld = false;
};

MESSAGE_MAP_BEGIN(Graph)
MESSAGE_MAP_ENTRY_PARAMS(MouseDown, WM_LBUTTONDOWN)
MESSAGE_MAP_ENTRY_PARAMS(MouseMove, WM_MOUSEMOVE)
MESSAGE_MAP_ENTRY_PARAMS(MouseUp, WM_LBUTTONUP)
MESSAGE_MAP_ENTRY(Paint, WM_PAINT)
MESSAGE_MAP_ENTRY_SINGLE(PostQuitMessage(0), 0, WM_DESTROY)
MESSAGE_MAP_END(HIMANI::HCustomApplication)


int Graph::Paint() {
	HIMANI::PaintDC dc(*this);
	Helpers::Rect rect = GetClientRect();
	int xSteps = rect.xLength() / 10;
	int ySteps = rect.yLength() / 10;
	SetViewportOrgEx(dc, newView.x, newView.y, NULL);

	for (int start = -rect.xLength(); start < rect.xLength(); start += xSteps) {
		MoveToEx(dc, start, -rect.yLength(), NULL);
		LineTo(dc, start, rect.yLength());
	}

	for (int start = -rect.yLength(); start < rect.yLength(); start += ySteps) {
		MoveToEx(dc, -rect.yLength(), start, NULL);
		LineTo(dc, rect.xLength(), start);
	}
	return 0;
}

ENTRY_APP(Graph)

