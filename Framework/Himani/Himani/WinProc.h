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




/////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                         His Code Ends                                           */
/////////////////////////////////////////////////////////////////////////////////////////////////////




namespace Himani {


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

	class FunctionThunk {
		friend class HWindowsProc;
		friend class HDialogProc;
	public:
		FunctionThunk();
		~FunctionThunk();

	protected:
		//Returns the windows Procedure specific to our class Object
		WNDPROC Procedure() {
			return thunk->GetThunkAddress();
		}


	private:
		winThunk * thunk;
		static HANDLE eHeapAddr;
		static int objInstances;

	};

	class HWindowsProc :public FunctionThunk {
	public:
		HWindowsProc() {
			thunk->Init(this, (DWORD_PTR)WndProc);
		}

	protected:
		virtual LRESULT MessageFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
			return DefWindowProc(hwnd, message, wParam, lParam);
		}

	private:
#if defined(_M_IX86)
		static LRESULT CALLBACK WndProc(HWindowsProc* pThis, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
#elif defined(_M_AMD64)
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, HWindowsProc* pThis) {
#endif
			return pThis->MessageFunc(hwnd, message, wParam, lParam);
		}

	};


	class HDialogProc :public FunctionThunk {
	public:
		HDialogProc() {
			thunk->Init(this, (DWORD_PTR)WndProc);
		}

	protected:
		virtual BOOL MessageFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
			return DefWindowProc(hwnd, message, wParam, lParam);
		}

	private:
#if defined(_M_IX86)
		static BOOL CALLBACK WndProc(HDialogProc* pThis, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
#elif defined(_M_AMD64)
		static BOOL CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, HDialogProc* pThis) {
#endif
			return pThis->MessageFunc(hwnd, message, wParam, lParam);
		}

	};

#pragma endregion


}