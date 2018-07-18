#include<Windows.h>
#include"WinProc.hpp"
#include<memory>


class DC {


public:
	DC (HWND _hwnd) 
		:hwnd(_hwnd){
		hdc = GetDC (hwnd);
	}

	//For Inherited PaintDC
protected:
	DC () {}
	
public:
	inline BOOL CancelDC ();
	
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

BOOL DC::CancelDC () {
	return ::CancelDC (hdc);
}


class PaintDC :public DC{
public:
	PaintDC(HWND _hwnd)
		:DC(){

		hwnd = _hwnd;
		hdc = BeginPaint (hwnd, &ps);

	}


	~PaintDC () {
		EndPaint (hwnd, &ps);
		hwnd = nullptr;

	}
public:
	PAINTSTRUCT ps;
};