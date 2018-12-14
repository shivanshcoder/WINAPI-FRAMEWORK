#pragma once
#include"Core.h"
#include"Helpers.h"

namespace WINAPIPP {


	/*
	Cannot be copied or 
	*/
    class BaseWin {
    public:
        BaseWin() {
            hwnd = NULL;
          //  valid = false;
        }

        bool CreateWin(std::wstring ClassName, std::wstring Tittle, DWORD style, WINAPIPP::Rectangle size, BaseWin const &Parent) {
            hwnd = CreateWindow(ClassName.c_str(), Tittle.c_str(), style,
				size.left, size.top, size.right, size.bottom,
                Parent.hwnd, NULL, Instance(), NULL);
			return hwnd;
        }
        
		//TODO should it be protected?????
//	protected:
        operator HWND() {
            return hwnd;
        }

		//TODO should it be allowed to copy or assigned or not???
		//BaseWin(BaseWin&) = delete;
		//BaseWin& operator=(BaseWin const&) = delete;

    private:
        //BaseWin(BaseWin& w);

    private:
    //    bool valid;
        HWND hwnd;
    };


	class Win :public BaseWin {

	};
}