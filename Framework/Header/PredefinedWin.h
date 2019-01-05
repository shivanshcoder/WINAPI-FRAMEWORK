#pragma once
#include"HBaseWin.hpp"
namespace HIMANI {
	class HStaticWindow :public HPredefinedWindow {
	public:
		OVERRIDE_PREDEFINEDCLASS(static)

			HStaticWindow(const std::wstring &Tittle, int Style, const Helpers::HRect &Size, const HBaseWin &_Parent = HBaseWin()) :HPredefinedWindow(_Parent) {
			CreateWin(Tittle, Style, Size, NULL);
		}

		HStaticWindow(const HStaticWindow&) = delete;
		HStaticWindow& operator=(const HStaticWindow&) = delete;

	};

}