#include<Windows.h>


class BrushBase {
public:
	BrushBase ( COLORREF Color );

private:
	HBRUSH hbrush;
};
