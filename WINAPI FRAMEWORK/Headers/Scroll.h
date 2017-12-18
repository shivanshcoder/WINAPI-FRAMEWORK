#pragma once
#include<Windows.h>
class ScrollInfo :public SCROLLINFO {
public:
	int SetScrollInfo(HWND hwnd, int nBar, BOOL fRedraw);
	BOOL GetScrollInfo(HWND hwnd, int fnBar);
};

int ScrollInfo::SetScrollInfo(HWND hwnd, int nBar, BOOL fRedraw) {
	cbSize = sizeof(SCROLLINFO);
	int temp = ::SetScrollInfo(hwnd, nBar, this, fRedraw);
	fMask = 0;
	return temp;
}
BOOL ScrollInfo::GetScrollInfo(HWND hwnd, int nBar) {
	cbSize = sizeof(SCROLLINFO);
	BOOL temp = ::GetScrollInfo(hwnd, nBar, this);
	fMask = 0;
	return temp;
}

class Scroll :protected ScrollInfo{
public:
	Scroll() {
		cbSize = sizeof(SCROLLINFO);
		fMask = 0;
	}
	void Range(int Max, int Min);
	void Page(int Pages);
	void Position(int Pos);

	int CurrentPosV(HWND hwnd);
	int CurrentPosH(HWND hwnd);
	int VScroll(HWND hwnd, LPARAM lParam, WPARAM wParam, int amountSc = 16);
	virtual int VScrollFunc() = 0;

	int HScroll(HWND hwnd, LPARAM lParam, WPARAM wParam, int amountSc = 7);
	virtual int HScrollFunc() = 0;
protected:

	//Horizontal Scroll Functions

	virtual inline void LineLeft() { nPos -= 1; }
	virtual inline void LineRight() { nPos += 1; }
	virtual inline void PageLeft() { nPage -= 1; }
	virtual inline void PageRight() { nPage += 1; }
	virtual inline void ThumbPosition() { nPos = nTrackPos; }

	//Vertical Scroll Functions

	virtual inline void Top() { nPos = nMin; }
	virtual inline void Bottom() { nPos = nMax; }
	virtual inline void LineUp() { nPos -= 1; }
	virtual inline void LineDown() { nPos += 1; }
	virtual inline void PageUp() { nPage -= 1; }
	virtual inline void PageDown() { nPage += 1; }

private:
	int PreviousnPos;
};

void Scroll::Page(int Pages) {
	nPage = Pages;
	fMask |= SIF_PAGE;
}
void Scroll::Position(int Pos) {
	nPos = Pos;
	fMask |= SIF_POS;
}
void Scroll::Range(int Min,int Max){
	nMin = Min;
	nMax = Max;
	fMask |= SIF_RANGE;
}

int Scroll::CurrentPosV(HWND hwnd) {
	fMask = SIF_POS;
	GetScrollInfo(hwnd, SB_VERT);
	return nPos;
}
int Scroll::CurrentPosH(HWND hwnd) {
	fMask = SIF_POS;
	GetScrollInfo(hwnd, SB_HORZ);
	return nPos;
}

int Scroll::VScroll(HWND hwnd, LPARAM lParam, WPARAM wParam, int amountSc) {
	fMask = SIF_ALL;
	GetScrollInfo(hwnd, SB_VERT);
	PreviousnPos = nPos;
	switch (LOWORD(wParam)) {
	case SB_TOP:
		Top();
		//nPos = nMin;

		break;
	case SB_BOTTOM:
		//nPos = nMax;
		Bottom();
		break;
	case SB_LINEUP:
		//nPos -= 1;
		LineUp();
		break;
	case SB_LINEDOWN:
		//nPos += 1;
		LineDown();
		break;
	case SB_PAGEUP:
		PageUp();
		//nPos -= nPage;
		break;
	case SB_PAGEDOWN:
		PageDown();
		//nPos += nPage;
		break;
	case SB_THUMBTRACK:
		
		//nPos = nTrackPos;
		break;
	default:
		break;
	}
	return VScrollFunc();
//	fMask = SIF_ALL;
//	SetScrollInfo(hwnd, SB_VERT, TRUE);
//	fMask = SIF_ALL;
//	GetScrollInfo(hwnd, SB_VERT);
//
//	if (nPos != iVertPos) {
//#ifdef EXSP
//		ScrollWindowEx(hwnd, 0, amountSc * (iVertPos - nPos), NULL, NULL, NULL, NULL, SW_INVALIDATE);
//#else
//		ScrollWindow(hwnd, 0, amountSc * (iVertPos - nPos), NULL, NULL);
//#endif // EXSP
//
//		UpdateWindow(hwnd);
//	}
//	return 0;

}
int Scroll::HScroll(HWND hwnd, LPARAM lParam, WPARAM wParam, int amountSc) {
	fMask = SIF_ALL;
	GetScrollInfo(hwnd, SB_HORZ);
	PreviousnPos = nPos;
	switch (LOWORD(wParam)) {
	case SB_LINELEFT:
	//	nPos -= 1;
		LineLeft();
		break;
	case SB_LINERIGHT:
	//	nPos += 1;
		LineRight();
		break;
	case SB_PAGELEFT:
	//	nPos -= nPage;
		PageLeft();
		break;
	case SB_PAGERIGHT:
	//	nPos += nPage;
		PageRight();
		break;
	case SB_THUMBPOSITION:
		ThumbPosition();
	//	nPos  = nTrackPos;
		break;
	default:
		break;
	}
	return HScrollFunc();
//	fMask = SIF_ALL;
//	SetScrollInfo(hwnd, SB_HORZ, TRUE);
//	fMask = SIF_ALL;
//	GetScrollInfo(hwnd, SB_HORZ);
//
//	if (nPos != iHorzPos) {
//#ifdef EXSP
//		ScrollWindowEx(hwnd, amountSc * (iHorzPos - nPos), 0, NULL, NULL, NULL, NULL, SW_INVALIDATE);
//#else
//		ScrollWindow(hwnd, amountSc * (iHorzPos - nPos), 0, NULL, NULL);
//#endif // EXSP
//
//		UpdateWindow(hwnd);
//	}
//	return 0;

}



