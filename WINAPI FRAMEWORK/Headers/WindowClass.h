#pragma once
#include"Print.h"
#include"Helper.h"
#include<string>
#include<iostream>
#include<assert.h>
#define tostring(a) #a 

#ifdef EXTENDEDSUPPORT
#define EXSP
#endif

#define MAIN() WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,PSTR szCmdLine,int iCmdShow)

#ifdef EXSP
typedef WNDCLASSEX _WNDCLASS;
#else 
typedef WNDCLASS _WNDCLASS;
#endif

//End of Preprocessor Directives


/*WindowClass starts*/


class Window;

bool Change = false;

class WindowClass :protected _WNDCLASS {

	//Window is Friend class so that objects can be accessed without object
	friend class WindowBase;

protected:

	///Constructors
	WindowClass ();
	void DefaultClass ();

public:
	void AttachWndProc ( WNDPROC _WndProc );
	void ChangeWinClassStyle ( UINT _style );
	void ChangeWinClassIcon ( HICON _hIcon );
	void ChangeWinClassCursor ( HCURSOR hCursor );
	void ChangeClassName ( std::wstring *ClassName );
	void ChangeClassName ( LPCWSTR ClassName );
	void ChangeMenuName ( LPCWSTR ClassName );
	void ChangeMenuName ( std::wstring *MenuName );

	bool AnyChange () { return Change; }
	//	bool Change;
		//	void ChangeBackground(HBRUSH NewBackground);
			//void ChangeMenuName()


	bool RegisterClass ();
	bool RegisterClass ( std::wstring NewClassName );
private:

	//Should'nt be used while making dll files
	void default_instance () {
		hInstance = GetModuleHandle ( NULL );
	}

};

///*Constructors start*/
WindowClass::WindowClass () {
	DefaultClass ();
}
/*Constructors finish*/

/*Inline Functions*/

//Attach a Windows Procedure
inline void WindowClass::AttachWndProc ( WNDPROC _WndProc ) {
	Change = true;
	lpfnWndProc = _WndProc;
}

//Change Windows Style
inline void WindowClass::ChangeWinClassStyle ( UINT _style ) {
	Change = true;
	style = _style;
}
inline void WindowClass::ChangeWinClassIcon ( HICON _hIcon ) {
	Change = true;
	hIcon = _hIcon;
}
inline void WindowClass::ChangeWinClassCursor ( HCURSOR _hCursor ) {
	Change = true;
	hCursor = _hCursor;
}
inline void WindowClass::ChangeMenuName ( LPCWSTR MenuName ) {
	Change = true;
	lpszMenuName = MenuName;
}
inline void WindowClass::ChangeMenuName ( std::wstring *MenuName ) {
	Change = true;
	lpszMenuName = ( *MenuName ).c_str ();
}
inline void WindowClass::ChangeClassName ( std::wstring *ClassName ) {
	Change = true;
	lpszClassName = ( *ClassName ).c_str ();
}
inline void WindowClass::ChangeClassName ( LPCWSTR ClassName ) {
	Change = true;
	lpszClassName = ClassName;
}
inline bool WindowClass::RegisterClass () {
	Change = false;
	int a = ::RegisterClass ( this );
	CheckError ();

	return a;
}
inline bool WindowClass::RegisterClass ( std::wstring NewClassName ) {
	Change = false;
	lpszClassName = NewClassName.c_str ();
	RegisterClass ();
}

void WindowClass::DefaultClass () {
	style = CS_HREDRAW | CS_VREDRAW;
	lpfnWndProc = DefWindowProc;
	cbClsExtra = 0;
	cbWndExtra = 0;
	hCursor = LoadCursor ( NULL, IDC_ARROW );
	hIcon = LoadIcon ( NULL, IDI_APPLICATION );
	hbrBackground = ( HBRUSH ) GetStockObject ( WHITE_BRUSH );
	lpszMenuName = NULL;
	lpszClassName = TEXT ( "SimpleWindow" );
	Change = true;
#ifdef _EXE
	default_instance ();
#endif

#ifdef EXSP
	cbSize = sizeof ( _WNDCLASS );
	hIconSm = NULL;
#endif // EXSP

}

/*WidnowClass ends*/

class WindowBase {
public:
	WindowBase ( std::wstring *ClassName ) { _WindowClass->ChangeClassName ( ClassName ); }
	WindowBase ( LPCWSTR ClassName ) { _WindowClass->ChangeClassName ( ClassName ); }
	WindowBase(){}
	/*WindowBase ( std::wstring caption );
	WindowBase ( std::wstring caption, Position pos, Size size );
	WindowBase ( std::wstring classname, std::wstring caption, DWORD style, Position pos, Size size, HWND hParent, HMENU hMenu, LPARAM lParam );
*/

