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
#define H_WM_GETOWNINSTANCE WM_USER+1
#define H_WM_SWAPPROCADDR WM_USER+2

#define H_WM_ACCELERATOR WM_USER+100
#define H_WM_DIALOGPROCESSING WM_USER+101

namespace Himani {

	template<class OwnerWindow>
	LRESULT CALLBACK CommonWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

		
		if (message == WM_NCCREATE) {
			auto createArguments = (LPCREATESTRUCT)lParam;
			if (createArguments->lpCreateParams != (LPVOID)-1) {
				HClassInitializer* Args = nullptr;
				OwnerWindow* ClassInstance;

				if(createArguments->lpCreateParams == NULL)
					ClassInstance = new OwnerWindow(HClassInitializer(hwnd,true));
				else {
					//TODO
					//Use Dynamic Cast or something like that maybe?
					Args = (HClassInitializer*)createArguments->lpCreateParams;
					Args->hwnd = hwnd;
					Args->SelfDestruct = true;
					ClassInstance = new OwnerWindow(*Args);
				}
				SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)ClassInstance->Procedure());
				
			}
			
			return TRUE;
		}

		if (message == H_WM_SWAPPROCADDR) {

			SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)lParam);
		}

		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	
}