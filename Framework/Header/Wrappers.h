#pragma once
#include"Hpch.h"
#include"HBaseWin.h"

namespace HIMANI{


	//TODO make ScrollWIndowClass


	//TODO check this window with SB_CTL type
	class HScrollBar {
	public:
		HScrollBar(short int ScrollBarType, HBaseWin *Win)
			:Type(ScrollBarType) {
			Info.cbSize = sizeof(Info);
			Owner = Win;
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

		//Send the LOWORD(wParam) of the Message Procedure
		void SetPos(int Pos) {
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

		void SetInfo(bool Erase) {
			SetScrollInfo(*Owner, Type, &Info, Erase);
			Info.fMask = 0;
		}

		void GetInfo() {
			//TODO change the default SIF_ALL later on
			Info.fMask = SIF_ALL;
			GetScrollInfo(*Owner, Type, &Info);
			Info.fMask = 0;
		}

		int GetCurrentPos() {
			GetInfo();
			return Info.nPos;
		}

		//TODO make it more generic for other scroll bars too
		int CheckChange(int NewPos) {
			GetInfo();
			int iVertPos = Info.nPos;

			SetPos(NewPos);

			SetInfo(true);
			GetInfo();

			return iVertPos - Info.nPos;

		}

		enum Position {

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


	private:
		short int Type;
		SCROLLINFO Info;
		HBaseWin *Owner;
	};

}