#pragma once
#include"Helpers.h"
#include"HBaseWin.h"


namespace Himani {

	enum HGDIObjectType {
		pen,
		brush,
		base
	};

	//	HBaseObject class for auto deletion of object
	class HBaseObject {
		friend class HGDIObject;

	public:
		~HBaseObject() {
			if (!stockObject && Object)
				DeleteObject(Object);
		}

		HBaseObject(HGDIOBJ Obj, bool _stockObject) {
			Object = Obj;
			stockObject = _stockObject;
		}

		operator HGDIOBJ() const {
			return Object;
		}



	protected:
		//NOTE Disabled CopyConstructor
		HBaseObject(HBaseObject& t) = delete;
		HBaseObject& operator=(const HBaseObject&) = delete;
		HGDIOBJ Object;
		bool stockObject;

	};

	//Wrapper for hiding std::shared_ptr and base class for HGDIObjects
	class HGDIObject {
		friend class DC;
		friend class SafeDC;
		friend class HStaticWindow;
		friend class HScrollbar;
		//For Getting underlying Handle
		//TODO make it protected maybe????????????
	public:
		HGDIOBJ RetrieveObject() const {
			return Object->Object;
		}

	protected:
		//Should not be inititialzed with nullptr
		void Init(HGDIOBJ Obj, bool stockObject);

		//Returns Type of Object
		virtual HGDIObjectType Type()const { return base; }// = 0;

	private:
		std::shared_ptr<HBaseObject>Object;
	};

	/*
		Pen Class for GDI
	*/
	//TODO Try Implementing GDI objects like the Shared_ptr Implementation
	class HPen :public HGDIObject {
		friend class DC;

	public:
		HPen() = delete;
		HPen(int iStyle, int cWidth, COLORREF color) {
			Init(CreatePen(iStyle, cWidth, color), false);
		}
		HPen(int StockObjectCode) {
			Init(GetStockObject(StockObjectCode), true);
		}
		HGDIObjectType Type()const override {
			return HGDIObjectType::pen;
		}

		/*----Disabled Functions---*/
		//Copy constructor disabled
		HPen(HPen&) = delete;
		HPen& operator=(HPen const&) = delete;
	protected:
		explicit operator HPEN() {
			return (HPEN)RetrieveObject();
		}
		
	};

	class HBrush :public HGDIObject {
		friend class DC;

	public:

		HBrush(COLORREF crColor) {
			Init(CreateSolidBrush(crColor), false);
		}
		HBrush(int iHatch, COLORREF Color) {
			Init(CreateHatchBrush(iHatch, Color), false);
		}
		HBrush(int StockObjectCode) {
			Init(GetStockObject(StockObjectCode), true);
		}

		HGDIObjectType Type()const override {
			return HGDIObjectType::brush;
		}


		/*----Disabled Functions---*/
		//Copy constructor disabled
		HBrush(HBrush&) = delete;
		HBrush& operator=(HBrush const&) = delete;

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

	class Region :public HGDIObject {
		friend class DC;
	public:
		Region() {
			Init(CreateRectRgn(0, 0, 1, 1), false);
		}
		Region(bool Elliptical, Helpers::HRect rect);

		Region(std::vector<POINT>Points, int PolyFillMode);

		Region(Region &region1, Region &region2, int iMode);

		Region(const Region &region1, Region const &region2, int iMode);

		void clear();
		void Combine(const Region &region1, Region const &region2, int iMode);


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
		//if hwnd is nullptr, it won't get automatically deleted!
		DC(HWindow *_hwnd)
			:hwnd(_hwnd),hdc(nullptr) {}

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

		void TextOut(Helpers::HPoint point, HString string) {
			::TextOut(hdc, point.x, point.y, string.c_str(), string.size());
		}

		void SetBkColor(COLORREF color) {
			::SetBkColor(hdc, color);
		}

		void SetTextColor(COLORREF color) {
			::SetTextColor(hdc, color);
		}
		
		bool FillRect(Helpers::HRect HRect, HBrush &HBrush) {
			return ::FillRect(hdc, &HRect.rect, static_cast<HBRUSH>(HBrush));
		}

		bool FrameRgn(Region &Reg, HBrush &brush, int width, int height) {
			return ::FrameRgn(hdc, static_cast<HRGN>(Reg), static_cast<HBRUSH>(brush), width, height);
		}

		bool InvertRgn(Region &Reg) {
			return ::InvertRgn(hdc, static_cast<HRGN>(Reg));
		}

		void SelectRegion(Region &Reg) {
			SelectClipRgn(hdc, static_cast<HRGN>(Reg));
		}

		/*			Wrappers				*/

		virtual void Attach(HGDIObject &Object) {
			SelectObject(hdc, Object.RetrieveObject());
		}

		void AttachStockObject(int Object) {
			SelectObject(hdc, GetStockObject(Object));
		}

		operator HDC() {
			return hdc;
		}

		virtual ~DC() {
			//if (hwnd) {
				::ReleaseDC(hwnd->Handle(), hdc);
		//	}
		}

		DC(DC&) = delete;
		DC& operator=(DC&) = delete;

	protected:
		//HWND hwnd;
		HWindow* hwnd;
		HDC hdc;
	};

	/*
		This DC implementation saves the Objects currently selected for the respective DCs
		This uses the std::shared_ptr for making it safe
		In case the the HBaseObject gets out of scope, this prevents it from getting deleted
		and automatically deletes the objects with it's own destructor
	*/
	class SafeDC :public DC {
	public:
		using DC::DC;

		//CLEAN is it really needed???????????????????????????????
		//For attaching the object which already has a scope from calling place
		void Attach(HGDIObject &Object)override;

		//For on the go constructor type Object
		void Attach(const HGDIObject &Object);

	private:
		//Saving reference of the selected objects
		HGDIObject Objects[5];
	};

	class QuickDC :public DC {
	public:
		//TODO make it HWindow compatible only
		QuickDC(HWindow* _hwnd) :DC(_hwnd) {
			hdc = GetDC(_hwnd->Handle());
		}

		//URGENT
		//SHouldn' it be disabled?
		//QuickDC(HDC dc):DC(nullptr) {
		//	hdc = dc;
		//}

		QuickDC(QuickDC&) = delete;
		QuickDC& operator=(QuickDC&) = delete;

	private:
	};

	//Should be made in WM_PAINT message only
	class PaintDC :public SafeDC {
	public:
		PaintDC(HWindow* _hwnd) :SafeDC(_hwnd) {
			hwnd = _hwnd;
			hdc = BeginPaint(hwnd->Handle(), &ps);
		}

		operator HDC() {
			return hdc;
		}

		~PaintDC() {
			//if (hwnd) {
				EndPaint(hwnd->Handle(), &ps);
			//}
		}

		PaintDC(PaintDC&) = delete;
		PaintDC& operator=(PaintDC&) = delete;

		PAINTSTRUCT ps;
	};


}