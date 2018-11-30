#pragma once
#include"BaseWin.hpp"

namespace WINAPIPP {


	class ScrollBar {

		enum Position{

			//Corresponds to SB_LINELEFT and SB_LINEUP
			LineLow,

			//Corresponds to SB_LINERIGHT and SB_LINEDOWN
			LineHigh,

			//Corresponds to SB_PAGELEFT and SB_PAGEUP
			PageLow,

			//Corresponds to SB_PAGEDOWN and SB_PAGERIGHT
			PageHigh,

			//Corresponds to SB_THUMBPOSITION
			ThumbPos,

			//Corresponds to SB_THUMBTRACK
			ThumbTrack,
			
			//Corresponds to SB_TOP and SB_LEFT
			Min,

			//Corresponds to SB_BOTTOM and SB_RIGHT
			Max,
		};

	public:
		ScrollBar(short int ScrollBarType, BaseWin *Win)
			:Type(ScrollBarType) {
			Info.cbSize = sizeof(Info);
			Parent = *Win;
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

		void SetPos(int Pos) {
			//Info.nPos = Pos;
			switch (Pos) {

			case Position::Min:
				Info.nPos = Info.nMin;
				break;

			case Position::Max:
				Info.nPos = Info.nMax;
				break;

			case Position::PageHigh:
				Info.nPos += Info.nPage;
				break;

			case Position::PageLow:
				Info.nPos -= Info.nPage;
				break;

			case Position::ThumbTrack:
				Info.nPos = Info.nTrackPos;
				break;

			case Position::LineLow:
				Info.nPos -= 1;
				break;

			case Position::LineHigh:
				Info.nPos += 1;
				break;

			}
			Info.fMask |= SIF_POS;
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

		//TODO make it more generic for other scroll bars too
		int CheckChange(BaseWin Win, int NewPos) {
			GetInfo(Win);
			int iVertPos = Info.nPos;

			SetPos(NewPos);

			SetInfo(Win, true);
			GetInfo(Win);

			return iVertPos - Info.nPos;

		}

	//TODO later make it private
	//private:
		short int Type;
		SCROLLINFO Info;
		BaseWin Parent;
	};
}