#pragma once

#include <string>
#include <vector>
#include "SpaceText.h"

struct Bracket
{
	// We are calling them Brackets because they are more general and various than just 'variable' brackets
	int nId;
	int nPosOBrac; //Position in StrToPaginate
	int nPosCBrac;
	int nPosOExpBrac;	//Position in StrToPaginate
	int nPosCExpBrac;
	int nDepthOfNesting;	//Exponent brackets will always be nested one deeper
	bool bNewTerm = false;
	bool bNegative = false;
	int nAggLenAboveLine = 0;
	int nAggLenBelowLine = 0;
	int nAggLenAboveLineExp = 0;
	int nAggLenBelowLineExp = 0;
	int nTotalLength = 0;
	int nTotalHeight = 0;
	std::string sExponentialString; //Special cases {1} do nothing {-1} place 'under line'
	bool bBracIsSurd = false;
	bool bBracDisplaysBrackets = false;
	std::vector<sSpaceText> PrePaginatedVar;
	std::vector<sSpaceText> PrePaginatedExp;
	std::vector<sSpaceText> PaginatedText;
	std::vector<int> ContainedBrackets;
	bool bPaginated = false;
	bool bCopied = false;
	bool bEvaluated = false;
	std::vector<float> Terms;
	std::vector<float> TermsInExponent;
	std::vector<float> TermsInVariable;
	float fExponentEvaluation;
	float fVariableEvaluation;
	float fEvaluation;
	sSpaceText stEvaluation;
};