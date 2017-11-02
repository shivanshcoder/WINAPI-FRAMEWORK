#pragma once
#include"Print.h"
#define tostring(a) #a 

int window_number = 0;

#define MAIN() WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,PSTR szCmdLine,int iCmdShow)

/*WindowClass starts*/

class WindowClass :protected WNDCLASS {
public:

	WindowClass();
	WindowClass(LPCWSTR _lpszClassName);
	WindowClass(LPCWSTR _lpszClassName, UINT _style);
	void AttachWndProc(WNDPROC _WndProc);
	void ChangeStyle(UINT _style);
	void DefaultClass();
	void ChangeIcon(HICON _hIcon);
	void ChangeCursor(HCURSOR hCursor);
	void ChangeClassName(LPCWSTR _lpszClassName);
	bool _RegisterClass();
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
	if (!GetClassInfo(NULL, _lpszClassName, this)) {
		DefaultClass();
		//_RegisterClass();
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
	_RegisterClass();
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
inline bool WindowClass::_RegisterClass() {
	return RegisterClass(this);
}

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
	DWORD style;
};

/*Constructors*/
Window::Window() {
	position = Position{ CW_USEDEFAULT, CW_USEDEFAULT };
	size = Size{ CW_USEDEFAULT, CW_USEDEFAULT };
	style = WS_OVERLAPPEDWINDOW;
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
	_RegisterClass();
	caption = _caption;
	position = _pos;
	size = _size;
	hwnd = CreateWindow(classname, caption, style, position.x, position.y, size.x, size.y, hparent, hmenu, hInstance, &lparam);
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
