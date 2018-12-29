#pragma once
#include"Helpers.h"
#include"BaseWin.hpp"
#include<memory>
#include<chrono>

namespace WINAPIPP {

	enum GDIObjectType {
		pen,
		brush,
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

		BaseObject(HGDIOBJ Obj) {
			Object = Obj;
		}

		operator HGDIOBJ() const {
			return Object;
		}

	protected:
		//NOTE Disabled CopyConstructor
		BaseObject(BaseObject& t) = delete;

		HGDIOBJ Object;
	};

	//Wrapper for hiding std::shared_ptr and base class for GDIObjects
	class GDIObject {
		friend class DC;
		friend class SafeDC;

	protected:

		//For Getting underlying Handle
		HGDIOBJ RetrieveObject() const {
			return Object->Object;
		}

		//Should not be inititialzed with NULL
		void Init(HGDIOBJ Obj) {
			if (!Obj)
				throw Exceptions(__LINE__, TEXT(__FILE__), TEXT("Object Initialized with NULL"));

			Object = std::make_shared<BaseObject>(Obj);
		}

		//Returns Type of Object
		virtual GDIObjectType Type()const { return base; }// = 0;

	private:
		std::shared_ptr<BaseObject>Object;
	};

	class Pen :public GDIObject {
		friend class DC;

	public:
		Pen(int iStyle, int cWidth, COLORREF color)
		{
			Init(CreatePen(iStyle, cWidth, color));
		}

		GDIObjectType Type()const override {
			return GDIObjectType::pen;
		}

		/*----Disabled Functions---*/
		//Copy constructor disabled
		Pen(Pen&) = delete;
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
		}
		Brush(int iHatch, COLORREF Color) {
			Init(CreateHatchBrush(iHatch, Color));
		}

		GDIObjectType Type()const override {
			return GDIObjectType::brush;
		}


		/*----Disabled Functions---*/
		//Copy constructor disabled
		Brush(Brush&) = delete;
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
		Region(bool Elliptical, Helpers::Rect rect) {
			if (Elliptical) {
				Init(CreateEllipticRgnIndirect(&rect.rect));
			}
			else {
				Init(CreateRectRgnIndirect(&rect.rect));
			}
		}

		/*Region(Rect rect, Pair CornerEllipse) {
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


		/*----Disabled Functions---*/
		//Copy constructor disabled
		Region(Region&) = delete;
		Region& operator=(Region const&) = delete;
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

		void TextOut(Helpers::Point point, std::wstring string) {
			::TextOut(hdc, point.x, point.y, string.c_str(), string.size());
		}



		bool FillRect(Helpers::Rect Rect, Brush &Brush) {
			return ::FillRect(hdc, &Rect.rect, static_cast<HBRUSH>(Brush));
		}

		bool FrameRgn(Region &Reg, Brush &brush, int width, int height) {
			return ::FrameRgn(hdc, static_cast<HRGN>(Reg), static_cast<HBRUSH>(brush), width, height);
		}

		bool InvertRgn(Region &Reg) {
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

		operator HDC() {
			return hdc;
		}

		virtual ~DC() {
			if (hwnd) {
				::ReleaseDC(hwnd, hdc);
			}
		}

		DC(DC&) = delete;
		DC& operator=(DC&) = delete;

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

			Objects[Object.Type()] = Object;
			SelectObject(hdc, (Object.RetrieveObject()));
		}

		//For on the go constructor type Object
		void Attach(const GDIObject &Object) {

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

		QuickDC(QuickDC&) = delete;
		QuickDC& operator=(QuickDC&) = delete;

	private:
		//		QuickDC(QuickDC&);
	};

	//Should be made in WM_PAINT message only
	class PaintDC :public SafeDC {
	public:
		PaintDC(HWND _hwnd) :SafeDC(_hwnd) {
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

		PaintDC(PaintDC&) = delete;
		PaintDC& operator=(PaintDC&) = delete;

		PAINTSTRUCT ps;
	};


}