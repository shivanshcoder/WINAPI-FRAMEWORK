#pragma once
#include"Helpers.h"
#include<string>
#include<Windows.h>
namespace WINAPIPP {
	extern HINSTANCE Instance;

    class BaseWin {
    public:
        BaseWin() {
            hwnd = NULL;
            valid = false;
        }

        bool Create(std::wstring ClassName, std::wstring Tittle, DWORD style, WINAPIPP::Rectangle size, BaseWin Parent) {
            hwnd = CreateWindow(ClassName.c_str(), Tittle.c_str(), style,
				//CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
				size.x, size.y, size.width, size.height,
                Parent, NULL, Instance, NULL);
			return hwnd;
        }
        
        operator HWND() {
            return hwnd;
        }

    private:
        //BaseWin(BaseWin& w);

    private:
        bool valid;
        HWND hwnd;
    };
}