#include"Helpers.h"

#include<Windows.h>
#include<memory>

namespace WINAPIPP {

	enum GDIObjectType {
		pen,
		brush
	};

	class GDIObject {
	public:
		GDIObject(HGDIOBJ Obj) {
			Object = Obj;
		}

		virtual ~GDIObject() {
			DeleteObject(Object);
		}

		operator HGDIOBJ() {
			return Object;
		}

	private:
		GDIObject(GDIObject& t) {}

	protected:
		HGDIOBJ Object;
	};

	class Pen {
	public:
		Pen( int iStyle, int cWidth, COLORREF color)
		{
			HPEN temp = CreatePen(iStyle, cWidth, color);
			pen = std::make_shared<GDIObject>(temp);
		}

		operator std::shared_ptr<GDIObject>() {
			return pen;
		}

	private:
		std::shared_ptr<GDIObject>pen;

		int Style;
		int Width;
		COLORREF color;
	};

	class Brush {
	public:

		Brush(HBRUSH _brush) {
			brush = _brush;
		}

		~Brush() {
			DeleteObject(brush);
		}
	private:
		HBRUSH brush;
	};


	//TODO make the wrappers for all the DC operations
	class DC {


	public:
		DC(HWND _hwnd)
			:hwnd(_hwnd) {}

		inline BOOL CancelDC() {
			return ::CancelDC(hdc);
		}

		/*			Wrappers				*/

		void TextOut(WINAPIPP::Point point, std::wstring string) {
			::TextOut(hdc, point.x, point.y, string.c_str(), string.size());
		}

		/*			Wrappers				*/

		virtual void Attach(GDIObject Object) {
			SelectObject(hdc, Object);
		}

		void AttachStockObject(int Object) {
			SelectObject(hdc, GetStockObject(Object));
		}

		operator HDC() {
			return hdc;
		}

		virtual ~DC() {
			if (hwnd) {
				::ReleaseDC(hwnd, hdc);
			}
		}

	protected:
		HWND hwnd;
		HDC hdc;
	};

	class QuickDC :public DC {
	public:
		QuickDC(HWND _hwnd) :DC(_hwnd) {
			hdc = GetDC(_hwnd);
		}
	};

	//Should be made in WM_PAINT message only
	class PaintDC :public DC {
	public:
		PaintDC(HWND _hwnd) :DC(_hwnd)
		{

			hwnd = _hwnd;
			hdc = BeginPaint(hwnd, &ps);

		}
		operator HDC() {
			return hdc;
		}

		~PaintDC() {
			if (hwnd) {
				EndPaint(hwnd, &ps);
			}
		}
	public:
		PAINTSTRUCT ps;
	};

	class SafeDC :public PaintDC {
	public:
		using PaintDC::PaintDC;

		void Attach(std::shared_ptr<GDIObject> Object, GDIObjectType Type) {
			Objects[Type] = Object;
			SelectObject(hdc, *Object);
		}

	private:
		void Attach(GDIObject Object)override {}

		std::shared_ptr<GDIObject> Objects[5];
	};

	//TODO Implement DrawText with this class
	class TextCursor {



	};
}