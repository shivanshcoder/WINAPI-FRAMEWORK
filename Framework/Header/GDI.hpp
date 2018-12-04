#include"Helpers.h"

#include<Windows.h>
#include<memory>

namespace WINAPIPP {

	enum GDIObjectType {
		pen,
		brush,
		base
	};


	/*
		BaseObject class for auto deletion of object
	*/
	class BaseObject {
		friend class BaseGDIObject;
	public:
		~BaseObject() {
			if (Object)
				DeleteObject(Object);
		}

		BaseObject(HGDIOBJ Obj) {
			Object = Obj;
		}

	protected:

		BaseObject(BaseObject& t) {}

		HGDIOBJ Object;
	};

	//TODO: Same GDIObject can be selected for multiple DC, need to verify if that is okay
	//Wrapper for hiding std::shared_ptr and base class for GDIObjects
	class BaseGDIObject {
		friend class DC;
		friend class SafeDC;

	protected:
		//For Getting underlying Handle
		HGDIOBJ operator *() {
			return Object->Object;
		}

		//Returns Type of Object
		virtual GDIObjectType Type()const { return base; }// = 0;

		operator std::shared_ptr<BaseObject>() {
			return Object;
		}

		std::shared_ptr<BaseObject>Object;
	};

	class Pen :public BaseGDIObject {
	public:
		Pen(int iStyle, int cWidth, COLORREF color)
		{
			HPEN temp = CreatePen(iStyle, cWidth, color);
			Object = std::make_shared<BaseObject>(temp);
		}

		GDIObjectType Type()const override {
			return GDIObjectType::pen;
		}

	private:
		Pen(Pen&);

		int Style;
		int Width;
		COLORREF color;
	};

	class Brush :public BaseGDIObject {
	public:

	private:
		Brush(Brush&);
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

	/*
		This DC implementation saves the Objects currently selected for the respective DCs
		This uses the std::shared_ptr for making it safe
		In case the the BaseObject gets out of scope, this prevents it from getting deleted
		and automatically deletes the objects with it's own destructor
	*/
	class SafeDC :public DC {
	public:
		using DC::DC;

		//For attaching the object which already has a scope from calling place
		void Attach(BaseGDIObject &Object)override {
			//TODO better checking
			//Strange
			if (Object.Type() == base)
				__debugbreak();
			Objects[Object.Type()] = Object;
			SelectObject(hdc, (*Object));
		}

		//For on the go constructor type Object
		void Attach(const BaseGDIObject &Object) {
			//TODO better checking
			//Strange
			if (Object.Type() == base)
				__debugbreak();
			Objects[Object.Type()] = BaseGDIObject(Object);
			SelectObject(hdc, *Objects[Object.Type()]);
		}

	private:
		//Saving reference of the selected objects
		BaseGDIObject Objects[5];
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