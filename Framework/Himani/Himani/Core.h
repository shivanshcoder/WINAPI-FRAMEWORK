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

#define DISABLE_CLASS_COPYCONS_ASSIGNMENT(ClassName)					\
	ClassName(const ClassName&) = delete;								\
	ClassName& operator=(const ClassName&) = delete;

#define DISABLE_CLASS_MOVECONS_ASSIGNMENT(ClassName)					\
	ClassName(ClassName&&) = delete;									\
	ClassName& operator=(ClassName&&) = delete;

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
		HClassInitializer(HWND wnd, bool AllowSelfDestruct) :hwnd(wnd), SelfDestruct(AllowSelfDestruct) {}

		HWND hwnd;
		bool SelfDestruct;
	};

#pragma region ThreadingComp


	/*
	------------------------------------------Threading Helping Components--------------------------------------------------
	*/

	class HEvent {

	public:
		enum Event_Type {
			auto_reset,
			manual_reset
		};

		//Disabled copy Constructors
		DISABLE_CLASS_COPYCONS_ASSIGNMENT(HEvent);

		//TODO implement Move Constructors

		HEvent(Event_Type type = Event_Type::manual_reset) {
			Event = CreateEvent(nullptr, type, false, nullptr);
		}

		HEvent(LPSECURITY_ATTRIBUTES attrs, Event_Type type, bool initialState) {
			Event = CreateEvent(attrs, type, initialState, nullptr);
		}


		void Set() {
			SetEvent(Event);
		}

		void Reset() {
			ResetEvent(Event);
		}

		DWORD Wait(DWORD milliseconds = INFINITE) {
			return WaitForSingleObject(Event, milliseconds);
		}

	private:
		HANDLE Event;
	};

	/*
		Critical Section Wrapper
		A simple wrapper with no overhead and simple functionality
		Just made for Auto Cleanup
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

		//No need for Copy or Move Constructors!
		DISABLE_CLASS_COPYCONS_ASSIGNMENT(HCriticalSectionBase);
		DISABLE_CLASS_MOVECONS_ASSIGNMENT(HCriticalSectionBase);


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
		HBaseApp() = default;
		//No Need of Copy or Move Constructors
		HBaseApp(HBaseApp const&) = delete;
		HBaseApp(HBaseApp&&) = delete;
		HBaseApp& operator=(HBaseApp const&) = delete;
		HBaseApp& operator=(HBaseApp&&) = delete;

		virtual void Start() {
			Run();
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

		DISABLE_CLASS_COPYCONS_ASSIGNMENT(HThreadLocalStorage);
		DISABLE_CLASS_MOVECONS_ASSIGNMENT(HThreadLocalStorage);

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



		friend HBaseApp* GetApp();

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
		DISABLE_CLASS_COPYCONS_ASSIGNMENT(HBaseThread);
		DISABLE_CLASS_MOVECONS_ASSIGNMENT(HBaseThread);

		void StartThread() {
			::ResumeThread(ThreadHandle);
		}

		void ResumeThread() {
			StartThread();
		}

		void SuspendThread() {
			::SuspendThread(ThreadHandle);
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
		//TODO waht happens if more processes are to be used?
		static inline HThreadLocalStorage tls;
	};

	//extern int WINAPI WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, PSTR CmdLine, int CmdShow);
	//More thn one Instance of this class with same template arguement is not allowed

	template<class WinApp>
	class HThread :public HBaseThread {
		//TODO make the function a friend!
		friend int WINAPI::WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, PSTR CmdLine, int CmdShow);

		static_assert(std::is_base_of<HBaseApp, WinApp>::value, "Base class of Template class should only be HBaseApp");

	public:
		HThread() :HBaseThread(HThread::ThreadFunc) {}

		DISABLE_CLASS_COPYCONS_ASSIGNMENT(HThread);
		DISABLE_CLASS_MOVECONS_ASSIGNMENT(HThread);


		static DWORD ThreadFunc(LPVOID lPvoid) {
			WinApp App;
			tls.SetValue((LPVOID)& App);
			App.Start();
			return 0;
		}

		static WinApp* GetApp() {
			return (WinApp*)tls.GetValue();
		}

	private:
		//Framework Reserved for Use in MainThread
		HThread(WinApp* Instance) :HBaseThread(HThread::ThreadFunc, false) {
			tls.SetValue((LPVOID)Instance);
			Instance->Run();
		}

	};

	/*
		Returns The Instance of the Running Application of the current thread of the current process!
	*/
	inline HBaseApp* GetApp() {
		return (HBaseApp*)HBaseThread::tls.GetValue();
	}

	/*
	------------------------------------------------------END---------------------------------------------------------------
	------------------------------------------Threading Helping Components--------------------------------------------------
	*/
#pragma endregion

}