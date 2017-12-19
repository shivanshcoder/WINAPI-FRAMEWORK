#pragma once
#include"WindowClass.h"

namespace scroll {
	/*struct SCROLLINFOTEST {
		static UINT    cbSize;
		static UINT    fMask;
		static int     nMin;
		static int     nMax;
		static UINT    nPage;
		static int     nPos;
		static int     nTrackPos;
};*/

	class ScrollInfo :public SCROLLINFO {
		friend class ScrollBase;
	public:
		int SetInfo ( HWND hwnd, int nBar, BOOL fRedraw );
		BOOL GetInfo ( HWND hwnd, int fnBar );
	};

	int ScrollInfo::SetInfo ( HWND hwnd, int nBar, BOOL fRedraw ) {
		cbSize = sizeof ( SCROLLINFO );
		int temp = ::SetScrollInfo ( hwnd, nBar, this, fRedraw );
		fMask = 0;
		return temp;
	}
	BOOL ScrollInfo::GetInfo ( HWND hwnd, int fnBar ) {
		cbSize = sizeof ( SCROLLINFO );
		BOOL temp = ::GetScrollInfo ( hwnd, fnBar, this );
		fMask = 0;
		return temp;
	}

	class ScrollBase  {
	public:
		ScrollBase () {
			Info.cbSize = sizeof ( SCROLLINFO );
			Info.fMask = 0;
		}
		void Range ( int Max, int Min );
		void Page ( int Pages );
		void Position ( int Pos );

		virtual inline void LineLeft (){}
		virtual inline void LineRight (){}
		virtual inline void PageLeft (){}
		virtual inline void PageRight (){}
		virtual inline void ThumbPosition (){}

		virtual inline void Top (){}
		virtual inline void Bottom (){}
		virtual inline void LineUp (){}
		virtual inline void LineDown (){}
		virtual inline void PageUp (){}
		virtual inline void PageDown (){}
	protected:
		static ScrollInfo Info;
		int PreviousnPos;
	};

	void ScrollBase::Page ( int Pages ) {
		Info.nPage = Pages;
		Info.fMask |= SIF_PAGE;
	}
	void ScrollBase::Position ( int Pos ) {
		Info.nPos = Pos;
		Info.fMask |= SIF_POS;
	}
	void ScrollBase::Range ( int Min, int Max ) {
		Info.nMin = Min;
		Info.nMax = Max;
		Info.fMask |= SIF_RANGE;
	}


	class WndScroll :public ScrollBase {
	public:

		//Horizontal Scroll Functions
		
		virtual inline void LineLeft () { Info.nPos -= 1; }
		virtual inline void LineRight () { Info.nPos += 1; }
		virtual inline void PageLeft () { Info.nPage -= 1; }
		virtual inline void PageRight () { Info.nPage += 1; }
		virtual inline void ThumbPosition () { Info.nPos = Info.nTrackPos; }

		//Vertical Scroll Functions

		virtual inline void Top () { Info.nPos = Info.nMin; }
		virtual inline void Bottom () { Info.nPos = Info.nMax; }
		virtual inline void LineUp () { Info.nPos -= 1; }
		virtual inline void LineDown () { Info.nPos += 1; }
		virtual inline void PageUp () { Info.nPage -= 1; }
		virtual inline void PageDown () { Info.nPage += 1; }

		int CurrentPosV ( HWND hwnd );
		int CurrentPosH ( HWND hwnd );

		int SetScrollInfo ( HWND hwnd, int nBar, bool fRedraw ) { return Info.SetInfo ( hwnd, nBar, fRedraw ); }

		BOOL GetScrollInfo ( HWND hwnd, int fnBar ) { return Info.GetInfo ( hwnd, fnBar ); }

		int VScroll ( HWND hwnd, LPARAM lParam, WPARAM wParam, int amountSc = 16 );
		virtual int VScrollFunc () = 0;

		int HScroll ( HWND hwnd, LPARAM lParam, WPARAM wParam, int amountSc = 7 );
		virtual int HScrollFunc () = 0;

	};

