#include<Windows.h>

class Text {
public:
	Text(){}
	Text(HWND _hwnd) { hwnd = _hwnd; }
	void fillhwnd(HWND _hwnd) { hwnd = _hwnd; }
	void GetTextMetrics();
	long cxChar();
	long cyChar();
	long cxCaps();
	void SetTextAlign(UINT align);
private:
	HDC hdc;
	HWND hwnd;
	TEXTMETRIC tm;
};

	
inline void Text::GetTextMetrics() {
	hdc = GetDC(hwnd);
	::GetTextMetrics(hdc, &tm);
	ReleaseDC(hwnd, hdc);
}
inline long Text::cxChar() { return tm.tmAveCharWidth; }
inline long Text::cyChar() { return tm.tmHeight + tm.tmExternalLeading; }
inline long Text::cxCaps() { return (tm.tmPitchAndFamily & 1 ? 3 : 2)*cxChar() / 2; }
inline void Text::SetTextAlign(UINT align) {
	hdc = GetDC(hwnd); 
	::SetTextAlign(hdc, align); 
	ReleaseDC(hwnd, hdc);
}