#include<Windows.h>

namespace shapes {
	class Rectangle {
	public:
		Rectangle(){}
		Rectangle ( RECT _rect ) { rect = _rect; }
		void ScaleDown ( int percent );
		void ScaleUp ( int percent );
		void GetWindowRect ( HWND hwnd );

		void Fill ( HDC hdc, HBRUSH hBrush );
		void Frame ( HDC hdc, HBRUSH hBrush );

		const int left () { return rect.left; }
		const int right () { return rect.right; }
		const int top () { return rect.top; }
		const int bottom () { return rect.bottom; }
		const int WidthX () { return rect.right - rect.left; }
		const int HeghtY () { return rect.top - rect.bottom; }
		
	private:
		RECT rect;
	};

	void Rectangle::ScaleDown ( int percent ) {
		RECT temp = rect;
		rect.left += ( (rect.right-rect.left) / percent );
		rect.right -= ( ( rect.right - rect.left ) / percent );
		rect.top += ( (rect.bottom - rect.top) / percent );
		rect.bottom -= ( ( rect.bottom - rect.top ) / percent );
	}

	void Rectangle::ScaleUp ( int percent ) {
		rect.left -= ( ( rect.right - rect.left ) / percent );
		rect.right += ( ( rect.right - rect.left ) / percent );
		rect.top -= ( (rect.bottom - rect.top ) / percent );
		rect.bottom += ( (rect.bottom - rect.top ) / percent );
	}

	void Rectangle::GetWindowRect ( HWND hwnd ) {
		::GetWindowRect ( hwnd, &rect );
	}

	inline void Rectangle::Fill ( HDC hdc, HBRUSH hBrush ) {
		FillRect ( hdc, &rect, hBrush );
	}
	inline void Rectangle::Frame ( HDC hdc, HBRUSH hBrush ) {
		FrameRect ( hdc, &rect, hBrush );
	}
}