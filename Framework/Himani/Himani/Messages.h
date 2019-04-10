#pragma once
#include"Core.h"

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

//Automatically destroy the Instance of the stored class
#define WM_DESTROYINSTANCE WM_USER+1
#define WM_SAVEINSTANCE WM_USER+2

namespace Himani {

	//Every class is registered with this class first
	//TODO should it be static function for proper functioning???
	static LRESULT CALLBACK CommonWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

		//TODO this expects lpCreateParams directly the WNDPROC type pointer
		if (message == WM_NCCREATE) {
			auto wndProc = ((LPCREATESTRUCT)lParam)->lpCreateParams;
			//this is NULL if not created by FrameWork
			if (wndProc) {
				//Replaces the Callback Procedure with the Thunk we have supplied with the CREATESTRUCT param
				SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)(wndProc));
				//return TRUE;
			}
			//Null when created by system 
			else{

			}
		}

		if (message == WM_DESTROYINSTANCE) {
			if (wParam) {
				/*
					This Message deletes the Class Instance associated with particular Window
				*/

				//URGENT well it should work base most class
				auto instance = (Himani::HHandleWrapperBaseClass<HWND>*)wParam;
				delete instance;
			}
		}

		return DefWindowProc(hwnd, message, wParam, lParam);
	}
}