#pragma once
#include<chrono>

namespace LogSystem {
	//TODO
	//Console Type Log
	//Window Type Log
	//FileStream Type Log

	

	//Logs Information to 
	class Log {
	public:
	
		enum LogLevels {
			Normal,
			Moderate,
			High
		};


		virtual void Refresh() {}
		void Push(int Level, std::wstring Log) {
			Entries.push_back({ Level, Log });
		}

	protected:

		struct LogEntries {
			int Level;
			std::wstring Entry;
		};
		
		std::vector<LogEntries>Entries;

	};

	class FileLog :public Log{
	public:
		FileLog(std::wstring __FileName = L"Log.txt")
			:FileName(__FileName) {}

		void Refresh()override;
	private:
		std::wstring FileName;
	};

	class WindowLog :public Log {
	public:
		WindowLog();

		void Refresh()override;
	private:

		void DefaultColors();

		std::unordered_map<int, COLORREF>TextColor;
		std::unordered_map<int, COLORREF>TextBgColor;
		

		void InitWindow();

		static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

		HWND LogWindowHandle;
	
		static RECT  rect;
		static int   cxChar, cyChar;
	};

	class QuickTimeLog {
	public:
		QuickTimeLog(Log &log) 
		:__log(log){
			Start = std::chrono::high_resolution_clock::now();
		}

		~QuickTimeLog() {
			std::wstringstream stream;

			stream << "Operation Completed in " << ( std::chrono::high_resolution_clock::now() - Start).count();

			__log.Push(1, stream.str());
		}
	private:
		Log& __log;
		std::chrono::time_point<std::chrono::high_resolution_clock> Start;
	};
}