	//void Createwindow ( std::wstring _caption );
	//void Createwindow ( std::wstring _caption, Position _pos, Size _size );
	void Createwindow ( std::wstring ClassName, std::wstring _caption, DWORD style, Position _pos, Size _size, HWND hparent, HMENU hmenu, LPARAM lParam );
	//void CreateWindow ( std::wstring ClassName, std::wstring _caption, DWORD style, Position _pos, Size _size,
	//	HWND hparent = NULL, HMENU hmenu = NULL, LPARAM lParam = NULL);
	void MoveWindow ( Position Pos, Size Siz, bool bRepaint = true );

	void ShowWindow ( int iCmdShow ) { ::ShowWindow ( hwnd, iCmdShow ); }
	void UpdateWindow () { ::UpdateWindow ( hwnd ); }
	void ChangeWinStyle ( DWORD _WinStyle ) { SetInfo ( GWL_STYLE, _WinStyle ); }

	void AttachWinProcedure ( WNDPROC _WndProc );

	void SetFocus ();
	LONG_PTR GetInfo ( int Index );
	LONG_PTR SetInfo ( int Index, LONG_PTR NewVal );
	LONG_PTR hParent ();

	WPARAM Run ();
	WPARAM ProcessMessage ();

	//~WindowBase () {
	/////	int a;
	//	while(GetMessage ( &msg, NULL, 0, 0 ) ) {
	//		/*	if (!PeekMessage(&msg, hwnd, 0, 0, PM_NOREMOVE))
	//		return ;*/
	//		TranslateMessage ( &msg );
	//		DispatchMessage ( &msg );
	//	}

	//	///return msg.wParam;
	//}
protected:
	///Handles to Windows
	HWND hwnd;
	static MSG msg;
	static WindowClass* _WindowClass;
};
WindowClass *WindowBase::_WindowClass = new WindowClass;
MSG WindowBase::msg = {};

//Constructors
//inline WindowBase::WindowBase ( std::wstring caption ) {
//	if( _WindowClass->AnyChange () )
//		_WindowClass->RegisterClass ();
//
//	hwnd = CreateWindow ( TEXT ( "SimpleWindow" ), caption.c_str (), WS_OVERLAPPEDWINDOW,
//		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, _WindowClass->hInstance, nullptr );
//
//}
//inline WindowBase::WindowBase ( std::wstring caption, Position pos, Size size ) {
//	if( _WindowClass->AnyChange () )
//		_WindowClass->RegisterClass ();
//
//	hwnd = CreateWindow ( TEXT ( "SimpleWindow" ), caption.c_str (), WS_OVERLAPPEDWINDOW,
//		pos.x, pos.y, size.x, size.y, nullptr, nullptr, _WindowClass->hInstance, nullptr );
//}
//inline WindowBase::WindowBase ( std::wstring classname, std::wstring caption, DWORD style, Position pos, Size size, HWND hParent, HMENU hMenu, LPARAM lParam ) {
//	if( _WindowClass->AnyChange () )
//		_WindowClass->RegisterClass ();
//
//	hwnd = CreateWindow ( classname.c_str (), caption.c_str (), style, pos.x, pos.y, size.x, size.y, hParent, hMenu, _WindowClass->hInstance, &lParam );
//}


/*Member Functions*/
inline void WindowBase::Createwindow ( std::wstring ClassName, std::wstring Caption, DWORD Style = WS_OVERLAPPEDWINDOW,
	Position Pos = { CW_USEDEFAULT,CW_USEDEFAULT }, Size Siz = { CW_USEDEFAULT,CW_USEDEFAULT },
	HWND hParent = nullptr, HMENU hMenu = nullptr, LPARAM lParam = NULL) {
	if( _WindowClass->AnyChange () )
		_WindowClass->RegisterClass ();

	hwnd = CreateWindow ( ClassName.c_str (), Caption.c_str (), Style, Pos.x, Pos.y, Siz.x, Siz.y, hParent, hMenu, _WindowClass->hInstance, &lParam );
}
//inline void WindowBase::Createwindow ( std::wstring _caption, Position _pos, Size _size ) {
//	//if( _WindowClass->AnyChange () )
////	_WindowClass->RegisterClass ();
//
//	hwnd = CreateWindow ( TEXT ( "SimpleWindow" ), _caption.c_str (), WS_OVERLAPPEDWINDOW, _pos.x, _pos.y, _size.x, _size.y, nullptr, nullptr, _WindowClass->hInstance, nullptr );
//	CheckError ();
//}
//inline void WindowBase::Createwindow ( std::wstring _caption ) {
//	if( _WindowClass->AnyChange () )
//		_WindowClass->RegisterClass ();
//	//	hwnd
//}
inline void WindowBase::MoveWindow ( Position Pos, Size Siz, bool bRepaint ) {
	::MoveWindow ( hwnd, Pos.x, Pos.y, Siz.x, Siz.y, bRepaint );
}

