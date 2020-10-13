#pragma once

#include <string>
#include <vector>
#include <map>
#include "SpaceText.h"
#include "Bracket.h"
//#include "TitanQuestion.h"
//#include "GenerateVariables.h"


//struct Expression
//{
//	std::vector<Bracket> vecBrackets;
//	std::vector<sSpaceText> vecPaginatedText;
//};


//struct ExponentBracket		//Well this is interesting - not using this presently.
//{
//	int nId;
//	int nPosOBrac;
//	int nPosCBrac;
//	int nDepthOfNesting;
//};

class Paginator
{

public:

	std::string ScreenOutputFlag1 = "";
	std::string ScreenOutputFlag2 = "";
	std::string ScreenOutputFlag3 = "";
	std::string ScreenOutputFlag4 = "";
	std::string ScreenOutputFlag5 = "";
	std::string ScreenOutputFlag6 = "";
	std::string ScreenOutputFlag7 = "";

	int WriteCounter = 0;

	bool SomethingToDisplay = false;
	bool EvaluationMade = false;
	bool bPaused;
	int nNumOpeningBrackets;
	int nNumClosingBrackets;
	int nNumBrackets;
	int nDepthBracketNesting; //Maybe depth of nesting can be counted purely from variable brackets
	int nMaxDepthNesting;
	int LoopCount;
	int AggregateLen;		//We will use an integer for now
	int DummyOffset = 0;	// This is a scaffolding measure.
	int PixelPadding = 0;

	std::string StrToPaginate;
	std::string StrToPaginate1;
	std::string StrToPaginate2;
	std::string StrToPaginate3;
	std::string StrToPaginate4;

	std::string StrToEvaluate;
	std::string StrToEvaluate1;
	std::string StrToEvaluate2;
	std::string StrToEvaluate3;
	std::string StrToEvaluate4;


	std::string IntroText;

	std::vector<Bracket> vecBrackets;
	std::vector<sSpaceText> vecPaginatedText;

	//This is not necessarily the best way of doing this
	//std::vector<sSpaceText> vecDivisionBars;
	//std::vector<sSpaceText> vecDisplayBrackets;

	//std::vector<Expression> vecExpressions;
	//std::vector<std::string> vecInputStr;

	//std::vector<TitanQuestion> Questions;
	int NumQuestions = 0;
	int SelectedQuestion = 0;
	bool DisplayQuestion = false;
	bool DisplayValues = false;

	bool Paginating = true;

	//
	//	TextBox
	//

	int bbTextBoxFont = 1;
	int bbTextHeight = 0;
	//int bbWordLength = 0;
	//int bbLineLength = 0;
	int bbTextBoxWidth = 400;
	std::vector<std::string> vecbbTextBox;

	std::map<std::string, int> mapOfFunctions;
	std::map<std::string, float> mapOfVariables;

	float ScreenWidth;
	float ScreenHeight;

	bool DisplayDiagnostics = false;
	std::vector<std::string> vecDiagnostics;

	//GenVariables bbGenVariables;

	//
	// Methods
	//

	Paginator()
	{

	}

	void Reset();
	void Paginator1(std::string StrToPaginate, float SetScreenWidth, float SetScreenHeight);
	void FindClosingBracket(Bracket& ConsideredBracket, std::string StrToPaginate);
	void FindClosingExponentialBracket(Bracket& ConsideredBracket, std::string StrToPaginate);
	bool IsNewTerm(Bracket& brac, std::string StrToPaginate);
	bool IsNegative(Bracket& brac, std::string StrToPaginate);
	void AddBracketSymbols(Bracket& brac, std::string StrToPaginate);
	void PaginateBracket(Bracket& brac, std::string StrToPaginate);
	bool PaginateExpression(Bracket& brac, std::string StrToPaginate);
}; 