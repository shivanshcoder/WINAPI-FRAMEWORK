
#include"Header/WINAPIPP.h"
#include<vector>
#define DIV 5


class ChildWindows :public WINAPIPP::Window {
	bool flag = false;

public:
	ChildWindows(BaseWin Parent, int index) {
		CreateWin(std::wstring(), WS_CHILDWINDOW | WS_VISIBLE, Helpers::CPPRectangle(0, 0, 0, 0), &Parent, (HMENU)index);
	}
	
	CLASS_ALL_PROPERTIES(ChildWindows, CS_HREDRAW | CS_VREDRAW, NULL, LoadCursor(NULL, IDC_ARROW), (HBRUSH)GetStockObject(WHITE_BRUSH), NULL)

	DECLARE_MESSAGE_MAP();

	int LButtonDown() {
		flag = (1 ^ flag);
		InvalidateRect(*this, NULL, FALSE);
		return 0;
	}

	int Paint() {
		WINAPIPP::PaintDC dc(*this);
		RECT rect;
		GetClientRect(*this, &rect);
		Rectangle(dc, rect.left, rect.top, rect.right, rect.bottom);
		if (flag) {
			MoveToEx(dc, 0, 0, NULL);
			LineTo(dc, rect.right, rect.bottom);
			MoveToEx(dc, 0, rect.bottom, NULL);
			LineTo(dc, rect.right, 0);
		}
		return 0;
	}

	int Destroy() {
		PostQuitMessage(0);
		return 0;
	}
};

MESSAGE_MAP_BEGIN(ChildWindows)
//MESSAGE_MAP_ENTRY(Create, WM_CREATE)
MESSAGE_MAP_ENTRY(LButtonDown, WM_LBUTTONDOWN)
MESSAGE_MAP_ENTRY(Paint, WM_PAINT)
MESSAGE_MAP_ENTRY(Destroy, WM_DESTROY)
MESSAGE_MAP_END();

class HelloWin : public WINAPIPP::Application {
	std::vector<std::vector<std::unique_ptr<ChildWindows> > > Children;
	//ChildWindows a, b, c, d, e;
	int cxBlock, cyBlock;
public:
	HelloWin()
		:Application(
			std::wstring(L"Click On"), WS_OVERLAPPEDWINDOW) {}

	WPARAM start()override {
		return Run();
	}
	int OnSize(WPARAM wParam, LPARAM lParam) override {
		Children.clear();
		Children.resize(5);
		for (int x = 0; x < DIV; ++x) {
			for (int y = 0; y < DIV; ++y) {
				Children[x].push_back(std::make_unique<ChildWindows>(*this, (y<<8|x)));
				
			}
		}

 		cxBlock = LOWORD(lParam)/DIV;
		cyBlock = HIWORD(lParam)/DIV;

		for (int x = 0; x < DIV; ++x) {
			for (int y = 0; y < DIV; ++y) {
				MoveWindow(*Children[x][y], x*cxBlock, y*cyBlock, cxBlock, cyBlock, TRUE);
			}
		}
		return 0;
	}
	int OnMouseDown(WPARAM wParam, LPARAM lParam)override {
		MessageBeep(0);
		return 0;
	}
	
	int OnDestroy() override {
		PostQuitMessage(0);
		return 0;
	}

	~HelloWin()override{}
};



//ENTRY_FUNC(start);
ENTRY_APP(HelloWin);