#include<Windows.h>

/*T is Derived class name*/
class AdvWndProc;

template<class T>class WndProcs {
	friend class AdvWndProc;
public:
	/*arguement is derived class this pointer*/
	WndProcs 
	( T* _instance ) {
		instance = _instance;
	}
	/*Returns callback function WndProc*/
	WNDPROC operator()() {
		return instance->Main;
	}

protected:
	virtual LONG_PTR WndProc () = 0;
	virtual void UpdateSize () {
		size.x = LOWORD ( lParam );
		size.y = HIWORD ( lParam );
	}

	static LRESULT CALLBACK Main 
	( HWND _hwnd, UINT _message, WPARAM _wParam, LPARAM _lParam );

	UINT message;
	HWND hwnd;
	WPARAM wParam;
	LPARAM lParam;
	Size size;
private:
	static T *instance;
	void initialize 
	( HWND _hwnd, UINT _message, WPARAM _wParam, LPARAM _lParam );
};
template<class T>
T *WndProcs<T>::instance = nullptr;

template<class T>
LRESULT CALLBACK WndProcs<T>::Main
( HWND _hwnd, UINT _message, WPARAM _wParam, LPARAM _lParam )
{
	instance->initialize ( _hwnd, _message, _wParam, _lParam );
	return instance->WndProc ();
}


template<class T>
void WndProcs<T>::initialize
( HWND _hwnd, UINT _message, WPARAM _wParam, LPARAM _lParam ) {
	hwnd = _hwnd;
	message = _message;
	wParam = _wParam;
	lParam = _lParam;
}

static LRESULT CALLBACK DefaultWndProc
( HWND _hwnd, UINT _message, WPARAM _wParam, LPARAM _lParam ){
	switch( _message ) {
	case WM_DESTROY:
		PostQuitMessage ( 0 );
		return 0;
	}
	return DefWindowProc ( _hwnd, _message, _wParam, _lParam );
}



template<class T>class AdvWndProcs :public WndProcs<T>{
public:
	AdvWndProcs(T* _instance):WndProcs(_instance)
	{ DefaultHandlingUsed = true; }

	LONG_PTR WndProc ();

protected:
	virtual LONG_PTR AdvWndProc () = 0;

	LONG_PTR DefaultHandling ();
private:
	bool DefaultHandlingUsed;
};
template<class T>LONG_PTR AdvWndProcs<T>::WndProc () {
	LONG_PTR return_val = DefaultHandling ();
	if( DefaultHandlingUsed )return return_val;
	return AdvWndProc ();

}
template<class T>LONG_PTR AdvWndProcs<T>::DefaultHandling () {
	DefaultHandlingUsed = true;
	switch ( message ) {
	case WM_DESTROY:
		PostQuitMessage ( 0 );

		return 0;
	default:
		DefaultHandlingUsed = false;
	}
}