	int WndScroll::CurrentPosV ( HWND hwnd ) {
		Info.fMask = SIF_POS;
		Info.GetInfo ( hwnd, SB_VERT );
		return Info.nPos;
	}
	int WndScroll::CurrentPosH ( HWND hwnd ) {
		Info.fMask = SIF_POS;
		Info.GetInfo ( hwnd, SB_HORZ );
		return Info.nPos;
	}

	int WndScroll::VScroll ( HWND hwnd, LPARAM lParam, WPARAM wParam, int amountSc ) {
		Info.fMask = SIF_ALL;
		GetScrollInfo ( hwnd, SB_VERT );
		PreviousnPos = Info.nPos;
		switch( LOWORD ( wParam ) ) {
		case SB_TOP:
			Top ();
			//nPos = nMin;

			break;
		case SB_BOTTOM:
			//nPos = nMax;
			Bottom ();
			break;
		case SB_LINEUP:
			//nPos -= 1;
			LineUp ();
			break;
		case SB_LINEDOWN:
			//nPos += 1;
			LineDown ();
			break;
		case SB_PAGEUP:
			PageUp ();
			//nPos -= nPage;
			break;
		case SB_PAGEDOWN:
			PageDown ();
			//nPos += nPage;
			break;
		case SB_THUMBTRACK:

			//nPos = nTrackPos;
			break;
		default:
			break;
		}
		return VScrollFunc ();
		//	fMask = SIF_ALL;
		//	SetScrollInfo(hwnd, SB_VERT, TRUE);
		//	fMask = SIF_ALL;
		//	GetScrollInfo(hwnd, SB_VERT);
		//
		//	if (nPos != iVertPos) {
		//#ifdef EXSP
		//		ScrollWindowEx(hwnd, 0, amountSc * (iVertPos - nPos), NULL, NULL, NULL, NULL, SW_INVALIDATE);
		//#else
		//		ScrollWindow(hwnd, 0, amountSc * (iVertPos - nPos), NULL, NULL);
		//#endif // EXSP
		//
		//		UpdateWindow(hwnd);
		//	}
		//	return 0;

	}
	int WndScroll::HScroll ( HWND hwnd, LPARAM lParam, WPARAM wParam, int amountSc ) {
		Info.fMask = SIF_ALL;
		GetScrollInfo ( hwnd, SB_HORZ );
		PreviousnPos = Info.nPos;
		switch( LOWORD ( wParam ) ) {
		case SB_LINELEFT:
			//	nPos -= 1;
			LineLeft ();
			break;
		case SB_LINERIGHT:
			//	nPos += 1;
			LineRight ();
			break;
		case SB_PAGELEFT:
			//	nPos -= nPage;
			PageLeft ();
			break;
		case SB_PAGERIGHT:
			//	nPos += nPage;
			PageRight ();
			break;
		case SB_THUMBPOSITION:
			ThumbPosition ();
			//	nPos  = nTrackPos;
			break;
		default:
			break;
		}
		return HScrollFunc ();
		//	fMask = SIF_ALL;
		//	SetScrollInfo(hwnd, SB_HORZ, TRUE);
		//	fMask = SIF_ALL;
		//	GetScrollInfo(hwnd, SB_HORZ);
		//
		//	if (nPos != iHorzPos) {
		//#ifdef EXSP
		//		ScrollWindowEx(hwnd, amountSc * (iHorzPos - nPos), 0, NULL, NULL, NULL, NULL, SW_INVALIDATE);
		//#else
		//		ScrollWindow(hwnd, amountSc * (iHorzPos - nPos), 0, NULL, NULL);
		//#endif // EXSP
		//
		//		UpdateWindow(hwnd);
		//	}
		//	return 0;

	}

	class ScrollWndCTL :public WindowBase, public ScrollBase {
	public:
		ScrollWndCTL ( LPCWSTR ClassName ) :WindowBase ( ClassName ) {
			
		}
		ScrollWndCTL () {}

		int SetScrollInfo ( bool fDraw ) { return Info.SetInfo ( hwnd, SB_CTL, fDraw ); }
		BOOL GetScrollInfo () { return Info.GetInfo ( hwnd, SB_CTL ); }
	};
}