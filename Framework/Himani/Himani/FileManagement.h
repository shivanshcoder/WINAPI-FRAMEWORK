#pragma once
#include"Core.h"

namespace Himani {

	unsigned long long operator"" MB(unsigned long long num) {
		return  1024 * 1024 * num;
	}
	unsigned long long operator"" GB(unsigned long long num) {
		return  1024 * 1024 * 1024 * num;
	}
	unsigned long long operator"" KB(unsigned long long num) {
		return  1024 * num;
	}

	enum TextFiles_BOM {

		//No BOM marks defaults to UTF8 format!
		NO_BOM_UTF8 = 0,
		//LE_ENDIAN architecture!
		//So the hex values have been reversed!!!


		UTF_8 = 0xBFBBEF,			//	EF BB BF

	//	UTF_32_LE = 0x0000FEFF,		//	FF FE 00 00

	//	UTF_32_BE = 0x0000FFFE,		//	00 00 FE FF

	UTF_16_LE = 0xFEFF,			// 	FF FE

	UTF_16_BE = 0xFFFE,			//	FE FF
	};
	//Reverses a Unicode String
	void ReverseUnicodeStr(TCHAR* pText, uint64_t size) {
		for (int i = 0; i < size / 2; i++)
		{
			BYTE bySwap = ((BYTE*)pText)[2 * i];
			((BYTE*)pText)[2 * i] = ((BYTE*)pText)[2 * i + 1];
			((BYTE*)pText)[2 * i + 1] = bySwap;
		}
	}
	void ReverseUnicodeStr(HBytes& str) {
		BYTE* pText = str.GetPtr();
		for (int i = 0; i < str.Size() / 2; i++)
		{

			BYTE bySwap = ((BYTE*)pText)[2 * i];
			((BYTE*)pText)[2 * i] = ((BYTE*)pText)[2 * i + 1];
			((BYTE*)pText)[2 * i + 1] = bySwap;
		}
	}

	//returns a Unicode string from MultiByteString(UTF8 or ANSI) !
	HBytes GetUnicode(HBytes& str) {
		if (!str.Size())
			return HBytes(1);
		int sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, (LPCCH)str.GetPtr(), -1, NULL, 0);

