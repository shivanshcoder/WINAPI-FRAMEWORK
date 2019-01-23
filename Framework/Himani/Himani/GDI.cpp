#include "Hpch.h"
#include "GDI.h"

//Should not be inititialzed with nullptr
namespace Himani {
	void Himani::HGDIObject::Init(HGDIOBJ Obj, bool stockObject) {
		if (!Obj)
			throw Exceptions(__LINE__, TEXT(__FILE__), TEXT("Object Initialized with nullptr"));

		Object = std::make_shared<HBaseObject>(Obj, stockObject);
	}
	inline Region::Region(bool Elliptical, Helpers::HRect rect) {
		if (Elliptical) {
			Init(CreateEllipticRgnIndirect(&rect.rect), false);
		}
		else {
			Init(CreateRectRgnIndirect(&rect.rect), false);
		}
	}
	inline Region::Region(std::vector<POINT> Points, int PolyFillMode) {
		Init(CreatePolygonRgn(&Points[0], Points.size(), PolyFillMode), false);
	}
	inline Region::Region(const Region & region1, Region const & region2, int iMode) {
		HRGN temp = CreateRectRgn(0, 0, 1, 1);
		CombineRgn(temp, static_cast<HRGN>(region1),
			static_cast<HRGN>(region2), iMode);
		Init(temp, false);
	}
	inline void Region::clear() {
		Init(CreateRectRgn(0, 0, 1, 1), false);
	}
	inline void Region::Combine(const Region & region1, Region const & region2, int iMode) {
		//HRGN temp = CreateRectRgn(0, 0, 1, 1);
		CombineRgn(static_cast<HRGN>(*this), static_cast<HRGN>(region1),
			static_cast<HRGN>(region2), iMode);
		//Init(temp);
	}
	inline Region::Region(Region & region1, Region & region2, int iMode) {
		HRGN temp = CreateRectRgn(0, 0, 1, 1);
		CombineRgn(temp, static_cast<HRGN>(region1),
			static_cast<HRGN>(region2), iMode);
		Init(temp, false);
	}

	//CLEAN is it really needed???????????????????????????????
	//For attaching the object which already has a scope from calling place

	inline void SafeDC::Attach(HGDIObject & Object) {

		Objects[Object.Type()] = Object;
		SelectObject(hdc, (Object.RetrieveObject()));
	}

	//For on the go constructor type Object

	inline void SafeDC::Attach(const HGDIObject & Object) {

		Objects[Object.Type()] = HGDIObject(Object);
		SelectObject(hdc, Objects[Object.Type()].RetrieveObject());
	}
}
