#pragma once
#include"Core.h"
#include"Helpers.h"

namespace WINAPIPP {


	/*
	Cannot be copied or 
	*/
    class BaseWin {
		friend class Window;

    public:
        /*BaseWin() {
            hwnd = NULL;
        }
*/
		//TODO function like this
		//BaseWin
		
		//TODO replace this function with simple wrapper around HWND
        virtual bool CreateWin( std::wstring Tittle, DWORD style, Helpers::CPPRectangle size,BaseWin *Parent) {
			
			//URGENT remove such ClassName
			std::wstring ClassName = L"Application";
            //hwnd = CreateWindow(ClassName.c_str(), Tittle.c_str(), style,
			//	size.left, size.top, size.right, size.bottom,
            //  *Parent, NULL, Instance(), NULL);
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