#pragma once
#include"Core.h"

//TODO fix the HWND initialization for WM_CREATE
//NOTE Dont override WM_CREATE instead use the constructor space ofr doing stuff

//Declares Message map for that class
#define DECLARE_MESSAGE_MAP() LRESULT MessageFunc(UINT message, WPARAM wParam, LPARAM lParam)override


#define MESSAGE_MAP_END(ParentWinClass)	}				\
return ParentWinClass::MessageFunc( message, wParam, lParam); }

#define MESSAGE_MAP_BEGIN(CLASS_NAME)	LRESULT CLASS_NAME::MessageFunc(UINT message, WPARAM wParam, LPARAM lParam) {  \
		switch(message){						

#define MESSAGE_MAP_ENTRY_PARAMS(FUNC, MESSAGE_VAL) case MESSAGE_VAL: return FUNC(wParam, lParam);

#define MESSAGE_MAP_ENTRY(FUNC, MESSAGE_VAL) case MESSAGE_VAL: return FUNC();

#define MESSAGE_MAP_ENTRY_SINGLE(STATEMENT, RETURN_VAL, MESSAGE_VAL) case MESSAGE_VAL: STATEMENT; return RETURN_VAL;


//Sends to Window using raw Handle and returns the Class Instance associated with that handle
#define H_WM_GETOWNINSTANCE WM_USER + 1
#define H_WM_SWAPPROCADDR WM_USER + 2
#define H_WM_GETCHILDID WM_USER + 3

#define H_WM_ACCELERATOR WM_USER + 100
#define H_WM_DIALOGPROCESSING WM_USER + 101
#define H_CM_PROCESSNOTIF WM_USER + 102
