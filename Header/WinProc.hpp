#pragma once
#include<Windows.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                      This Code doesn't belong to me                             */
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
	
	bool Init (void* Instance_to_attach, DWORD_PTR Function_to_suppply); 
	
	//Some thunk will dynamically allocate the memory for the code
	WNDPROC GetThunkAddress ();

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
	bool Init (void *pThis, DWORD_PTR proc);
	
	WNDPROC GetThunkAddress ();
	
	void* operator new(size_t, HANDLE heapaddr);
	
	void operator delete(void* pThunk);
	
};

#pragma pack(pop)
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                         His Code Ends                                           */
/////////////////////////////////////////////////////////////////////////////////////////////////////



//Function to place breakpoint in case of error
inline void CheckError () {
	char buf[256];
	FormatMessageA (FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, ::GetLastError (), MAKELANGID (LANG_NEUTRAL, SUBLANG_DEFAULT),
		buf, sizeof (buf), NULL);
	if (strcmp (buf, "The operation completed successfully.\r\n")) {
		__debugbreak ();
	}
	return;
}

namespace WINAPIPP {
	class BaseWinProc {
	public:
		BaseWinProc ();
		~BaseWinProc ();

	protected:
		WNDPROC Procedure;
		virtual LRESULT MessageFunc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	private:
		winThunk * thunk;
		static HANDLE eHeapAddr;
		static int objInstances;

#if defined(_M_IX86)
		static LRESULT CALLBACK WndProc (BaseWinProc* pThis, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
			pThis->MessageFunc (hwnd, message, wParam, lParam);
		}
#elif defined(_M_AMD64)
		static LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, BaseWinProc* pThis);
#endif
	};
}