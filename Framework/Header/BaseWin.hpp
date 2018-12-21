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
        BaseWin() {
            hwnd = NULL;
        }
		void Init(HWND t) {
			hwnd = t;
		}
		//TODO function like this
		//BaseWin
		
		//TODO replace this function with simple wrapper around HWND
      
		//TODO should it be protected?????
//	protected:
        operator HWND() {
            return hwnd;
        }


    private:
        HWND hwnd;
    };


	class Win :public BaseWin {

	};
}