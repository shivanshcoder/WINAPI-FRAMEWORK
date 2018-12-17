
#include"Header/WINAPIPP.h"

#define DIV 5


class HelloWin : public WINAPIPP::Application {

	int cxBlock, cyBlock;
	bool fState[DIV][DIV];
public:

	WPARAM start()override {

		CreateWin(TEXT("The Hello Program"),
			WS_OVERLAPPEDWINDOW,
			Helpers::CPPRectangle(CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT),
			nullptr);

		return Run();
	}

	int OnSize(WPARAM wParam, LPARAM lParam) override {
		cxBlock = LOWORD(lParam)/DIV;
		cyBlock = HIWORD(lParam)/DIV;

		return 0;
	}
	int OnMouseDown(WPARAM wParam, LPARAM lParam)override {
		int x = LOWORD(lParam) / cxBlock;
		int y = HIWORD(lParam) / cyBlock;
		RECT rect;
		if (x < DIV && y < DIV) {
			fState[x][y] ^= 1;

			rect.left = x * cxBlock;
			rect.top = y * cyBlock;
			rect.right = (x + 1) *cxBlock;
			rect.bottom = (y + 1)*cyBlock;

			InvalidateRect(*this, &rect, FALSE);

		}
		else
			MessageBeep(0);
		return 0;
	}
	int OnPaint() override {
		WINAPIPP::PaintDC dc(*this);

		for (int x = 0; x < DIV; ++x) {
			for (int y = 0; y < DIV; ++y) {
				::Rectangle(dc, x*cxBlock, y*cyBlock,
					(x + 1)*cxBlock, (y + 1)*cyBlock);
				if (fState[x][y]) {
					MoveToEx(dc, x*cxBlock, y*cyBlock, NULL);
					LineTo(dc, (x + 1)*cxBlock, (y + 1)*cyBlock);
					MoveToEx(dc, x*cxBlock, (y + 1)*cyBlock, NULL);
					LineTo(dc, (x + 1)*cxBlock, y*cyBlock);
				}
			}
		}
		return 0;
	}

	int OnDestroy() override {
		PostQuitMessage(0);
		return 0;
	}


};



//ENTRY_FUNC(start);
ENTRY_APP(HelloWin);