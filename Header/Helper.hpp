#pragma once
#include<Windows.h>

class Basic_DC {

public:
	virtual operator HDC() {
		return DeviceClient;
	}

	Basic_DC operator=(HDC hdc) {
		DeviceClient = hdc;
		return *this;
	}

	Basic_DC operator=(HDC &hdc) {
		DeviceClient = hdc;
	}


protected:

	HDC DeviceClient;
};

class Temp_DC :public Basic_DC{
public:
	Temp_DC (HWND hwnd) :Window (hwnd) { DeviceClient = GetDC (hwnd); }
	~Temp_DC () {
		ReleaseDC (Window, *this);
	}

private:
	HWND Window;
};