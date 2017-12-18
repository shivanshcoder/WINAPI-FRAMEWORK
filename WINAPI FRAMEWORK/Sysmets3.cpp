#include"Headers\WindowClass.h"
#include"TEXT.H"
#include"Scroll.h"
#include"Headers\SysMets.h"

class sysmets :public WndProcs<sysmets> {
public:
	sysmets() :WndProcs(this){}
	int WndProc();
	int WndProcz();
private:
	int iMaxWidth;
	Text *t;
	Size Client;
//	int temp;
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
		sc.Page(Client.x / t->cxChar());
		sc.SetScrollInfo(hwnd, SB_HORZ, TRUE);

		return 0;

	case WM_VSCROLL:
		return sc.VScroll(hwnd, lParam, wParam,t->cyChar());
	case WM_HSCROLL:
		return sc.HScroll(hwnd, lParam, wParam,t->cxChar());

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

int sysmets::WndProcz()
{
	static int  cxChar, cxCaps, cyChar, cxClient, cyClient, iMaxWidth;
	HDC         hdc;
	int         i, x, y, iVertPos, iHorzPos, iPaintBeg, iPaintEnd;
	PAINTSTRUCT ps;
	SCROLLINFO  si;
	TCHAR       szBuffer[10];
	TEXTMETRIC  tm;

	switch (message)
	{
	case WM_CREATE:
		hdc = GetDC(hwnd);

		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;

		ReleaseDC(hwnd, hdc);

		// Save the width of the three columns

		iMaxWidth = 40 * cxChar + 22 * cxCaps;
		return 0;

	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);

		// Set vertical scroll bar range and page size

		si.cbSize = sizeof(si);
		si.fMask = SIF_RANGE | SIF_PAGE;
		si.nMin = 0;
		si.nMax = NUMLINES - 1;
		si.nPage = cyClient / cyChar;
		SetScrollInfo(hwnd, SB_VERT, &si, TRUE);

		// Set horizontal scroll bar range and page size

		si.cbSize = sizeof(si);
		si.fMask = SIF_RANGE | SIF_PAGE;
		si.nMin = 0;
		si.nMax = 2 + iMaxWidth / cxChar;
		si.nPage = cxClient / cxChar;
		SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
		return 0;

	//case WM_VSCROLL:
	//	// Get all the vertial scroll bar information

	//	si.cbSize = sizeof(si);
	//	si.fMask = SIF_ALL;
	//	GetScrollInfo(hwnd, SB_VERT, &si);

	//	// Save the position for comparison later on

	//	iVertPos = si.nPos;

	//	switch (LOWORD(wParam))
	//	{
	//	case SB_TOP:
	//		si.nPos = si.nMin;
	//		break;

	//	case SB_BOTTOM:
	//		si.nPos = si.nMax;
	//		break;

	//	case SB_LINEUP:
	//		si.nPos -= 1;
	//		break;

	//	case SB_LINEDOWN:
	//		si.nPos += 1;
	//		break;

	//	case SB_PAGEUP:
	//		si.nPos -= si.nPage;
	//		break;

	//	case SB_PAGEDOWN:
	//		si.nPos += si.nPage;
	//		break;

	//	case SB_THUMBTRACK:
	//		si.nPos = si.nTrackPos;
	//		break;

	//	default:
	//		break;
	//	}
	//	// Set the position and then retrieve it.  Due to adjustments
	//	//   by Windows it may not be the same as the value set.

	//	si.fMask = SIF_POS;
	//	SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
	//	GetScrollInfo(hwnd, SB_VERT, &si);

	//	// If the position has changed, scroll the window and update it

	//	if (si.nPos != iVertPos)
	//	{
	//		ScrollWindow(hwnd, 0, cyChar * (iVertPos - si.nPos),
	//			NULL, NULL);
	//		UpdateWindow(hwnd);
	//	}
	//	return 0;

	//case WM_HSCROLL:
	//	// Get all the vertial scroll bar information

	//	si.cbSize = sizeof(si);
	//	si.fMask = SIF_ALL;

	//	// Save the position for comparison later on

	//	GetScrollInfo(hwnd, SB_HORZ, &si);
	//	iHorzPos = si.nPos;

	//	switch (LOWORD(wParam))
	//	{
	//	case SB_LINELEFT:
	//		si.nPos -= 1;
	//		break;

	//	case SB_LINERIGHT:
	//		si.nPos += 1;
	//		break;

	//	case SB_PAGELEFT:
	//		si.nPos -= si.nPage;
	//		break;

	//	case SB_PAGERIGHT:
	//		si.nPos += si.nPage;
	//		break;

	//	case SB_THUMBPOSITION:
	//		si.nPos = si.nTrackPos;
	//		break;

	//	default:
	//		break;
	//	}
	//	// Set the position and then retrieve it.  Due to adjustments
	//	//   by Windows it may not be the same as the value set.

	//	si.fMask = SIF_POS;
	//	SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
	//	GetScrollInfo(hwnd, SB_HORZ, &si);

	//	// If the position has changed, scroll the window 

	//	if (si.nPos != iHorzPos)
	//	{
	//		ScrollWindow(hwnd, cxChar * (iHorzPos - si.nPos), 0,
	//			NULL, NULL);
	//	}
	//	return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		// Get vertical scroll bar position

		si.cbSize = sizeof(si);
		si.fMask = SIF_POS;
		GetScrollInfo(hwnd, SB_VERT, &si);
		iVertPos = si.nPos;

		// Get horizontal scroll bar position

		GetScrollInfo(hwnd, SB_HORZ, &si);
		iHorzPos = si.nPos;

		// Find painting limits

		iPaintBeg = max(0, iVertPos + ps.rcPaint.top / cyChar);
		iPaintEnd = min(NUMLINES - 1,
			iVertPos + ps.rcPaint.bottom / cyChar);

		for (i = iPaintBeg; i <= iPaintEnd; i++)
		{
			x = cxChar * (1 - iHorzPos);
			y = cyChar * (i - iVertPos);

			TextOut(hdc, x, y,
				sysmetrics[i].szLabel,
				lstrlen(sysmetrics[i].szLabel));

			TextOut(hdc, x + 22 * cxCaps, y,
				sysmetrics[i].szDesc,
				lstrlen(sysmetrics[i].szDesc));

			SetTextAlign(hdc, TA_RIGHT | TA_TOP);

			TextOut(hdc, x + 22 * cxCaps + 40 * cxChar, y, szBuffer,
				wsprintf(szBuffer, TEXT("%5d"),
					GetSystemMetrics(sysmetrics[i].iIndex)));

			SetTextAlign(hdc, TA_LEFT | TA_TOP);
		}

		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

int MAIN() {
	Window w;
	w.AttachWndProc(sysmet());
	w.ChangeWinStyle(WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL);
	w.Createwindow(L"Sysmets 3", Position{ 0,0 }, Size{ 400,300 });
	return w.Run();
}
