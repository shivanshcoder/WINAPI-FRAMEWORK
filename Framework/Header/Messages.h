#pragma once
#include<Windows.h>
#include<iostream>

#define DECLARE_MESSAGE_MAP() LRESULT MessageFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)final

#define MESSAGE_MAP_END()	}						\
return DefWindowProc(hwnd, message, wParam, lParam); }

#define MESSAGE_MAP_BEGIN(CLASS_NAME)	LRESULT CLASS_NAME::MessageFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {  \
		switch(message){															
#define MESSAGE_MAP_ENTRY_PARAMS(FUNC, MESSAGE_VAL) case MESSAGE_VAL: return FUNC(wParam, lParam);

#define MESSAGE_MAP_ENTRY(FUNC, MESSAGE_VAL) case MESSAGE_VAL: return FUNC();

struct Params {
	WPARAM wParam;
	LPARAM lParam;
	int Type;
};

#pragma region PARAM_UNPACK
#define WM_SIZE_ONE int
#define WM_SIZE_TWO int
#define WM_CREATE_ONE int
#define WM_CREATE_TWO LPCREATESTRUCT

#define UNPACK_PARAMS(Param, MESSAGE) std::pair<MESSAGE##_ONE, MESSAGE##_TWO>((MESSAGE##_ONE)Param.wParam,(MESSAGE##_TWO)Param.lParam )


#pragma endregion

void fn() {
	Params t;
	auto tt = UNPACK_PARAMS(t, WM_CREATE);
}