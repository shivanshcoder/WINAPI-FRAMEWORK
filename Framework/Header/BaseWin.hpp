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
	class WrapperWin :public BaseWin {
	public:
		Helpers::Rect GetClientRect() {
			Helpers::Rect rect;
			if (!::GetClientRect(*this, &rect.rect)) {
				throw std::exception("Rect error");
			}
			return rect;
		}

		Helpers::Rect GetWinRect() {
			Helpers::Rect rect;
			if (!::GetWindowRect(*this, &rect.rect)) {
				throw std::exception("Rect error");
			}
			return rect;
		}

		void InvalidateRect(Helpers::Rect rect, bool redraw) {
			if (!::InvalidateRect(*this, &rect.rect, redraw))
				throw std::exception("Error");

		}
	};
}