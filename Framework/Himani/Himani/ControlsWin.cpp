#include "Hpch.h"
#include "ControlsWin.h"

namespace Himani {
	//MESSAGE_MAP_BEGIN(HControls)
	//	MESSAGE_MAP_ENTRY_PARAMS(CTL_Color, WM_CTLCOLORMSGBOX);
	//MESSAGE_MAP_ENTRY_PARAMS(CTL_Color, WM_CTLCOLOREDIT);
	//MESSAGE_MAP_ENTRY_PARAMS(CTL_Color, WM_CTLCOLORLISTBOX);
	//MESSAGE_MAP_ENTRY_PARAMS(CTL_Color, WM_CTLCOLORBTN);
	//MESSAGE_MAP_ENTRY_PARAMS(CTL_Color, WM_CTLCOLORDLG);
	//MESSAGE_MAP_ENTRY_PARAMS(CTL_Color, WM_CTLCOLORSCROLLBAR);
	//MESSAGE_MAP_ENTRY_PARAMS(CTL_Color, WM_CTLCOLORSTATIC);
	//MESSAGE_MAP_END(HPredefinedWindow);


	//MESSAGE_MAP_BEGIN(HScrollBar)
	//	MESSAGE_MAP_ENTRY_PARAMS(Scroll, WM_VSCROLL)
	//	MESSAGE_MAP_ENTRY_PARAMS(Scroll, WM_HSCROLL)

	//MESSAGE_MAP_END(HControls);


	//INT_PTR HScrollBar::CTL_Color(WPARAM wParam, LPARAM lParam) {

	//	return (INT_PTR)brush.RetrieveObject();
	//}

	//void HScrollBar::SetInfo(int Min, int Max, int Page, int Pos, bool Redraw) {
	//	SCROLLINFO sInfo;

	//	sInfo.cbSize = sizeof(SCROLLINFO);
	//	sInfo.nMin = Min;
	//	sInfo.nMax = Max;
	//	if (Page == 0)
	//		sInfo.nPage = (Max - Min) / 20;

	//	sInfo.nPos = Pos;
	//	sInfo.fMask = SIF_POS | SIF_RANGE | SIF_RANGE | SIF_PAGE;

	//	SetScrollInfo(&sInfo, SB_CTL, Redraw);
	//}

	//int HScrollBar::Scroll(WPARAM wParam, LPARAM lParam) {
	//	SCROLLINFO info;
	//	info.cbSize = sizeof(SCROLLINFO);
	//	info.fMask = SIF_ALL;
	//	GetScrollInfo(&info, SB_CTL);
	//	//	int oldPos = info.nPos;
	//	switch (LOWORD(wParam)) {
	//	case SB_LINEUP:
	//		//case 	SB_LINELEFT:
	//		info.nPos -= 1;
	//		break;
	//	case SB_LINEDOWN:
	//		//case 	SB_LINERIGHT:
	//		info.nPos += 1;
	//		break;
	//	case SB_BOTTOM:
	//		//	case 	SB_RIGHT:
	//		info.nPos = info.nMax;
	//		break;
	//	case SB_TOP:
	//		//	case 	SB_LEFT:
	//		info.nPos = info.nMin;
	//		break;
	//	case SB_PAGEUP:
	//		//case 	SB_PAGELEFT:
	//		info.nPos -= info.nPage;
	//		break;
	//	case SB_PAGEDOWN:
	//		//case 	SB_PAGERIGHT:
	//		info.nPos += info.nPage;
	//		break;

	//	case SB_THUMBPOSITION:
	//	case SB_THUMBTRACK:
	//		info.nPos = info.nTrackPos;
	//		break;
	//	}

	//	SetScrollInfo(&info, SB_CTL, true);
	//	GetScrollInfo(&info, SB_CTL);


	//	ScrollCallbk(info.nPos);
	//	return 0;
	//}
	//MESSAGE_MAP_BEGIN(HStaticWindow)

	//	MESSAGE_MAP_ENTRY_PARAMS(CTL_Color, WM_CTLCOLORSTATIC);
	//MESSAGE_MAP_END(HPredefinedWindow);

	LRESULT HButton::MessageFunc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		return HCustomWindow::MessageFunc(message, wParam, lParam);
	}

}