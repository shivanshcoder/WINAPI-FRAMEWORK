#pragma once

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

	private:

		void DefaultColors();

		std::unordered_map<int, COLORREF>TextColor;
		std::unordered_map<int, COLORREF>TextBgColor;

		HWND LogWindowHandle;
	};
}

