#include<Windows.h>

int WINAPI WinMain (HINSTANCE Instance, HINSTANCE PrevInstance, PSTR CmdLine, int CmdShow) {
	MessageBox (0, TEXT ("Hello"), TEXT ("Hello"), MB_OK);
	return 0;
}