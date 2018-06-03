#pragma once
#include<Windows.h>

class Basic_DC {

public:
	virtual operator HDC() {
		return DeviceContext;
	}

	Basic_DC operator=(HDC hdc) {
		DeviceContext = hdc;
		return *this;
	}

	Basic_DC operator=(HDC &hdc) {
		DeviceContext = hdc;
	}


protected:

	HDC DeviceContext;
};

class Temp_DC :public Basic_DC{
public:
	Temp_DC (HWND hwnd) :Window (hwnd) {}
	
	void InitDC () {
		DeviceContext = GetDC (Window);
	}

	void InitDC (HWND _Window) {
		Window = _Window;
		DeviceContext = GetDC (Window);
	}


	~Temp_DC () {
		ReleaseDC (Window, *this);
	}

private:
	HWND Window;
};