#pragma once
#include"Print.h"
#define tostring(a) #a 

#ifdef EXTENDEDSUPPORT
#define EXSP
#endif

int window_number = 0;

#define MAIN() WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,PSTR szCmdLine,int iCmdShow)

/*WindowClass starts*/

#ifdef EXSP
typedef WNDCLASSEX _WNDCLASS;
#else 
typedef WNDCLASS _WNDCLASS;
#endif

class WindowClass :protected _WNDCLASS {
public:

	WindowClass();
	WindowClass(LPCWSTR _lpszClassName);
	WindowClass(LPCWSTR _lpszClassName, UINT _style);
	void AttachWndProc(WNDPROC _WndProc);
	void ChangeStyle(UINT _style);
	void DefaultClass();
	void ChangeIcon(HICON _hIcon);
#ifdef EXSP
	void ChangeIconSm(HICON _hIconSm);
#endif // EXSP

	void ChangeCursor(HCURSOR hCursor);
	void ChangeClassName(LPCWSTR _lpszClassName);
	bool RegisterClass();
private:
#ifdef _EXE
	//Should'nt be used while making dll files
	void default_instance() { 
		hInstance = GetModuleHandle(NULL); 
	}
#else
	void AddInstance(HINSTANCE _hInstance) { hInstance = _hInstance; }
#endif
};

/*Constructors start*/
WindowClass::WindowClass(LPCWSTR _lpszClassName) {

#ifdef EXSP
	if (!GetClassInfoEx(NULL, _lpszClassName, this)) 
#else
	if (!GetClassInfo(NULL, _lpszClassName, this)) 
#endif //ExSP
	{
		DefaultClass();
	}

}
WindowClass::WindowClass() {
	DefaultClass();
	//_RegisterClass();
}
WindowClass::WindowClass(LPCWSTR _lpszClassName, UINT _style) {
	DefaultClass();
	lpszClassName = _lpszClassName;
	style = _style;
	RegisterClass();
}
/*Constructors finish*/

/*Inline Functions*/
inline void WindowClass::AttachWndProc(WNDPROC _WndProc) {
	lpfnWndProc = _WndProc;
}
inline void WindowClass::ChangeStyle(UINT _style) {
	style = _style;
}
inline void WindowClass::ChangeIcon(HICON _hIcon) {
	hIcon = _hIcon;
}
inline void WindowClass::ChangeCursor(HCURSOR _hCursor) {
	hCursor = _hCursor;
}
inline void WindowClass::ChangeClassName(LPCWSTR _lpszClassname) {
	lpszClassName = _lpszClassname;
}
inline bool WindowClass::RegisterClass() {
#ifdef EXSP
	return ::RegisterClassEx(this);
#else 
	return ::RegisterClass(this);
#endif // EXSP

}

#ifdef EXSP
inline void WindowClass::ChangeIconSm(HICON _hIconSm) {
	hIconSm = _hIconSm;
}
#endif // EXSP

/*Inline Functions functions*/

/*Member Functions*/
void WindowClass::DefaultClass() {
	style = CS_HREDRAW | CS_VREDRAW;
	lpfnWndProc = DefWindowProc;
	cbClsExtra = 0;
	cbWndExtra = 0;
	hCursor = LoadCursor(NULL, IDC_ARROW);
	hIcon = LoadIcon(NULL, IDI_APPLICATION);
	hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	lpszMenuName = NULL;
	lpszClassName = TEXT("Window");
#ifdef _EXE
	default_instance();
#endif

#ifdef EXSP
	cbSize = sizeof(_WNDCLASS);
	hIconSm = NULL;
#endif // EXSP

}

/*WidnowClass ends*/


class Window :public WindowClass {
public:
	Window();
	WPARAM Run();
	WPARAM ProcessMessage();
	/// Inline Functions
	void Createwindow(TCHAR *_caption, Position _pos, Size _size);
	void ShowWindow(int iCmdShow) { ::ShowWindow(this->hwnd,iCmdShow); }
	void UpdateWindow() { ::UpdateWindow(this->hwnd); }
	void ChangeWinStyle(DWORD _WinStyle) {
		Winstyle = _WinStyle;
	}

private:
	HWND hwnd;
	MSG msg;
	HWND hparent;
	TCHAR *caption;
	Position position;
	Size size;
	HMENU hmenu;
	LPARAM lparam;
	LPCWSTR classname;
	DWORD Winstyle;
};

/*Constructors*/
Window::Window() {
	position = Position{ CW_USEDEFAULT, CW_USEDEFAULT };
	size = Size{ CW_USEDEFAULT, CW_USEDEFAULT };
	Winstyle = WS_OVERLAPPEDWINDOW|WS_HSCROLL|WS_VSCROLL;
	hparent = NULL;
	hmenu = NULL;
	hparent = NULL;
	hmenu = NULL;
	lparam = NULL;
	classname = lpszClassName;
	caption = TEXT("Simple Window");
}

/*Member Functions*/
void Window::Createwindow(TCHAR *_caption, Position _pos, Size _size) {
	RegisterClass();
	caption = _caption;
	position = _pos;
	size = _size;
	hwnd = CreateWindow(classname, caption, Winstyle, position.x, position.y, size.x, size.y, hparent, hmenu, hInstance, &lparam);
}
WPARAM Window::ProcessMessage() {
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
WPARAM Window::Run() {
	ShowWindow(1);
	UpdateWindow();
	return ProcessMessage();
}

/*Window class ends*/


/*T is Derived class name*/
template<class T>class WndProcs {
public:
	/*arguement is derived class this pointer*/
	WndProcs(T*_instance) {
		instance = _instance;
	}
	/*Returns callback function WndProc*/
	WNDPROC operator()() {
		return instance->Main;
	}

protected:
	
	virtual int WndProc();
	virtual int WM_destroy() { PostQuitMessage(0); return 0; }

	static LRESULT CALLBACK Main(HWND _hwnd, UINT _message, WPARAM _wParam, LPARAM _lParam);
	
	UINT message;
	HWND hwnd;
	WPARAM wParam;
	LPARAM lParam;
private:
	static T *instance;
	void initialize(HWND _hwnd, UINT _message, WPARAM _wParam, LPARAM _lParam);
};
template<class T>T *WndProcs<T>::instance = nullptr;

template<class T>LRESULT CALLBACK WndProcs<T>::Main(HWND _hwnd, UINT _message, WPARAM _wParam, LPARAM _lParam) {
	instance->initialize(_hwnd, _message, _wParam, _lParam);
	return instance->WndProc();
}
template<class T>void WndProcs<T>::initialize(HWND _hwnd, UINT _message, WPARAM _wParam, LPARAM _lParam) {
	hwnd = _hwnd;
	message = _message;
	wParam = _wParam;
	lParam = _lParam;
}
template<class T>int WndProcs<T>::WndProc() {
	return DefWindowProc(hwnd, message, wParam, lParam);
}
