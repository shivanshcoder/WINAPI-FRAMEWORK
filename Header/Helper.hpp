#pragma once
#include<Windows.h>

class Basic_DC {
public:
	Basic_DC () :ValidDC (true) {}

	Basic_DC (HDC hdc) {
		DeviceContext = std::make_shared<HDC> (hdc);
	}
	virtual operator HDC() {
		return *DeviceContext;
	}

	virtual operator std::shared_ptr<HDC> () {
		return DeviceContext;
	}

	Basic_DC operator=(HDC hdc) {
		DeviceContext = std::make_shared<HDC>(hdc);
		return *this;
	}
	
	virtual ~Basic_DC () {
		//ReleaseDC (WindowFromDC (*DeviceContext), *DeviceContext);
		DeviceContext.reset ();
		ValidDC = false;
	}


	void GetDC (HWND _Window) {
		DeviceContext = std::make_shared<HDC> (::GetDC (_Window));
	}

protected:
	std::shared_ptr<HDC>DeviceContext;
	bool ValidDC;
};

//class Temp_DC :public Basic_DC{
//public:
//	//Temp_DC (HWND hwnd) :Window (hwnd) {}
//	
//	//void InitDC () {
//	//	DeviceContext = std::make_shared<HDC> (GetDC (Window));
//	//}
//
//	void InitDC (HWND _Window) {
//		//Window = _Window;
//		DeviceContext = std::make_shared<HDC> (GetDC (_Window));
//	}
//
//
//	/*~Temp_DC () override{
//		ReleaseDC (Window, *this);
//	}*/
//
////private:
//	//HWND Window;
//};