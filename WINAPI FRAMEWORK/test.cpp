#include"Headers\WindowClass.h"
class _SimpleProc :public WndProcs<_SimpleProc> {
public:
	_SimpleProc():WndProcs(this){}
	int WndProc();
private:
	RECT rect;
	HDC hdc;
	strings<TCHAR, 100>p;
	PAINTSTRUCT ps;
}SimpleProc;

int _SimpleProc::WndProc() {
	switch (message) {
	case WM_CREATE:
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		//TextOut(hdc, 50, 50, p << TEXT("Hello World\nI just Made a FRAMEWORK!!") << &p, NULL);
		GetClientRect(hwnd, &rect);
		DrawText(hdc, p << TEXT("Hello Windows ") << 10 << &p, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

int MAIN(){
	Window w;
	w.AttachWndProc(SimpleProc());
	w.Createwindow(TEXT("Hello World"), Position{ 0,0 }, Size{ 400,400 });
	return w.Run();
}