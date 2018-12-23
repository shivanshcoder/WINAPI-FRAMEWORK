
#include"Header/WINAPIPP.h"
#include<vector>
#define DIV 5

int idFocus = 0;
class ChildWindows :public WINAPIPP::Window {
	bool flag = false;

public:
	ChildWindows(BaseWin Parent, int index) {
		CreateWin(std::wstring(), WS_CHILDWINDOW | WS_VISIBLE, Helpers::Rect(0, 0, 0, 0), &Parent, (HMENU)index);
	}

	CLASS_ALL_PROPERTIES(ChildWindows, CS_HREDRAW | CS_VREDRAW, NULL, LoadCursor(NULL, IDC_ARROW), (HBRUSH)GetStockObject(WHITE_BRUSH), NULL)
		DECLARE_MESSAGE_MAP();

	int KeyDown(WPARAM wParam, LPARAM lParam) {
		if (wParam != VK_RETURN && wParam != VK_SPACE) {
			SendMessage(Parent(), WM_KEYDOWN, wParam, lParam);
			return 0;
		}
		return LButtonDown();
	}

	int SetFocus() {
		idFocus = GetWindowLongPtr(*this, GWLP_ID);
		return KillFocus();
	}

	int KillFocus() {
		::InvalidateRect(*this, NULL, TRUE);
		return 0;
	}

	int LButtonDown() {
		flag = (1 ^ flag);
		::SetFocus(*this);
		::InvalidateRect(*this, NULL, FALSE);
		return 0;
	}

	int Paint() {
		WINAPIPP::PaintDC dc(*this);
		WINAPIPP::Pen pen(PS_DASH, 0, 0);
		RECT rect;

		::GetClientRect(*this, &rect);
		Rectangle(dc, 0, 0, rect.right, rect.bottom);

		if (flag) {
			MoveToEx(dc, 0, 0, NULL);
			LineTo(dc, rect.right, rect.bottom);
			MoveToEx(dc, 0, rect.bottom, NULL);
			LineTo(dc, rect.right, 0);
		}

		//Draw Focus Rectangle
		if ((HWND)*this == GetFocus()) {
			rect.left += rect.right / 10;
			rect.right -= rect.left;
			rect.top += rect.bottom / 10;
			rect.bottom -= rect.top;

			dc.Attach(pen);
			dc.AttachStockObject(NULL_BRUSH);
			Rectangle(dc, rect.left, rect.top, rect.right, rect.bottom);
		}
		return 0;
	}

};

MESSAGE_MAP_BEGIN(ChildWindows)
MESSAGE_MAP_ENTRY_PARAMS(KeyDown, WM_KEYDOWN)
MESSAGE_MAP_ENTRY(SetFocus, WM_SETFOCUS)
MESSAGE_MAP_ENTRY(KillFocus, WM_KILLFOCUS)
MESSAGE_MAP_ENTRY(LButtonDown, WM_LBUTTONDOWN)
MESSAGE_MAP_ENTRY(Paint, WM_PAINT)
MESSAGE_MAP_END();

class HelloWin : public WINAPIPP::CustomApplication {
	std::vector<std::vector<std::unique_ptr<ChildWindows> > > Children;
	int cxBlock, cyBlock;

public:
	CLASS_ALL_PROPERTIES(HelloWin, CS_HREDRAW | CS_VREDRAW, LoadIcon(NULL, IDI_APPLICATION), LoadCursor(NULL, IDC_ARROW), (HBRUSH)GetStockObject(WHITE_BRUSH), NULL)

		DECLARE_MESSAGE_MAP();

	HelloWin() {
		CreateWin(std::wstring(), WS_OVERLAPPEDWINDOW, Helpers::Rect(CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT));
		Children.resize(5);
		for (int x = 0; x < DIV; ++x)
			for (int y = 0; y < DIV; ++y)
				Children[x].push_back(std::make_unique<ChildWindows>(*this, (y << 8 | x)));
	}

	WPARAM start() { return Run(); }

	int OnSize(WPARAM wParam, LPARAM lParam) {

		cxBlock = LOWORD(lParam) / DIV;
		cyBlock = HIWORD(lParam) / DIV;

		for (int x = 0; x < DIV; ++x) {
			for (int y = 0; y < DIV; ++y) {
				MoveWindow(*Children[x][y], cxBlock*x, cyBlock*y, cxBlock, cyBlock, TRUE);
			}
		}
		return 0;
	}
	int OnMouseDown(WPARAM wParam, LPARAM lParam) {
		MessageBeep(0);
		return 0;
	}
	int KeyDown(WPARAM wParam, LPARAM lParam) {
		int x = idFocus & 0xff;
		int y = idFocus >> 8;

		switch (wParam) {
		case VK_UP: y--; break;
		case VK_DOWN: y++; break;
		case VK_LEFT: x--; break;
		case VK_RIGHT: x++; break;
		case VK_HOME: x = y = 0; break;
		case VK_END: x = y = DIV - 1; break;
		default: return 0;
		}

		x = (x + DIV) % DIV;
		y = (y + DIV) % DIV;

		idFocus = y << 8 | x;

		SetFocus(GetDlgItem(*this, idFocus));
		return 0;
	}

};

MESSAGE_MAP_BEGIN(HelloWin)
MESSAGE_MAP_ENTRY_PARAMS(OnSize, WM_SIZE)
MESSAGE_MAP_ENTRY_SINGLE(MessageBeep(0), 0, WM_LBUTTONDOWN)
MESSAGE_MAP_ENTRY_SINGLE(SetFocus(GetDlgItem(*this, idFocus)), 0, WM_SETFOCUS)
MESSAGE_MAP_ENTRY_PARAMS(KeyDown, WM_KEYDOWN)
MESSAGE_MAP_ENTRY_SINGLE(PostQuitMessage(0), 0, WM_DESTROY)
MESSAGE_MAP_END()

ENTRY_APP(HelloWin);