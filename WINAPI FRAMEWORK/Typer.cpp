#include"Headers\Print.h"
#include"Headers\Scroll.h"
#include"Headers\WindowClass.h"
#include"Headers\TEXT.H"

#define BUFFER(x,y) *(pBuffer + y*cxBuffer + x)

class typerProc :public WndProcs<typerProc> {
public:
	int WndProc();
	typerProc():WndProcs(this){}
private:
	int WM_create();
	int WM_paint();
	int WM_size();
	static DWORD dwCharSet;
	Text tm;
	Position Caret;
	Position Buffer;
}Proc;

int typerProc::WndProc() {
	switch (message) {
	case WM_INPUTLANGCHANGE:
		dwCharSet = wParam;
	case WM_CREATE:
		return WM_create();
	case WM_SIZE:
		return WM_size();
	case WM_SETFOCUS:
		CreateCaret(hwnd, NULL, tm.cxChar(),tm.cyChar());
		SetCaretPos(Caret.x*tm.cxChar(), Caret.y*tm.cyChar());
		ShowCaret(hwnd);
		return 0;
	case WM_KILLFOCUS:
		HideCaret(hwnd);
		DestroyCaret();
		return 0;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_HOME:
			Caret.x = 0;
			break;
		case VK_END:
			Caret.x = Buffer.x - 1;
			break;
		case VK_PRIOR:
			Caret.y = 0;
			break;
		case VK_NEXT:
			Caret.y = Buffer.y - 1;
			break;
		case VK_LEFT:
			Caret.x = max(Caret.x - 1, 0);
			break;
		case VK_RIGHT:
			Caret.x = min(Caret.x + 1, Buffer.x - 1);
			break;
		case VK_UP:
			Caret.y = min(Caret.y - 1, 0);
			break;
		case VK_DOWN:
			Caret.y = min(Caret.y + 1, Buffer.y - 1);
			break;
		}
	}
}


int MAIN() {
	Window window;
	window.AttachWndProc(Proc());
	window.Createwindow(L"Typer", Position{ 0,0 }, Size{ 500,500 });
	return window.Run();
}