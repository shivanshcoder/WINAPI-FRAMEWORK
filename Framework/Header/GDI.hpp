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
		friend class GDIObject;


	public:
		~BaseObject() {
			if (Object)
				DeleteObject(Object);
		}


		HGDIOBJ Object;

	protected:
		operator HGDIOBJ() {
			return Object;
		}

		//NOTE Disabled CopyConstructor

		//TODO check if it affects or not
		//BaseObject(BaseObject& t) {}
		BaseObject(BaseObject& t) = delete;

	};


#pragma region GDIOBjects


	//TODO: Same GDIObject can be selected for multiple DC, need to verify if that is okay
	//It is okay to have same logical pen for different DCs
	//Wrapper for hiding std::shared_ptr and base class for GDIObjects
	class GDIObject {
		friend class DC;
		friend class SafeDC;

	protected:

		//For Getting underlying Handle
		HGDIOBJ RetrieveObject() const {
			return Object->Object;

		}

		void Init(HGDIOBJ Obj) {
			Object = std::make_shared<BaseObject>(Obj);
		}

		//Returns Type of Object
		virtual GDIObjectType Type()const { return base; }// = 0;

		//NOTE
		//operator std::shared_ptr<BaseObject>() {
		//	return Object;
		//}


		std::shared_ptr<BaseObject>GDIObject;
	};


	class Pen :public GDIObject {
		friend class DC;

	public:
		Pen(int iStyle, int cWidth, COLORREF color)
		{
			Init(CreatePen(iStyle, cWidth, color));
			//Object = std::make_shared<BaseObject>(temp);
		}
		Pen(Pen&) = delete;

		GDIObjectType Type()const override {
			return GDIObjectType::pen;
		}

		Pen& operator=(Pen const&) = delete;
	protected:
		explicit operator HPEN() {
			return (HPEN)RetrieveObject();
		}



		//TODO store properties and use
		//int Style;
		//int Width;
		//COLORREF color;
	};

	class Brush :public GDIObject {
		friend class DC;

	public:
		Brush(COLORREF crColor) {
			Init(CreateSolidBrush(crColor));
			//	Object = std::make_shared<BaseObject>(temp);
		}
		Brush(int iHatch, COLORREF Color) {
			Init(CreateHatchBrush(iHatch, Color));
			//	Object = std::make_shared<BaseObject>(temp);
		}
		//Copy constructor disabled
		Brush(Brush&) = delete;

		GDIObjectType Type()const override {
			return GDIObjectType::brush;
		}


		Brush& operator=(Brush const&) = delete;

	protected:

		explicit operator HBRUSH() const {

			return (HBRUSH)RetrieveObject();
		}



	};

	enum RegionTypes {
		rectangular,
		elliptical,
		roundrect
	};

	class Region :public GDIObject {
		friend class DC;
	public:
		Region() {
			Init(CreateRectRgn(0, 0, 1, 1));
		}
		Region(bool Elliptical, Rectangle rect) {
			if (Elliptical) {
				Init(CreateEllipticRgnIndirect(&rect.rect));
			}
			else {
				Init(CreateRectRgnIndirect(&rect.rect));
			}
		}

		/*Region(Rectangle rect, Pair CornerEllipse) {
			Init(CreateRoundRectRgn(
				rect.left, rect.top,
				rect.right, rect.bottom,
				CornerEllipse.x, CornerEllipse.y
			));
		}*/

		Region(std::vector<POINT>Points, int PolyFillMode) {
			Init(CreatePolygonRgn(&Points[0], Points.size(), PolyFillMode));
		}

		Region(Region &region1, Region &region2, int iMode) {
			HRGN temp = CreateRectRgn(0, 0, 1, 1);
			CombineRgn(temp, static_cast<HRGN>(region1),
				static_cast<HRGN>(region2), iMode);
			Init(temp);
		}

		Region(const Region &region1, Region const &region2, int iMode) {
			HRGN temp = CreateRectRgn(0, 0, 1, 1);
			CombineRgn(temp, static_cast<HRGN>(region1),
				static_cast<HRGN>(region2), iMode);
			Init(temp);
		}

		void clear() {
			Init(CreateRectRgn(0, 0, 1, 1));
		}
		void Combine(const Region &region1, Region const &region2, int iMode) {
			//HRGN temp = CreateRectRgn(0, 0, 1, 1);
			CombineRgn(static_cast<HRGN>(*this), static_cast<HRGN>(region1),
				static_cast<HRGN>(region2), iMode);
			//Init(temp);
		}
		Region(Region&) = delete;
		Region& operator=(Region const&) = delete;
		//Region& operator=(Region &) = delete;
	protected:
		explicit operator HRGN()const {
			return (HRGN)RetrieveObject();
		}



	};
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

		bool FillRect(Rectangle Rect, Brush &Brush) {
			return ::FillRect(hdc, &Rect.rect, static_cast<HBRUSH>(Brush));
		}

		bool FrameRgn(Region Reg, Brush brush, int width, int height) {
			return ::FrameRgn(hdc, static_cast<HRGN>(Reg), static_cast<HBRUSH>(brush), width, height);
		}

		bool FillRect(Rectangle Rect, Brush &Brush) {
			return ::FillRect(hdc, &Rect.rect, static_cast<HBRUSH>(Brush));
		}

		bool FrameRgn(Region Reg, Brush brush, int width, int height) {
			return ::FrameRgn(hdc, static_cast<HRGN>(Reg), static_cast<HBRUSH>(brush), width, height);
		}

		bool InvertRgn(Region Reg) {
			return ::InvertRgn(hdc, static_cast<HRGN>(Reg));
		}

		void SelectRegion(Region &Reg) {
			SelectClipRgn(hdc, static_cast<HRGN>(Reg));
		}

		/*			Wrappers				*/

		virtual void Attach(GDIObject &Object) {
			SelectObject(hdc, Object.RetrieveObject());
		}

		void AttachStockObject(int Object) {
			SelectObject(hdc, GetStockObject(Object));
		}

		operator HDC()const {
			return hdc;
		}

		virtual ~DC() {
			if (hwnd) {
				::ReleaseDC(hwnd, hdc);
			}
		}

		DC(DC& const) = delete;
		DC& operator=(const DC&) = delete;

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
		void Attach(GDIObject &Object)override {
			//TODO better checking
			//Strange
			if (Object.Type() == base)
				__debugbreak();
			Objects[Object.Type()] = Object;
			SelectObject(hdc, (Object.RetrieveObject()));
		}

		//For on the go constructor type Object
		void Attach(const GDIObject &Object) {
			//TODO better checking
			//Strange
			if (Object.Type() == base)
				__debugbreak();
			Objects[Object.Type()] = GDIObject(Object);
			SelectObject(hdc, Objects[Object.Type()].RetrieveObject());
		}

	private:
		//Saving reference of the selected objects
		GDIObject Objects[5];
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


	//TODO Implement DrawText with this class
	class TextCursor {



	};




}