#include"Headers\SysMets.h"
#include"Headers\WindowClass.h"
#include"TEXT.H"
#define NUMLINES ((int) (sizeof sysmetrics / sizeof sysmetrics [0]))
void TextOut(HDC hdc, int x, int y, LPCWSTR lpString) {
	TextOut(hdc, x, y, lpString, lstrlen(lpString));
}
class sysmets :public WndProcs<sysmets> {
public:
	sysmets():WndProcs(this){}

	int WndProc();
private:
	int cxChar, cyChar, cxCaps;
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR szBuffer[10];
	strings<TCHAR, 300>str;
	Text *t;
//	HDC hdc;
//	TEXTMETRIC t;
}Sysmets;
int sysmets::WndProc() {

	switch (message) {
	case WM_CREATE:
		//t.fillhwnd(hwnd);
		t = new Text(hwnd);
		t->GetTextMetrics();
		cxChar = t->cxChar();
		cyChar = t->cyChar();
		cxCaps = t->cxCaps();
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		for (int i = 0; i < NUMLINES; ++i) {
			//str << sysmetrics[i].szLabel << L"                  " << sysmetrics[i].szDesc << L"                          " << GetSystemMetrics(sysmetrics[i].iIndex);
			//TextOut(hdc, 0, cyChar*i, str << sysmetrics[i].szLabel << L"                  " << sysmetrics[i].szDesc << L"                          " << GetSystemMetrics(sysmetrics[i].iIndex)<<&str );

			TextOut(hdc, 0, cyChar*i, sysmetrics[i].szLabel /*,lstrlen(sysmetrics[i].szLabel)*/);
			SetTextAlign(hdc, TA_LEFT | TA_TOP);
			TextOut(hdc, 22 * cxCaps, cyChar*i, sysmetrics[i].szDesc/*, lstrlen(sysmetrics[i].szDesc)*/);

		//	TextOut(hdc, 22 * cxCaps + 40 * cxChar, cyChar*i, str << GetSystemMetrics(sysmetrics[i].iIndex) << &str/*, wsprintf(szBuffer, TEXT("%5d"), GetSystemMetrics(sysmetrics[i].iIndex))*/);

			SetTextAlign(hdc, TA_RIGHT | TA_TOP);

			
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
	w.AttachWndProc(Sysmets());
	w.Createwindow(L"Sysmets 1", Position{ 0,0 }, Size{ 1900,1000 });
	return w.Run();
}