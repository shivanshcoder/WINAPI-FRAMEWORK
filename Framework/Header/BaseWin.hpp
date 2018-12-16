#pragma once
#include"Core.h"
#include"Helpers.h"

namespace WINAPIPP {


	/*
	Cannot be copied or 
	*/
    class BaseWin {
		friend class CUSTOM_CLASS;

    public:
        /*BaseWin() {
            hwnd = NULL;
        }
*/
		//TODO function like this
		//BaseWin
		
		//TODO replace this function with simple wrapper around HWND
        virtual bool CreateWin( std::wstring Tittle, DWORD style, WINAPIPP::Rectangle size, BaseWin const &Parent) {
			//URGENT remove such ClassName
			std::wstring ClassName = L"MainWindow";
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


    private:

    private:
        HWND hwnd;
    };


	class Win :public BaseWin {

	};
}