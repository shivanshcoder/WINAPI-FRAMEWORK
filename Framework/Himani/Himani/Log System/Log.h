#pragma once
#include<chrono>


namespace LogSystem {


#ifdef UNICODE
	typedef std::wstring LogString;
	typedef std::wstringstream LogStringStream;
	typedef std::wofstream LogFStream;

#else
	typedef std::string LogString;
	typedef std::stringstream LogStringStream;
	typedef std::ofstream LogFStream;
#endif


	//TODO
	//Console Type Log
	//Window Type Log
	//FileStream Type Log

	//Logs Information 
	class Log {
	public:

		enum LogLevels {
			Normal,
			Moderate,
			High
		};


		virtual void Refresh() {}
		void Push(int Level, LogString Log) {
			Entries.push_back({ Level, Log });
		}

	protected:

		struct LogEntries {
			int Level;
			LogString Entry;
		};

		std::vector<LogEntries>Entries;

	};

	//Logs out to File
	class FileLog :public Log {
	public:
		FileLog(LogString __FileName = L"Log.txt")
			:FileName(__FileName) {}

		void Refresh()override;
	private:
		LogString FileName;
		int lastIndex = 0;
	};

	//Creates a window and logs out on it
	class WindowLog :public Log {
	public:
		WindowLog() {
			InitWindow();
			DefaultColors();
		}

		//Should be called frequently i.e. every tick
		//Dispatches messages for the LogWindow if there are left any or no dispatching is done
		void Refresh()override;


		void PushWinErrors(int ErrorCode = GetLastError());
	private:

		void DefaultColors();

	public:
		std::unordered_map<int, COLORREF>TextColor;
		std::unordered_map<int, COLORREF>TextBgColor;


		void InitWindow();

		static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);


		static RECT  rect;
		static int   cxChar, cyChar;

		//TODO make it private
	public:
		HWND LogWindowHandle;
	};


	//Simply create Instance of this Class in a scope to calculate time length for that scope
	class QuickTimeLog {
	public:
		QuickTimeLog(Log& log)
			:__log(log) {
			Start = std::chrono::high_resolution_clock::now();
		}

		~QuickTimeLog() {
			auto TimeDiff = std::chrono::duration_cast<std::chrono::microseconds>((std::chrono::high_resolution_clock::now() - Start)).count();
			LogStringStream stream;

			stream << "Operation Completed in " << TimeDiff << "MicroSeconds";

			__log.Push(1, stream.str());
		}
	private:
		Log & __log;
		std::chrono::time_point<std::chrono::high_resolution_clock> Start;
	};



	class CustomErrors {

	private:
		std::unordered_map<int, LogString>ErrorList;
	};
}