inline void WindowBase::SetFocus () { 
	::SetFocus ( hwnd ); 
}

void WindowBase::AttachWinProcedure ( WNDPROC _WndProc ) {
	if( hwnd == NULL ) {
		_WindowClass->AttachWndProc ( _WndProc );
		_WindowClass->RegisterClass ();
	}
	else
		SetInfo ( GWLP_WNDPROC, ( LONG ) _WndProc );
}

inline LONG_PTR WindowBase::GetInfo ( int Index ) {
	return GetWindowLongPtr ( hwnd, Index );
}
inline LONG_PTR WindowBase::SetInfo ( int Index, LONG_PTR NewVal ) {
	return SetWindowLongPtr ( hwnd, Index, NewVal );
}
inline LONG_PTR WindowBase::hParent () {
	return GetInfo ( GWLP_HWNDPARENT );
}
WPARAM WindowBase::ProcessMessage () {
	static int a;
	while( a = GetMessage ( &msg, NULL, 0, 0 ) ) {
		TranslateMessage ( &msg );
		DispatchMessage ( &msg );
	}

	return msg.wParam;
}
WPARAM WindowBase::Run () {
	ShowWindow ( 1 );
	UpdateWindow ();
	return ProcessMessage ();
	/*while (1) {
		CheckMessage();
		std::cout << "Hello";
	}*/
	return msg.wParam;
}



class SimpleWindow :public WindowBase {
public:
	SimpleWindow ();
private:
	///Handles to Windows
	HWND hwnd;
	std::wstring classname;
	Position position;
	Size size;

};
/*Constructor*/

SimpleWindow::SimpleWindow () :WindowBase ( &( classname = L"SimpleWindow" ) ) {
	position = Position{ CW_USEDEFAULT, CW_USEDEFAULT };
	size = Size{ CW_USEDEFAULT, CW_USEDEFAULT };
	//Winstyle = WS_OVERLAPPEDWINDOW;
	//classname = L"SimpleWindow";
}


/*Window class ends*/

/*T is Derived class name*/
template<class T>class WndProcs {
public:
	/*arguement is derived class this pointer*/
	WndProcs ( T*_instance ) {
		instance = _instance;
	}
	/*Returns callback function WndProc*/
	WNDPROC operator()() {
		return instance->Main;
	}

protected:

	virtual int WndProc () = 0;
	virtual int WM_destroy () { PostQuitMessage ( 0 ); return 0; }
	virtual void UpdateSize(){
		size.x = LOWORD ( lParam );
		size.y = HIWORD ( lParam );
	}

	static LRESULT CALLBACK Main ( HWND _hwnd, UINT _message, WPARAM _wParam, LPARAM _lParam );

	UINT message;
	HWND hwnd;
	WPARAM wParam;
	LPARAM lParam;
	//Client Size
	Size size;
private:
	static T *instance;
	void initialize ( HWND _hwnd, UINT _message, WPARAM _wParam, LPARAM _lParam );
};
template<class T>T *WndProcs<T>::instance = nullptr;

template<class T>LRESULT CALLBACK WndProcs<T>::Main ( HWND _hwnd, UINT _message, WPARAM _wParam, LPARAM _lParam ) {
	instance->initialize ( _hwnd, _message, _wParam, _lParam );
	return instance->WndProc ();
}
template<class T>void WndProcs<T>::initialize ( HWND _hwnd, UINT _message, WPARAM _wParam, LPARAM _lParam ) {
	hwnd = _hwnd;
	message = _message;
	wParam = _wParam;
	lParam = _lParam;
}

static LRESULT CALLBACK DefaultWndProc ( HWND _hwnd, UINT _message, WPARAM _wParam, LPARAM _lParam ) {
	switch( _message ) {
	case WM_DESTROY:
		PostQuitMessage ( 0 );
		return 0;
	}
	return DefWindowProc ( _hwnd, _message, _wParam, _lParam );
}

