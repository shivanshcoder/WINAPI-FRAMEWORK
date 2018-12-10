#include"Helpers.h"
#include"BaseWin.hpp"
#include<memory>
#include<chrono>

namespace WINAPIPP {

	enum GDIObjectType {
		pen,
		brush,
		region,
		base
	};

	//	BaseObject class for auto deletion of object

	class BaseObject {
		//friend class BaseGDIObject;

	public:
		~BaseObject() {
			if (Object)
				DeleteObject(Object);
		}

		operator HGDIOBJ() {
			return Object;
		}

		HGDIOBJ Object;

	protected:
		BaseObject(BaseObject& t) {}

	};

	//TODO: Same GDIObject can be selected for multiple DC, need to verify if that is okay
	//It is okay to have same logical pen for different DCs
	//Wrapper for hiding std::shared_ptr and base class for GDIObjects
	class BaseGDIObject {
		friend class DC;
		friend class SafeDC;

	protected:

		//Getting Underlying Handle to Object
		HGDIOBJ RetrieveObject() {
			return GDIObject->Object;
		}

		void Init(HGDIOBJ Obj) {
			GDIObject = std::make_shared<BaseObject>(Obj);
		}

		//Returns Type of Object
		virtual GDIObjectType Type()const { return base; }// = 0;

		operator std::shared_ptr<BaseObject>() {
			return GDIObject;
		}

		std::shared_ptr<BaseObject>GDIObject;
	};

#pragma region DCs


	//TODO make the wrappers for all the DC operations
	class DC {


	public:
		DC(HWND _hwnd)
			:hwnd(_hwnd) {}

		/*			Wrappers				*/

		//TODO change return values acording to standard functions
		BOOL CancelDC() {
			return ::CancelDC(hdc);
		}
		void SaveDC() {
			::SaveDC(hdc);
		}
		void RestoreDC() {
			::RestoreDC(hdc, -1);
		}

		void TextOut(WINAPIPP::Point point, std::wstring string) {
			::TextOut(hdc, point.x, point.y, string.c_str(), string.size());
		}




		/*			Wrappers				*/

		virtual void Attach(BaseGDIObject &Object) {
			SelectObject(hdc, Object.RetrieveObject());
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
			SelectObject(hdc, (Object.RetrieveObject()));
		}

		//For on the go constructor type Object
		void Attach(const BaseGDIObject &Object) {
			//TODO better checking
			//Strange
			if (Object.Type() == base)
				__debugbreak();
			Objects[Object.Type()] = BaseGDIObject(Object);
			SelectObject(hdc, Objects[Object.Type()].RetrieveObject());
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

	private:
		QuickDC(QuickDC&);
	};

	//Should be made in WM_PAINT message only
	class PaintDC :public SafeDC {
	public:
		PaintDC(HWND _hwnd) :SafeDC(_hwnd)
		{
			hwnd = _hwnd;
			hdc = BeginPaint(hwnd, &__ps);
		}

		operator HDC() {
			return hdc;
		}

		~PaintDC() {
			if (hwnd) {
				EndPaint(hwnd, &__ps);
			}
		}
		PAINTSTRUCT ps() const {
			return __ps;
		}

		//TODO make this private
	private:
		PaintDC(PaintDC&);

		PAINTSTRUCT __ps;
	};


#pragma endregion




	class Pen :public BaseGDIObject {
	public:
		Pen(int iStyle, int cWidth, COLORREF color)
		{
			Init(CreatePen(iStyle, cWidth, color));
			//Object = std::make_shared<BaseObject>(temp);
		}

		GDIObjectType Type()const override {
			return GDIObjectType::pen;
		}

		operator HPEN() {
			return (HPEN)RetrieveObject();
		}
	private:
		Pen(Pen&);

		//TODO store properties and use
		//int Style;
		//int Width;
		//COLORREF color;
	};

	class Brush :public BaseGDIObject {
	public:
		Brush(COLORREF crColor) {
			Init(CreateSolidBrush(crColor));
		//	Object = std::make_shared<BaseObject>(temp);
		}
		Brush(int iHatch, COLORREF Color) {
			Init(CreateHatchBrush(iHatch, Color));
		//	Object = std::make_shared<BaseObject>(temp);
		}

		GDIObjectType Type()const override {
			return GDIObjectType::brush;
		}

		operator HBRUSH() {
			//HGDIOBJ temp =  *Object;
			return (HBRUSH)RetrieveObject();
		}
	private:
		Brush(Brush&);

	};

	enum RegionTypes {
		rectangular,
		elliptical,
		roundrect
	};

	class Region :public BaseGDIObject{
	public:
		Region(Rectangle rect, bool Elliptical) {
			if (Elliptical) {
				Init(CreateEllipticRgnIndirect(&rect.rect));
			}
			else {
				Init(CreateRectRgnIndirect(&rect.rect));
			}
		}

		Region(Rectangle rect, Pair CornerEllipse) {
			Init(CreateRoundRectRgn(
				rect.left, rect.top,
				rect.right, rect.bottom,
				CornerEllipse.x, CornerEllipse.y
			));
		}

		Region(std::vector<POINT>Points, int PolyFillMode) {
			Init(CreatePolygonRgn(&Points[0], Points.size(), PolyFillMode));
		}

		void Combine(Region region1, Region region2, int iMode) {
			HRGN temp = CreateRectRgn(0, 0, 1, 1);
			CombineRgn(temp, region1, region2, iMode);
		}

		operator HRGN() {
			return (HRGN)RetrieveObject();
		}

	private:
		Region(Region&);


	};

	//TODO Implement DrawText with this class
	class TextCursor {



	};




}