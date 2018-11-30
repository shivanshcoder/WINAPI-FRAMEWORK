#include"Helpers.h"
#include<Windows.h>

//TODO make the wrappers for all the DC operations
class DC {


public:
	DC(HWND _hwnd)
		:hwnd(_hwnd) {}
	
	inline BOOL CancelDC () {
		return ::CancelDC(hdc);
	}

	/*			Wrappers				*/

	void TextOut(WINAPIPP::Point point, std::wstring string) {
		::TextOut(hdc, point.x, point.y, string.c_str(), string.size());
	}

	/*			Wrappers				*/

	operator HDC() {
		return hdc;
	}

	virtual ~DC () {
		if (hwnd) {
			::ReleaseDC (hwnd, hdc);
		}
	}
	

//protected:
protected:
	HWND hwnd;
	HDC hdc;
};

class QuickDC:public DC {
public:
	QuickDC(HWND _hwnd) :DC(_hwnd) {
		hdc = GetDC(_hwnd);
	}
};

//Should be made in WM_PAINT message only
class PaintDC :public DC {
public:
	PaintDC(HWND _hwnd):DC(_hwnd)
		{

		hwnd = _hwnd;
		hdc = BeginPaint (hwnd, &ps);

	}
	operator HDC() {
		return hdc;
	}

	~PaintDC () {
		if (hwnd) {
			EndPaint(hwnd, &ps);
		}
	}
public:
	PAINTSTRUCT ps;
};


//TODO Implement DrawText with this class
class TextCursor {



};