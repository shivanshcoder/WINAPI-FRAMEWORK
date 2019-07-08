#pragma once
#include"Hpch.h"
#include"WinProc.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                        His Code Starts                                          */
/////////////////////////////////////////////////////////////////////////////////////////////////////

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

bool winThunk::Init(void* pThis, DWORD_PTR proc) {
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


namespace Himani {

	FunctionThunk::FunctionThunk() {
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

		//if (!thunk->Init(this, (DWORD_PTR)WndProc))
		//	CheckDefaultWinError;
	}
	FunctionThunk::~FunctionThunk() {
		deleteThunk();
	}

	void FunctionThunk::deleteThunk(){
		if (!thunk)return;

		if (objInstances == 1) {
			//TOOD Check for Error or not?
			delete thunk;
			HeapDestroy(eHeapAddr);
			eHeapAddr = NULL;
			objInstances = 0;
		}
		else if(objInstances >= 0){
			objInstances--;
			delete thunk;
		}
	}

	HANDLE FunctionThunk::eHeapAddr = nullptr;
	int FunctionThunk::objInstances = 0;

	LRESULT HWindowsProc::__MessageProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	BOOL HDialogProc::__MessageFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
		return TRUE;
	}

}