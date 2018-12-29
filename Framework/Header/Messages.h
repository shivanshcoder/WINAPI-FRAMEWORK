#pragma once
#include<Windows.h>
#include<iostream>

//TODO fix the HWND initialization for WM_CREATE
//NOTE Dont override WM_CREATE instead use the constructor space ofr doing stuff

//Declares Message map for that class
#define DECLARE_MESSAGE_MAP() LRESULT MessageFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)override


#define MESSAGE_MAP_END(ParentWinClass)	}				\
return ParentWinClass::MessageFunc(hwnd, message, wParam, lParam); }

#define MESSAGE_MAP_BEGIN(CLASS_NAME)	LRESULT CLASS_NAME::MessageFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {  \
		switch(message){						
	
#define MESSAGE_MAP_ENTRY_PARAMS(FUNC, MESSAGE_VAL) case MESSAGE_VAL: return FUNC(wParam, lParam);

#define MESSAGE_MAP_ENTRY(FUNC, MESSAGE_VAL) case MESSAGE_VAL: return FUNC();

#define MESSAGE_MAP_ENTRY_SINGLE(STATEMENT, RETURN_VAL, MESSAGE_VAL) case MESSAGE_VAL: STATEMENT; return RETURN_VAL;


//
//#define MESSAGE_MAP_KEYBOARD(KEYBOARD) \
//		case WM_ACTIVATE	:\
//		case WM_APPCOMMAND	:\
//		case WM_CHAR		:\
//		case WM_DEADCHAR	:\
//		case WM_HOTKEY		:\
//		case WM_KEYDOWN		:\
//		case WM_KEYUP		:\
//		case WM_KILLFOCUS	:\
//		case WM_SETFOCUS	:\
//		case WM_SYSDEADCHAR	:\
//		case WM_SYSKEYDOWN	:\
//		case WM_SYSKEYUP	:\
//		case WM_UNICHAR		: return KEYBOARD.Process(message, wParam,lParam);





