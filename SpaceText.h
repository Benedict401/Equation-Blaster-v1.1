#pragma once

#include <string>

struct sSpaceText
{
	float x;
	float y;
	float dx;
	float dy;
	int nFontSize;
	int nHeight;
	int nWidth;
	bool bImABracket = false;
	bool bImAnOpeningBracket = false;
	bool bImADivisionBar = false;
	std::string str;
};