		//TODO why sizeNeed
		HBytes strTo(sizeNeeded * 2);
		int retVal = MultiByteToWideChar(CP_UTF8, 0, (LPCCH)str.GetPtr(), -1, (LPWSTR)strTo.GetPtr(), strTo.Size());
		if (!retVal)
			__debugbreak();
		return strTo;
	}

	//returns a string in UTF8 format
	HBytes GetUTF8(HBytes& str) {
		if (!str.Size())
			return HBytes(1);

		int s = lstrlen((LPCWSTR)str.GetPtr());
		int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)str.GetPtr(), s, NULL, 0, NULL, NULL);

		HBytes strTo(sizeNeeded);

		WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)str.GetPtr(), (int)str.Size(), (LPSTR)strTo.GetPtr(), sizeNeeded, NULL, NULL);
		//WARNING is it needed?
		//strTo.GetPtr()[sizeNeeded] = '\0';

		return strTo;
	}

	class HFileSystem {
		class HWindow;
		friend TextFiles_BOM CheckForBOM(HFileSystem& fileSys);
	public:

		enum FileOpr {
			Read,
			Write,
			Execute
		};

		//Exact replica of CreateFile
		HFileSystem(PTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
			LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition,
			DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) {
			hFile = CreateFile(lpFileName, dwDesiredAccess, dwShareMode,
				lpSecurityAttributes, dwCreationDisposition,
				dwFlagsAndAttributes, hTemplateFile);

			if (hFile == INVALID_HANDLE_VALUE) {
				HString Error = TEXT("Unable to Create/Open the Specified File");
				throw Exceptions(Error);
			}
			Init();
		}


		HFileSystem(PTSTR fileName, DWORD desiredAccess, DWORD shareMode, DWORD creationDisposition) {
			hFile = CreateFile(fileName, desiredAccess, shareMode, NULL, creationDisposition, 0, NULL);
			if (hFile == INVALID_HANDLE_VALUE) {
				HString Error = TEXT("Unable to Create/Open the Specified File: ");
				Error += fileName;
				throw Exceptions(Error);
			}
			Init();
		}

		void Close() {
			if (hFile) {
				CloseHandle(hFile);
				hFile = nullptr;
			}
		}

		HANDLE Handle() {
			return hFile;
		}

		uint64_t FileSize() {
			return fileSize;
		}

		~HFileSystem() {
			Close();
		}
	protected:

		void Init() {
			LARGE_INTEGER tempFileSize;
			GetFileSizeEx(hFile, &tempFileSize);
			fileSize = tempFileSize.QuadPart;
		}

		uint64_t fileSize;
		HANDLE hFile;
	};



	//Checks for the BOM on the text file!
	TextFiles_BOM CheckForBOM(HFileSystem& fileSys) {
		HBytes tempStore(4);
		DWORD dataRead;


		ReadFile(fileSys.Handle(), tempStore.GetPtr(), 4, &dataRead, NULL);

		int32_t* ptr = (int32_t*)tempStore.GetPtr();

		//TODO do UTF32 encoding or not???

		//if (*ptr == UTF_32_LE) {
		//	return UTF_32_LE;
		//}
		//if (*ptr == UTF_32_BE) { 
		//	return UTF_32_BE;
		//}

		int16_t* ptr2 = (int16_t*)tempStore.GetPtr();

		if (*ptr2 == UTF_16_LE) {
			fileSys.fileSize -= 2;
			SetFilePointer(fileSys.Handle(), 2, NULL, FILE_BEGIN);
			return UTF_16_LE;
		}
		if (*ptr2 == UTF_16_BE) {
			fileSys.fileSize -= 2;
			SetFilePointer(fileSys.Handle(), 2, NULL, FILE_BEGIN);
			return UTF_16_BE;
		}

		if (((*ptr) & 0x00FFFFFF) == UTF_8) {
			fileSys.fileSize -= 3;
			SetFilePointer(fileSys.Handle(), 3, NULL, FILE_BEGIN);
			return UTF_8;
		}

		//restore to original place since no BOM is present!
		SetFilePointer(fileSys.Handle(), 0, NULL, FILE_BEGIN);
		return NO_BOM_UTF8;
	}

	void WriteBOM(HFileSystem& fileSys, TextFiles_BOM fileEncoding) {
		if (fileEncoding == TextFiles_BOM::NO_BOM_UTF8)
			return;
		int32_t BOM_mark = fileEncoding;


		DWORD dataWritten;
		WriteFile(fileSys.Handle(), (&BOM_mark), 4, &dataWritten, NULL);

		switch (BOM_mark)
		{
		case UTF_8:
			SetFilePointer(fileSys.Handle(), 3, 0, FILE_BEGIN);
			break;
		case UTF_16_BE:
		case UTF_16_LE:
			SetFilePointer(fileSys.Handle(), 2, 0, FILE_BEGIN);
			break;
		}
		if (dataWritten != 4) {
			//Throw exception maybe!!
			__debugbreak();
		}

	}

	//Reads the Entire text file and returns the data
	HBytes ReadTextFile(HString fileName, uint64_t FileSizeCap = 1GB) {
		HFileSystem file(&fileName[0], GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING);
		if (file.FileSize() > FileSizeCap) {
			HString Error = TEXT("File Too large to be read!");
			Error += fileName;
			throw Exceptions(Error);
		}

		TextFiles_BOM fileType = CheckForBOM(file);

		DWORD dataRead;
		HBytes dataBuffer(file.FileSize() + 2);
		ReadFile(file.Handle(), dataBuffer.GetPtr(), file.FileSize(), &dataRead, NULL);

		file.Close();

		dataBuffer.GetPtr()[file.FileSize()] = '\0';
		dataBuffer.GetPtr()[file.FileSize() + 1] = '\0';

		HBytes convertedData(0);

		switch (fileType) {
		case TextFiles_BOM::UTF_8:
		case TextFiles_BOM::NO_BOM_UTF8:
#ifndef UNICODE
			//If UNICODE not available simply copy the buffer!
			convertedData = std::move(dataBuffer);
#else
			//convert the data to UNICODE
			convertedData = GetUnicode(dataBuffer);

#endif // !UNICODE
			break;

		case TextFiles_BOM::UTF_16_LE:
			//Incase of Little Endian format for bytes reverse the bytes!
			ReverseUnicodeStr(dataBuffer);

			//Fall through!
		case TextFiles_BOM::UTF_16_BE:


#ifndef UNICODE
			convertedData = GetUTF8(dataBuffer);
#else
			convertedData = std::move(dataBuffer);

#endif // !UNICODE


			break;
		}

		return convertedData;
	}

	/*
	Creates the file or overwrites the existing file!
	Always prefer UTF8 with no BOM mark for storing data
	returns true when successful!
	*/
	bool WriteTextFile(HString fileName, HBytes& dataBuffer, TextFiles_BOM fileEncoding = TextFiles_BOM::NO_BOM_UTF8) {
		HFileSystem file(&fileName[0], GENERIC_WRITE, NULL, CREATE_ALWAYS);


		WriteBOM(file, fileEncoding);

		DWORD dataWritten;
		WriteFile(file.Handle(), dataBuffer.GetPtr(), dataBuffer.Size(), &dataWritten, NULL);
		if (dataWritten != dataBuffer.Size()) {
			//Couldn't write the whole data!!
			return false;
		}
		return true;

	}

	

	//TODO make class which supports streams and other things later on!!
	namespace TODO {
		class TextFileRead :public HFileSystem {
		public:
			//FileSizeCap is in Bytes!
			TextFileRead(PTSTR const fileName, unsigned long long FileSizeCap = 1GB) :HFileSystem(fileName, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING) {
				if (fileSize > FileSizeCap) {
					HString Error = TEXT("File Too large to be read!");
					Error += fileName;
					throw Exceptions(Error);
				}

				DWORD dataRead;

				std::shared_ptr<HBytes>tempStr;

				tempStr = std::make_shared<HBytes>(fileSize + 2);

				//TODO maybe do return val error checking?
				ReadFile(hFile, tempStr->GetPtr(), fileSize, &dataRead, NULL);

				BYTE* ptr = tempStr->GetPtr();
				//Copied from book check for explanation!
				ptr[fileSize] = '\0';
				ptr[fileSize + 1] = '\0';

				//TODO remove!
				Close();

				int UniTest = IS_TEXT_UNICODE_REVERSE_SIGNATURE | IS_TEXT_UNICODE_SIGNATURE;
				if (IsTextUnicode(ptr, fileSize, &UniTest)) {
					// Get past the BOM mark
					BYTE* text = ptr + 2;

					if (UniTest & IS_TEXT_UNICODE_REVERSE_SIGNATURE) {
						ReverseUnicodeStr((TCHAR*)text, fileSize - 2);
					}

#ifndef UNICODE
					ptrData = std::make_shared<HBytes>(fileSize);
					ptrData = GetUTF8(tempStr);
					WideCharToMultiByte(CP_ACP, 0, (PWSTR)text, -1, (LPSTR)ptrData->GetPtr(), fileSize, NULL, NULL);
#else

					lstrcpy((PTSTR)ptrData->GetPtr(), (PTSTR)text);
#endif
				}
				else {
					BYTE* text = ptr;

					ptrData = std::make_shared<HBytes>((1 + fileSize) * 2);

#ifdef UNICODE
					MultiByteToWideChar(CP_ACP, 0, (LPCCH)text, -1, (PTSTR)ptrData->GetPtr(), fileSize + 1);
					//CheckDefaultWinError;
#else
					lstrcpy((PTSTR)ptrData->GetPtr(), (PTSTR)text);
#endif


				}
				//OutputDebugStringA(GetUTF8(*ptrData).c_str());
				OutputDebugStringA("HELLO");
				//		OutputDebugString((LPCWSTR)ptrData->GetPtr());
						//Is Protection needed?
						/*if (!ptrData) {

							throw Exceptions(TEXT("Not able to Allocate data for reading!"));
						}*/
			}


			std::shared_ptr<HBytes> TakeData() {
				return ptrData;
			}


		private:
			//VERSION
			std::shared_ptr<HBytes>ptrData;

		};
	}


}