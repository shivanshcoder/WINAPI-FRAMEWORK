#pragma once
#include"Hpch.h"

/*
	Tasklist Tags!
	URGENT
	TODO
	WARNING
	VERSION
*/


#pragma region CUSTOM_CLASS_MACROS
///Function for Getting class Name
///#define DEFINE_CLASSNAME(ClassName__) LPCWSTR ClassName()override { return L###ClassName__; } 

///#define OVERRIDE_PREDEFINEDCLASS(ClassName__) DEFINE_CLASSNAME(ClassName__)

#define WINCLASS_PROPERTIES(__ClassName, Style) inline static Himani::HWinClassProperties __Prop = { TEXT( #__ClassName ), Himani::CommonWndProc<__ClassName>,(Style)};\
	Himani::HString& ClassName()override {		\
		return __Prop.ClassName;			\
	}											\
friend LRESULT CALLBACK Himani::CommonWndProc<__ClassName>(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

#pragma endregion


#pragma region ErrorMacros

#define S(x) #x
#define S_(x) S(x)
#define __S_LINE__ S_(__LINE__)

#define CheckDefaultWinError	if (GetLastError())	throw Himani::WinExceptions(__LINE__,TEXT(__FILE__))

	//Delete or make something else
#define CheckTempError if(GetLastError()){\
	wchar_t *buf;	\
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,\
		NULL, ::GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),\
		(LPWSTR)& buf, sizeof(buf), NULL);\
	__debugbreak();\
	}

#define DISABLE_CLASS_COPY_ASSIGNMENT(ClassName) \
	ClassName(const ClassName&) = delete;\
	ClassName& operator=(const ClassName&) = delete;


#pragma endregion


namespace Himani {


#ifdef UNICODE
	typedef std::wstring HString;
#else
	typedef std::string HString;
#endif



	extern HINSTANCE __ProgramInstance;
	extern int __ProgramCmdShow;

	inline HINSTANCE Instance() {
		return __ProgramInstance;
	}
	inline int CmdShow() {
		return __ProgramCmdShow;
	}

	//Class storing all the 
	struct Framework {

		std::vector<HString>ClassNameList;
	};


#pragma region ErrorHandling


	/*------------------------------------Error Handling------------------------------------*/
	class Exceptions {

	public:

		Exceptions(const HString& data = TEXT("Unknown Error")) :Data(data) {}

		Exceptions(int LineNumber, const HString FilePath, const HString& data = TEXT("Unknown Error"));

		const wchar_t* what() {
			return Data.c_str();
		}

	protected:
		HString Data;
	};

	class WinExceptions :public Exceptions {
	public:
		WinExceptions(int LineNumber, const HString FilePath);

	};



	/*------------------------------------Error Handling------------------------------------*/

#pragma endregion



	//TODO make HandleWrapperClass for all types of handles
	template<class HandleType>
	class HHandleWrapperBaseClass {
	public:
		HHandleWrapperBaseClass() {
			handle = (nullptr);
		}

		explicit HHandleWrapperBaseClass(HandleType __handle) {
			handle = __handle;
		}

		HandleType Handle()const {
			if (handle)
				return handle;
			//	else
				//	throw Exceptions(TEXT("Handle yet not Initialized"));
		}


		explicit operator HandleType() {
			return handle;
		}

		//This should be Protected?
	//protected:
		void InitHandle(HandleType __handle) {
			handle = __handle;
		}

		virtual ~HHandleWrapperBaseClass() {}

	private:
		HandleType handle;
	};

	class HWNDCLASS {
	public:
		HWNDCLASS() {
			handle = (nullptr);
		}

		explicit HWNDCLASS(HWND __handle) {
			handle = __handle;
		}

		HWND Handle()const {
			if (handle)
				return handle;
			//	else
				//	throw Exceptions(TEXT("Handle yet not Initialized"));
		}


		explicit operator HWND() {
			return handle;
		}

		//This should be Protected?
	//protected:
		void InitHandle(HWND __handle) {
			handle = __handle;
		}

		virtual ~HWNDCLASS() {}

	private:
		HWND handle;
	};

	/*
	----------------------- HWCustomWindow Classes Constructor Arguement Helper Class------------------------------
	This is a helper class for Initializing Extra Data members even if class is being constructed using CreateWin!
	HCustomWindow Derived Class Constructors will only take pointer to one of such class object(i.e. User should derive from
	*/
	class HClassInitializer {
		template<class OwnerWindow>
		friend LRESULT CALLBACK CommonWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
		friend class HCustomWindow;

	public:

		HClassInitializer() :hwnd(nullptr), SelfDestruct(false) {}

	private:
		HClassInitializer(HWND wnd , bool AllowSelfDestruct) :hwnd(wnd),SelfDestruct(AllowSelfDestruct) {}

		HWND hwnd;
		bool SelfDestruct;
	};

	/*
	------------------------------------------Threading Helping Components--------------------------------------------------
	*/


	/*
		Critical Section Wrapper
		A simple wrapper with no overhead and simple functionality
	*/
	class HCriticalSectionBase {

	public:


