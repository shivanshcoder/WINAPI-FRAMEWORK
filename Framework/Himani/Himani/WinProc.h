#pragma once
/*
	Code Seems complete as of 27-12-2018
	No need of change!!
*/
#include"Hpch.h"
#include"Core.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                 This Code doesn't belong to me                                 */
/*         The Code has been taken from a article from CodeProject by Jon Campbell                 */
/* https://www.codeproject.com/articles/1121696/cplusplus-winapi-wrapper-object-using-thunks-x-and */
/*              Thanks to Jon Campbell for helping out with Thunking for WNDPROC                   */
/////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                        His Code Starts                                          */
/////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined(_M_IX86)	
#pragma pack(push,1)


struct winThunk {

	unsigned short m_push1; //push dword ptr[esp] ; push return address
	unsigned short m_push2;

	unsigned short m_mov1; //mov dword ptrpesp+0x4], pThis; Set our new parameter by replacing old return address
	unsigned char m_mov2; //(esp+0x4 is first parameter)

	unsigned long m_this; //ptr to our object

	unsigned char m_jmp; //jmp WndProc

	unsigned long m_relproc; //relative proc

	static HANDLE eHeapAddr;

	bool Init(void* Instance_to_attach, DWORD_PTR Function_to_suppply);

	//Some thunk will dynamically allocate the memory for the code
	WNDPROC GetThunkAddress();

	void* operator new(size_t, HANDLE HeapAddr);

	void operator delete(void* pThunk);
};

#pragma pack(pop)

#elif defined(_M_AMD64)
#pragma pack(push, 2)

struct winThunk {
	unsigned short     RaxMov;  //movabs rax, pThis
	unsigned long long RaxImm;
	unsigned long      RspMov;  //mov [rsp+28], rax
	unsigned short     RspMov1;
	unsigned short     Rax2Mov; //movabs rax, proc
	unsigned long long ProcImm;
	unsigned short     RaxJmp;  //jmp rax
	static HANDLE      eHeapAddr; //heap address this thunk will be initialized too
	bool Init(void *pThis, DWORD_PTR proc);

	WNDPROC GetThunkAddress();

	void* operator new(size_t, HANDLE heapaddr);

	void operator delete(void* pThunk);

};

#pragma pack(pop)
#endif


#if defined(_M_IX86)
#pragma pack(push, 1)

bool winThunk::Init(void* Instance_to_attach, DWORD_PTR Function_to_suppply) {
	m_push1 = 0x34ff; //ff 34 23 push DWORD PTR [esp]
	m_push2 = 0xc724;

	m_mov1 = 0x2444; //c7 44 24 04 mov dword ptr [esp +0x4],
	m_mov2 = 0x04l;

	m_this = PtrToUlong(Instance_to_attach);

	m_jmp = 0xe9; //jmp

				  //Calculate relative address of proc to jump to
	m_relproc = unsigned long((INT_PTR)Function_to_suppply - ((INT_PTR)this + sizeof(winThunk)));

	//write block from data cache and flush from instruction cache
	if (FlushInstructionCache(GetCurrentProcess(), this, sizeof(winThunk)))
		//Success
		return true;
	else
		//Fail
		return false;
}

WNDPROC winThunk::GetThunkAddress() {
	return (WNDPROC)this;
}

void* winThunk::operator new(size_t, HANDLE HeapAddr) {
	eHeapAddr = HeapAddr;

	return HeapAlloc(HeapAddr, 0, sizeof(winThunk));
}

void winThunk::operator delete(void* pThunk) {
	HeapFree(eHeapAddr, 0, pThunk);
}

HANDLE winThunk::eHeapAddr = NULL;
#pragma pack(pop)

#elif defined(_M_AMD64)

#pragma pack(push, 2)

bool winThunk::Init(void *pThis, DWORD_PTR proc) {
	RaxMov = 0xb848;                    //movabs rax (48 B8), pThis
	RaxImm = (unsigned long long)pThis; //
	RspMov = 0x24448948;                //mov qword ptr [rsp+28h], rax (48 89 44 24 28)
	RspMov1 = 0x9028;                    //to properly byte align the instruction we add a nop (no operation) (90)
	Rax2Mov = 0xb848;                    //movabs rax (48 B8), proc
	ProcImm = (unsigned long long)proc;
	RaxJmp = 0xe0ff;                     //jmp rax (FF EO)
	//write block from data cache and flush from instruction cache
	if (FlushInstructionCache(GetCurrentProcess(), this, sizeof(winThunk)))
		//Success
		return true;
	else
		//Fail
		return false;

}

WNDPROC winThunk::GetThunkAddress() {
	return (WNDPROC)this;
}

void* winThunk::operator new(size_t, HANDLE HeapAddr) {
	eHeapAddr = HeapAddr;

	return HeapAlloc(HeapAddr, 0, sizeof(winThunk));
}


void winThunk::operator delete(void* pThunk) {
	HeapFree(eHeapAddr, 0, pThunk);
}

HANDLE winThunk::eHeapAddr = NULL;

#pragma pack(pop)

#endif



/////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                         His Code Ends                                           */
/////////////////////////////////////////////////////////////////////////////////////////////////////




namespace Himani{


	//The Default Callback Windows Procedure for every window
	static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
		if (message == WM_CREATE) {

			//Replaces the Callback Procedure with the Thunk we have supplied with the CREATESTRUCT param
			SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)(((LPCREATESTRUCT)lParam)->lpCreateParams));
			//return TRUE;
		}
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

#pragma region HBaseWinProc

	class HBaseWinProc {
	public:
		HBaseWinProc();
		~HBaseWinProc();

	protected:
		//Returns the windows Procedure specific to our class Object
		WNDPROC Procedure() {
			return thunk->GetThunkAddress();
		}
		virtual LRESULT MessageFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
			return DefWindowProc(hwnd, message, wParam, lParam);
		}
	private:
		winThunk * thunk;
		static HANDLE eHeapAddr;
		static int objInstances;

#if defined(_M_IX86)
		static LRESULT CALLBACK WndProc(HBaseWinProc* pThis, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
#elif defined(_M_AMD64)
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, HBaseWinProc* pThis) {
#endif
			return pThis->MessageFunc(hwnd, message, wParam, lParam);
		}

	};

	HBaseWinProc::HBaseWinProc() {
		++objInstances;
		if (!eHeapAddr) {
			eHeapAddr = HeapCreate(HEAP_CREATE_ENABLE_EXECUTE | HEAP_GENERATE_EXCEPTIONS, 0, 0);

			if (!eHeapAddr)
				CheckDefaultWinError;
		}
		try {
			thunk = new(eHeapAddr)winThunk;
		}
		catch (...) {
			throw Himani::Exceptions(L"Thunk could not be allocated");
		}

		if (!thunk->Init(this, (DWORD_PTR)WndProc))
			CheckDefaultWinError;
	}
	HBaseWinProc::~HBaseWinProc() {
		if (objInstances == 1) {
			//TOOD Check for Error or not?
			HeapDestroy(eHeapAddr);
			eHeapAddr = NULL;
			objInstances = 0;
		}
		else {
			objInstances--;
			delete thunk;
		}
	}

	HANDLE HBaseWinProc::eHeapAddr = nullptr;
	int HBaseWinProc::objInstances = 0;


#pragma endregion


}