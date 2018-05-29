#include<Windows.h>

int WINAPI WinMain (HINSTANCE Instance, HINSTANCE PrevInstance, PSTR CmdLine, int CmdShow) {
	WNDCLASS Window = {};

	Window.hInstance = Instance;
	Window.lpfnWndProc = DefWindowProc;
	Window.style = WS_OVERLAPPEDWINDOW;
	Window.lpszClassName = TEXT("	");
	return 0;
}