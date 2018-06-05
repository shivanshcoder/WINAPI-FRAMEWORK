#pragma once
#include"Header/Helper.hpp"
#include<memory>
#include<string>
#include<vector>


class Basic_Text {
public:
	Basic_Text (std::shared_ptr<Basic_DC> DC) :DeviceContext (DC) {}
	

protected:
	std::shared_ptr<Basic_DC>DeviceContext;

};

class Text_List :public Basic_Text {
public:



	Text_List (int x, int y, std::shared_ptr<Basic_DC> DC)
		:X (x), Y (y), Basic_Text (DC) {
		TEXTMETRIC  tm;
		GetTextMetrics (*DC, &tm);
		cyChar = tm.tmHeight + tm.tmExternalLeading;
	}

	void Display () {
		for (int i = 0; i < Lines.size (); ++i) {
			TextOut (*DeviceContext, X, Y + cyChar * i, Lines[i].c_str (), Lines[i].size ());
		}
	}
	void AppendText (std::wstring Text) {
		if (Lines.empty ())
			Lines.push_back (L"");
		Lines[Lines.size () - 1].append (Text);
	}

	std::wstring AddSpace (int NumberOfSpaces) {
		std::wstring Spaces;
		for (int i = 0; i < NumberOfSpaces; ++i) {
			Spaces += L" ";
		}

		return std::wstring (Spaces);
	}


	void AddVertPadding (int Padding) {
		cyChar += Padding;
	}

	void AppendLine (std::wstring Line) {
		Lines.push_back (Line);
	}

private:
	int X, Y;
	char cxChar;
	char cxCaps;
	char cyChar;

	std::vector<std::wstring>Lines;

};


class Text_2DList :public Basic_Text {
public:



	Text_2DList (int x, int y, std::shared_ptr<Basic_DC> DC)
		:X (x), Y (y), Basic_Text (DC) {
		TEXTMETRIC  tm;
		GetTextMetrics (*DC, &tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
	}

	void Display () {
		for (int x = 0; x < Lines.size (); ++x) {
			for (int y = 0; y < Lines[x].size (); ++y) {
				TextOut (*DeviceContext, X + (cxCaps *  HorzPadding)* y, Y + (cyChar * VertPadding) * x, Lines[x][y].c_str (), Lines[x][y].size ());
			}
		}
	}
	void AppendText (std::wstring Text) {
		if (Lines.empty ())
			Lines[Lines.size()-1].push_back (L"");
		Lines[Lines.size ()-1].push_back (Text);
	}

	std::wstring AddSpace (int NumberOfSpaces) {	
		std::wstring Spaces;
		for (int i = 0; i < NumberOfSpaces; ++i) {
			Spaces += L" ";
		}

		return std::wstring (Spaces);
	}


	void AddVertGap (int Padding) {
		if (!Padding)
			Padding = 1;
		VertPadding = Padding;
	}

	void AddHorzGap (int Padding) {
		if (!Padding)
			Padding = 1;
		HorzPadding = Padding;
	}

	void AppendLine (std::vector<std::wstring>Line) {
		Lines.push_back (Line);
	}

private:
	int X, Y;
	int8_t cxChar;
	int8_t cxCaps;
	int8_t cyChar;

	int8_t HorzPadding;
	int8_t VertPadding;
	
	std::vector< std::vector<std::wstring> >Lines;;

};

