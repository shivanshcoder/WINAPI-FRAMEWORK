#include"Helpers.h"

#include<Windows.h>
#include<memory>

namespace WINAPIPP {

	enum GDIObjectType {
		pen,
		brush,
		base
	};


	class GDIObject {
		friend class BaseGDIObject;
	public:
		~GDIObject() {
			if(Object)
				DeleteObject(Object);
		}
	
		GDIObject(HGDIOBJ Obj) {
			Object = Obj;
		}

	protected:

		GDIObject(GDIObject& t) {}

		HGDIOBJ Object;

	
	};

	class BaseGDIObject {
		friend class DC;
	public:

		HGDIOBJ operator *() {
			return Object->Object;
		}

		virtual GDIObjectType Type() { return base; }// = 0;

		operator std::shared_ptr<GDIObject>() {
			return Object;
		}

	protected:
		std::shared_ptr<GDIObject>Object;
	};

	class Pen :public BaseGDIObject{
	public:
		Pen( int iStyle, int cWidth, COLORREF color)
		{
			HPEN temp = CreatePen(iStyle, cWidth, color);
			Object = std::make_shared<GDIObject>(temp);
		}

		GDIObjectType Type() {
			return GDIObjectType::pen;
		}

	private:
		int Style;
		int Width;
		COLORREF color;
	};

	class Brush:public BaseGDIObject {
	public:

	private:

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

		virtual void Attach(BaseGDIObject &Object) {
			SelectObject(hdc, *Object);
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


	class SafeDC :public DC {
	public:
		using DC::DC;

		void Attach(BaseGDIObject &Object) {
			int tem = Object.Type();
			Objects[Object.Type()] = Object;
			SelectObject(hdc, (*Object));
		}
		/*
		void Attach( const BaseGDIObject &Object) {
		
			auto temp = std::make_shared<BaseGDIObject>(Object);
			Objects[temp->Type()] = tek
			SelectObject(hdc, (*Object));
		}*/

	private:
		//void Attach(BaseGDIObject &Object) {}
		std::shared_ptr<GDIObject> Objects[5];
	//	BaseGDIObject Objects[5];
	};

	class QuickDC :public DC {
	public:
		QuickDC(HWND _hwnd) :DC(_hwnd) {
			hdc = GetDC(_hwnd);
		}
	};

	//Should be made in WM_PAINT message only
	class PaintDC :public SafeDC {
	public:
		PaintDC(HWND _hwnd) :SafeDC(_hwnd)
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


	//TODO Implement DrawText with this class
	class TextCursor {



	};
}