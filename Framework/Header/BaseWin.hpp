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

		//TODO should it be protected?????
//	protected:
        operator HWND() {
            return hwnd;
        }


    private:
        HWND hwnd;
    };

	//TODO make HWND functions wrapper class
	class Win :public BaseWin {

	};
}