		class HCriticalSection {
			friend HCriticalSectionBase;
			//Takes already Initialized CRITICAL_SECTION
			HCriticalSection(CRITICAL_SECTION& _section)
				:section(_section) {
				EnterCriticalSection(&section);
			}

			CRITICAL_SECTION& section;

		public:
			~HCriticalSection() {
				LeaveCriticalSection(&section);
			}
		};

		HCriticalSectionBase(HCriticalSectionBase const&) = delete;
		HCriticalSectionBase(HCriticalSectionBase&&) = delete;
		HCriticalSectionBase& operator=(HCriticalSectionBase const&) = delete;
		HCriticalSectionBase& operator=(HCriticalSectionBase&&) = delete;

		HCriticalSectionBase() {
			InitializeCriticalSection(&sc);
		}

		inline void Enter() {
			EnterCriticalSection(&sc);
		}
		inline void Leave() {
			LeaveCriticalSection(&sc);
		}

		//Returns a Instance of Critical Section handler which can be used under a scope for auto Enter and Leaving of a Critical Section
		inline const HCriticalSection& GetSection() {
			return HCriticalSection(sc);
		}

		~HCriticalSectionBase() {
			DeleteCriticalSection(&sc);
		}

	private:
		CRITICAL_SECTION sc;
	};

	class HBaseApp {

	public:
		virtual void start() {

		}

		virtual void Run() {
			MessageProcess();
		}

	protected:

		//Override for Custom task during the IDLE time
		virtual void Idle() {}

		//Default Message Processing
		virtual WPARAM MessageProcess() {
			MSG msg;

			while (true) {
				if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
					if (msg.message == WM_QUIT)
						break;

					TranslateMessage(&msg);
					DispatchMessage(&msg);

				}
				else {
					Idle();
				}
			}

			return msg.wParam;
		}

		HCriticalSectionBase CriticalSection;
	};


	/*
		Thread Local Storage small wrapper for auto cleanup
		Allocates teh Index on construction!
	*/
	class HThreadLocalStorage {
	public:
		HThreadLocalStorage() {
			Index = TlsAlloc();
		}

		void SetValue(LPVOID Data) {
			TlsSetValue(Index, Data);
		}

		LPVOID GetValue() {
			return TlsGetValue(Index);
		}

		~HThreadLocalStorage() {
			TlsFree(Index);
		}

		DWORD Index = 0;
	};

	template<class WinApp>
	class HThread;

	class HBaseThread {
		template<class WinApp>
		friend class HThread;
		/*
			Thread Local Storage works only if one process exists and all the framework works using single process 
			mabe later find some fix for this
		*/
	private:
		HBaseThread(DWORD(WINAPI* func)(LPVOID), bool createNewThread = true) {
			//This creates a new thread and calls the callback function in the new thread
			if (createNewThread)
				ThreadHandle = CreateThread(NULL, NULL, func, NULL, NULL/*CREATE_SUSPENDED*/, NULL);

			//This calls the callback function using the same thread System Reserved!
			else {
				ThreadHandle = GetCurrentThread();
			}

		}
	public:
		void StartThread() {
			::ResumeThread(ThreadHandle);
		}

		void ResumeThread() {
			StartThread();
		}

		void SuspendThread() {
			::SuspendThread(ThreadHandle);
		}
		//protected:
		virtual void Run() {

		}

		~HBaseThread() {
			WaitForSingleObject(ThreadHandle, INFINITE);
			DWORD exitCode;
			GetExitCodeThread(ThreadHandle, &exitCode);

			//WARNING need to refactor it later on when the logic flow is clearer!
			if (exitCode == STILL_ACTIVE) {
				__debugbreak();

				//TODO should i Terminate if somehow the thread doesn't terminate?
				TerminateThread(ThreadHandle, 0);

			}
		}
	protected:
		HANDLE ThreadHandle = nullptr;

		//Is it safe it to be statically constructed?
		static inline HThreadLocalStorage tls;
	};

	extern int WINAPI WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, PSTR CmdLine, int CmdShow);

	//More thn one Instance of this class with same template arguement is not allowed
	template<class WinApp>
	class HThread :public HBaseThread {
		//TODO make the function a friend!
		friend int WINAPI WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, PSTR CmdLine, int CmdShow);

		static_assert(std::is_base_of<HBaseApp, WinApp>::value, "Base class of Template class should only be HBaseApp");
		//Should anything be public?
	public:
		HThread() :HBaseThread(HThread::ThreadFunc) {}


		static DWORD ThreadFunc(LPVOID lPvoid) {
			WinApp App;
			tls.SetValue((LPVOID)&App);
			App.Run();
			//Success!
			return 0;
		}

		static WinApp* GetApp() {
			return (WinApp*)tls.GetValue();
		}

	//private:
		//TODO make this constructor private later!!
		//Framework Reserved for Use in MainThread
		HThread(WinApp* Instance) :HBaseThread(HThread::ThreadFunc, false) {
			tls.SetValue((LPVOID)Instance);
			Instance->Run();

		}

	};

	/*template<class AppName>
	AppName* GetApp() {
		return HThread<AppName>::GetApp
	}*/
}