#include"Headers\WindowClass.h"
#include"TEXT.H"
#include"Scroll.h"
#include"Headers\SysMets.h"

class sysmets :public WndProcs<sysmets> {
public:
	sysmets() :WndProcs(this){}
	int WndProc();
private:
	int iMaxWidth;
	Text *t;
	Size Client;
	Scroll sc;
	HDC hdc;
	PAINTSTRUCT ps;
	int iPaintBeg, iPaintEnd;
	int x, y;
	int cx, cy;
	strings<TCHAR,100> out;
}sysmet;

int sysmets::WndProc() {
	switch (message) {
	case WM_CREATE:

		t = new Text(hwnd);
		t->GetTextMetrics();
		iMaxWidth = 40 * t->cxChar() + 22 * t->cxCaps();
		return 0;

	case WM_SIZE:

		Client.x = LOWORD(lParam);
		Client.y = HIWORD(lParam);

		sc.Range(0, NUMLINES-1);
		sc.Page(Client.y / t->cyChar());
		sc.SetScrollInfo(hwnd, SB_VERT, TRUE);

		sc.Range(0, 2 + iMaxWidth / t->cxChar());
		sc.Page(Client.y / t->cxChar());
		sc.SetScrollInfo(hwnd, SB_HORZ, TRUE);

		return 0;

	case WM_VSCROLL:
		return sc.VScroll(hwnd, lParam, wParam);
	case WM_HSCROLL:
		return sc.HScroll(hwnd, lParam, wParam);

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		iPaintBeg = max(0,
			sc.CurrentPosV(hwnd) + ps.rcPaint.top / t->cyChar());
		iPaintEnd = min(NUMLINES - 1,
			sc.CurrentPosV(hwnd) + ps.rcPaint.bottom / t->cyChar());

			out.hdc = GetDC(hwnd);
		for (int i = iPaintBeg; i <= iPaintEnd; ++i) {

			x = t->cxChar()*(1 - sc.CurrentPosH(hwnd));

			y = t->cyChar()*(i - sc.CurrentPosV(hwnd));

			out[{x,y}] << sysmetrics[i].szLabel;

			out[{x + 22 * t->cxCaps(), y}] << sysmetrics[i].szDesc;

		//	SetTextAlign(hdc, TA_RIGHT | TA_TOP);
			out[{x+22*t->cxCaps()+40*t->cxChar(), y}] << GetSystemMetrics(sysmetrics[i].iIndex);
		//	SetTextAlign(hdc, TA_LEFT | TA_TOP);
			out.finish();
		}
		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		return WM_destroy();

	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

int MAIN() {
	Window w;
	w.AttachWndProc(sysmet());
//	w.ChangeWinStyle(WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL);
	w.Createwindow(L"Sysmets 3", Position{ 0,0 }, Size{ 1000,600 });
	return w.Run();
}
