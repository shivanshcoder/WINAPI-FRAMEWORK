
#include"Hpch.h"
#define AUTO_APP_ENTRY
#include"Himani.h"
#include"Himani/Himani/PredefinedClasses.h"
#include"Himani/Himani/SampleWinClasses.h"
#include"Himani/Log System/Log.h"
#include"resource.h"

int mainTLS = -1;

DWORD func(PVOID c) {
	return 0;
}

DWORD func2(PVOID c) {
	return 0;
}

class MainApp :public Himani::HBaseApp {
public:
	MainApp():
	w(TEXT("SAMPLE") ),z(w, MAKEINTRESOURCE(IDD_DIALOG1)) {
		//Himani::CreateDialogBox<Himani::HBaseDialog>(MAKEINTRESOURCE(IDD_DIALOG1), w);
		w.Show(SW_NORMAL);
		z.Show(SW_NORMAL);
		//PostQuitMessage(0);
		
	}
	Himani::HSimpleWindow w;
	Himani::HBaseDialog z;
};


ENTRY_APP(MainApp);

