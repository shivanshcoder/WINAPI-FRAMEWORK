#pragma once
#include"BaseWin.hpp"

namespace WINAPIPP {


	class ScrollBar {

	public:
		ScrollBar(short int ScrollBarType)
			:Type(ScrollBarType) {
			Info.cbSize = sizeof(Info);
		}

		void SetRange(int Min, int Max) {
			Info.nMax = Max;
			Info.nMin = Min;
			Info.fMask |= SIF_RANGE;
		}

		void SetPage(int Page) {
			Info.nPage = Page;
			Info.fMask |= SIF_PAGE;
		}

		void SetInfo(BaseWin Win, bool Erase) {
			SetScrollInfo(Win, Type, &Info, Erase);
			Info.fMask = 0;
		}

		void GetInfo(BaseWin Win) {
			//TODO change the default SIF_ALL later on
			Info.fMask = SIF_ALL;
			GetScrollInfo(Win, Type, &Info);
			Info.fMask = 0;
		}
	//TODO later make it private
	//private:
		short int Type;
		SCROLLINFO Info;
	};
}