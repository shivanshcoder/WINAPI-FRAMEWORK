#pragma once
#include"BaseWin.hpp"
namespace HIMANI {
	class StaticWindow :public PredefinedWindow {
	public:
		OVERRIDE_PREDEFINEDCLASS(static)

			StaticWindow(const std::wstring &Tittle, int Style, const Helpers::Rect &Size, const BaseWin &_Parent = BaseWin()) :PredefinedWindow(_Parent) {
			CreateWin(Tittle, Style, Size, NULL);
		}

		StaticWindow(const StaticWindow&) = delete;
		StaticWindow& operator=(const StaticWindow&) = delete;

	};
}