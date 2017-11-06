#pragma once
#include<Windows.h>
class ScrollInfo :protected SCROLLINFO {
public:
	int SetScrollInfo(HWND hwnd, int nBar, BOOL fRedraw);
	BOOL GetScrollInfo(HWND hwnd, int fnBar);
};

int ScrollInfo::SetScrollInfo(HWND hwnd, int nBar, BOOL fRedraw) {
	//fMask = 0;
	cbSize = sizeof(SCROLLINFO);
	/*int temp = */::SetScrollInfo(hwnd, nBar, this, fRedraw);
	fMask = 0;
	return 0;
}
BOOL ScrollInfo::GetScrollInfo(HWND hwnd, int nBar) {
	//fMask = 0;
	cbSize = sizeof(SCROLLINFO);
	/*BOOL temp = */::GetScrollInfo(hwnd, nBar, this);
	fMask = 0;
	return 0;
}

class Scroll :public ScrollInfo{
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
	int VScroll(HWND hwnd, LPARAM lParam, WPARAM wParam);
	int HScroll(HWND hwnd, LPARAM lParam, WPARAM wParam);
};
void Scroll::Page(int Pages) {
	nPage = Pages;
	fMask |= SIF_PAGE;
}
void Scroll::Position(int Pos) {
	nPos = Pos;
	fMask |= SIF_POS;
}
void Scroll::Range(int Max,int Min){
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

int Scroll::VScroll(HWND hwnd,LPARAM lParam,WPARAM wParam) {
	fMask = SIF_ALL;
	GetScrollInfo(hwnd, SB_VERT);
	int iVertPos = nPos;
	switch (LOWORD(wParam)) {
	case SB_TOP:
		nPos = nMin;
		break;
	case SB_BOTTOM:
		nPos = nMax;
		break;
	case SB_LINEUP:
		nPos -= 1;
		break;
	case SB_LINEDOWN:
		nPos += 1;
		break;
	case SB_PAGEUP:
		nPos -= nPage;
		break;
	case SB_PAGEDOWN:
		nPos += nPage;
		break;
	case SB_THUMBTRACK:
		nPos = nTrackPos;
		break;
	default:
		break;
	}
	fMask = SIF_ALL;
	SetScrollInfo(hwnd, SB_VERT, TRUE);
	fMask = SIF_ALL;
	GetScrollInfo(hwnd, SB_VERT);

	if (nPos != iVertPos) {
#ifdef EXSP
		ScrollWindowEx(hwnd, 0, 7 * (iVertPos - nPos), NULL, NULL, NULL, NULL, SW_INVALIDATE);
#else
		ScrollWindow(hwnd,0 ,16*(iVertPos - nPos), NULL, NULL);
#endif // EXSP

		UpdateWindow(hwnd);
	}
	return 0;

}
int Scroll::HScroll(HWND hwnd, LPARAM lParam, WPARAM wParam) {
	fMask = SIF_ALL;
	GetScrollInfo(hwnd, SB_HORZ);
	int iHorzPos = nPos;
	switch (LOWORD(wParam)) {
	case SB_LINELEFT:
		nPos = 1;
		break;
	case SB_LINERIGHT:
		nPos += 1;
		break;
	case SB_PAGELEFT:
		nPos -= nPage;
		break;
	case SB_PAGERIGHT:
		nPos += nPage;
		break;
	case SB_THUMBPOSITION:
		nPos  = nTrackPos;
		break;
	default:
		break;
	}
	fMask = SIF_ALL;
	SetScrollInfo(hwnd, SB_HORZ, TRUE);
	fMask = SIF_ALL;
	GetScrollInfo(hwnd, SB_HORZ);

	if (nPos != iHorzPos) {
#ifdef EXSP
		ScrollWindowEx(hwnd, 3 * (iHorzPos - nPos), 0, NULL, NULL, NULL, NULL, SW_INVALIDATE);
#else
		ScrollWindow(hwnd, 7 * (iHorzPos - nPos), 0, NULL, NULL);
#endif // EXSP

		UpdateWindow(hwnd);
	}
	return 0;

}
