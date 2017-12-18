#include"Headers\WindowClass.h"
#include"Headers\Scroll.h"
#include"Headers\TEXT.H"
#include<sstream>
class test :public WndProcs<test> {
public:
	int WndProc();
	Size client;
	strings<TCHAR, 100>out;
	PAINTSTRUCT ps;
	HDC hdc;
};

int test::WndProc() {
	switch (message) {
	//case WM_CREATE:
	case WM_SIZE:
		client.x = LOWORD(lParam);
		client.y = HIWORD(lParam);
		return 0;
	//case WM_HSCROLL:
		//ScrollWindow(hwnd,7,0,)
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		for (int i = 0; i < (client.y) - 100; i += 16) {
			out[{0, i}] << i;
			out.finish();
		}
		EndPaint(hwnd, &ps);
	case WM_DESTROY:
		return WM_destroy();
	}

}

int MAIN() {
	Window w;
	test testProcs;
	w.AttachWndProc(testProcs());
	
	w.ChangeWinStyle(WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL);
	w.Createwindow(L"Test", { 0,0 }, { 400,400 });
	return w.Run();
}