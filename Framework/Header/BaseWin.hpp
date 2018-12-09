#pragma once
#include"Core.h"
#include"Helpers.h"

namespace WINAPIPP {

    class BaseWin {
    public:
        BaseWin() {
            hwnd = NULL;
          //  valid = false;
        }

        bool CreateWin(std::wstring ClassName, std::wstring Tittle, DWORD style, WINAPIPP::Rectangle size, BaseWin Parent) {
            hwnd = CreateWindow(ClassName.c_str(), Tittle.c_str(), style,
				size.left, size.top, size.right, size.bottom,
                Parent, NULL, Instance(), NULL);
			return hwnd;
        }
        
        operator HWND() {
            return hwnd;
        }

    private:
        //BaseWin(BaseWin& w);

    private:
    //    bool valid;
        HWND hwnd;
    };
}