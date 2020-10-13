#include "Paginator.h"

void Paginator::Reset()
{
	vecDiagnostics.clear();
	vecBrackets.clear();
	vecPaginatedText.clear();
	nNumBrackets = 0;
	nMaxDepthNesting = 0;
	ScreenOutputFlag1 = "";
	ScreenOutputFlag2 = "";
	ScreenOutputFlag3 = "";
	ScreenOutputFlag4 = "";
	ScreenOutputFlag5 = "";
	ScreenOutputFlag6 = "";
	ScreenOutputFlag7 = "";
	WriteCounter = 0;
}

void Paginator::Paginator1(std::string StrToPaginate, float SetScreenWidth, float SetScreenHeight)
{

	ScreenWidth = SetScreenWidth;
	ScreenHeight = SetScreenHeight;

	nNumOpeningBrackets = 0;
	nNumClosingBrackets = 0;
	nNumBrackets = 0;
	nDepthBracketNesting = 0;
	nMaxDepthNesting = 0;
	int nPositionInStr = 0;

	//vecDiagnostics.clear();

	// Check For Consistency - actually this is more nuanced because there are other ways
	// the brackets might be inconsisten ]][[ for instance...interesting consideration: 
	// with equal numbers of brackets if you allow wrapping round of the string is there a defined
	// outcome for each bracket, one imagines there would be, how do multiple failures manifest
	// if there are unequal numbers of brackets? Anyway.

	for (char a : StrToPaginate)
	{
		if (a == '[')
		{
			Bracket bbBracket;
			bbBracket.nPosOBrac = nPositionInStr;
			bbBracket.nPosOExpBrac = nPositionInStr + 1;
			bbBracket.nDepthOfNesting = nDepthBracketNesting;
			bbBracket.nId = nNumBrackets;
			nNumOpeningBrackets++;
			nNumBrackets++;
			nDepthBracketNesting++;	//Well we have identified some things
			vecBrackets.push_back(bbBracket);
		}
		if (a == ']')
		{
			nNumClosingBrackets++;
			nDepthBracketNesting--;
		}
		if (nDepthBracketNesting > nMaxDepthNesting)
		{
			nMaxDepthNesting = nDepthBracketNesting;
		}
		nPositionInStr++;
	}

	if (nNumOpeningBrackets != nNumClosingBrackets)
	{
		ScreenOutputFlag1 = "Inconsistent in Brackets Cap'n";
		return;
	}

	// Find Closing Brackets
	for (Bracket& brac : vecBrackets)
	{
		FindClosingBracket(brac, StrToPaginate);
	}

	// Find Position Closing Exponential Bracket
	for (Bracket& brac : vecBrackets)
	{
		FindClosingExponentialBracket(brac, StrToPaginate);
	}

	//Find if new term
	int nTermCount = 0;
	for (Bracket& brac : vecBrackets)
	{
		if (brac.nId != 0)
		{
			if (IsNewTerm(brac, StrToPaginate) == true)
			{
				brac.bNewTerm = true;
				nTermCount++;
				if (IsNegative(brac, StrToPaginate) == true)
				{
					brac.bNegative = true;
				}
			}
		}
	}

	//ScreenOutputFlag5 = "We have " + std::to_string(nTermCount) + " terms Cap'n";

	//Find contained Brackets 
	for (Bracket& bracOuter : vecBrackets)
	{
		// Find enclosedBrackets(brac, StrToPaginate);
		for (Bracket bracInner : vecBrackets)
		{
			if ((bracInner.nPosOBrac > bracOuter.nPosOBrac) and (bracInner.nPosOBrac < bracOuter.nPosCBrac))
				bracOuter.ContainedBrackets.push_back(bracInner.nId);
		}
	}

	ScreenOutputFlag1 = "Brac 0 contains ";
	//ScreenOutputFlag2 = "Brac 1 contains ";

	for (auto a : vecBrackets[0].ContainedBrackets)
		ScreenOutputFlag1 += std::to_string(a) + " ";

	ScreenOutputFlag2 = "Pagination order ";	//This is updated in PaginateExpression


	// Well this is some spaghetti code
	//if (Paginating)
	//{
	//	PaginateExpression(vecBrackets[0], StrToPaginate);
	//	SomethingToDisplay = true;
	//}
	//else
	//{
		//EvaluateExpression(vecBrackets[0], StrToPaginate);
		//EvaluationMade = true;
		//ScreenOutputFlag2 = std::to_string(vecBrackets[0].fEvaluation);
	//}
}

void Paginator::FindClosingBracket(Bracket& ConsideredBracket, std::string StrToPaginate)
{
	std::string StrToPaginateRemainder;

	StrToPaginateRemainder = StrToPaginate.substr(ConsideredBracket.nPosOBrac);

	nDepthBracketNesting = 0;
	int nPositionInStr = 0;
	// So when we hit a closing brackt and our depth of nesting is zero we have found closing bracket

	// If we then check to see if the next character is a ')' we know if we have brackets to display

	for (char a : StrToPaginateRemainder)
	{
		if (a == '[')
		{
			nDepthBracketNesting++;
		}
		if (a == ']')
		{
			nDepthBracketNesting--;
			if (nDepthBracketNesting == 0)
			{
				ConsideredBracket.nPosCBrac = ConsideredBracket.nPosOBrac + nPositionInStr;
				if (ConsideredBracket.nPosCBrac != StrToPaginate.length())
				{
					if (StrToPaginate[ConsideredBracket.nPosCBrac + 1] == ')')
					{
						ConsideredBracket.bBracDisplaysBrackets = true;
					}
				}
				return;
			}
		}
		nPositionInStr++;
	}
}

void Paginator::FindClosingExponentialBracket(Bracket& ConsideredBracket, std::string StrToPaginate)
{
	std::string StrToPaginateRemainder;

	StrToPaginateRemainder = StrToPaginate.substr(ConsideredBracket.nPosOBrac);

	nDepthBracketNesting = 0;
	int nPositionInStr = 0;
	// So when we hit a closing brackt and our depth of nesting is zero we have found closing bracket
	for (char a : StrToPaginateRemainder)
	{
		if (a == '{')
		{
			nDepthBracketNesting++;
		}
		if (a == '}')
		{
			nDepthBracketNesting--;
			if (nDepthBracketNesting == 0)
			{
				ConsideredBracket.nPosCExpBrac = ConsideredBracket.nPosOBrac + nPositionInStr;
				return;
			}
		}
		nPositionInStr++;
	}
}

bool Paginator::IsNewTerm(Bracket& brac, std::string StrToPaginate)
{
	if (StrToPaginate[brac.nPosOBrac - 1] == '+' || StrToPaginate[brac.nPosOBrac - 1] == '-')
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Paginator::IsNegative(Bracket& brac, std::string StrToPaginate)
{
	if (StrToPaginate[brac.nPosOBrac - 1] == '-')
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Paginator::AddBracketSymbols(Bracket& brac, std::string StrToPaginate)
{
	//There is an argument that this should be done procedurally and a procedure definitely exists;
	//however I am inclined to use 'manually' entered brackets as this gives more control over the 
	//final appearance of the expression - and this is after all an exercise in making it look nice.
}

void Paginator::PaginateBracket(Bracket& brac, std::string StrToPaginate)
{
	//std::string BracketStr;
	PixelPadding = 0;
	int Offset = 0;
	int OffsetHold = 0;
	float Smallesty = INFINITY;
	float Largesty = 0;
	bool PushedFirst = false;
	std::string VarString;
	std::string ExponentStr;
	int nLenExpStr;
	int nLenVarStr;
	int bbCounter = 0;

	//Arguably this should be inside the if clause below
	nLenExpStr = brac.nPosCExpBrac - (brac.nPosOExpBrac + 1);
	brac.sExponentialString = StrToPaginate.substr(brac.nPosOExpBrac + 1, nLenExpStr);

	//
	//	Case of a terminal bracket
	//
	if (brac.ContainedBrackets.empty() == true)
	{
		//
		//	Paginate Variable
		//

		nLenVarStr = brac.nPosCBrac - (brac.nPosCExpBrac + 1);
		VarString = StrToPaginate.substr(brac.nPosCExpBrac + 1, nLenVarStr);

		sSpaceText MyVar;
		MyVar.str = VarString;
		MyVar.nFontSize = 2;

		MyVar.x = ScreenWidth/2;
		MyVar.y = ScreenHeight / 2;
		MyVar.dx = -200;
		brac.PaginatedText.push_back(MyVar);
		brac.nAggLenAboveLine += (8 * MyVar.nFontSize * nLenVarStr);
		brac.nTotalHeight = 8 * MyVar.nFontSize + PixelPadding;
		//
		//
		//	Paginate Exponent
		//

		ExponentStr = brac.sExponentialString;

		// Case of special exponents
		if (ExponentStr != "1")
		{
			if (ExponentStr != "-1")
			{
				sSpaceText MyExp;
				MyExp.str = ExponentStr;
				MyExp.nFontSize = 2;

				MyExp.x = (ScreenWidth/2) + brac.nAggLenAboveLine;
				MyExp.y = (ScreenHeight / 2) - (brac.nTotalHeight / 2);
				MyExp.dx = -200;
				brac.PaginatedText.push_back(MyExp);
				brac.nAggLenAboveLine += (8 * MyVar.nFontSize * nLenExpStr);
				brac.nTotalHeight += (brac.nTotalHeight / 2) + PixelPadding;
			}
		}
		brac.nTotalLength = brac.nAggLenAboveLine + 16;	// This should be the case here, will be different for
													// non-terminal brackets
	}

	//
	//	Handle internal paginated brackets
	//

	// What we are trying to do is hold on to the height of the previous bracket
	for (int bracId : brac.ContainedBrackets)
	{
		if (vecBrackets[bracId].bCopied == false)
		{

			Offset = brac.nTotalHeight;

			//Checking for taller brackets
			if (vecBrackets[bracId].nTotalHeight > brac.nTotalHeight)
			{
				Offset = brac.nTotalHeight;
				brac.nTotalHeight = vecBrackets[bracId].nTotalHeight;
			}

			// Is bracket in Exponent?
			if (vecBrackets[bracId].nPosCBrac < brac.nPosCExpBrac)
			{
				//
				// Pre-Paginate Exponent
				//

				if (vecBrackets[bracId].bNewTerm == true)
				{
					if (vecBrackets[bracId].bNegative != true)
					{
						//	Add a plus - ah, at this point we don't know how big the Term is 
						sSpaceText Plus;
						Plus.str = "+";
						Plus.nFontSize = 2;
						Plus.x = (ScreenWidth / 2) + brac.nAggLenAboveLineExp + 4;
						Plus.y = (ScreenHeight / 2); // -vecBrackets[bracId].nTotalHeight / 2;
						Plus.dx = -200;
						brac.PrePaginatedExp.push_back(Plus);
						brac.nAggLenAboveLineExp += 34;
						brac.nAggLenBelowLineExp += 34;
					}
					else
					{
						sSpaceText Minus;
						Minus.str = "-";
						Minus.nFontSize = 2;
						Minus.x = (ScreenWidth / 2) + brac.nAggLenAboveLineExp + 4;
						Minus.y = (ScreenHeight / 2); // -vecBrackets[bracId].nTotalHeight / 2;
						Minus.dx = -200;
						brac.PrePaginatedExp.push_back(Minus);
						brac.nAggLenAboveLineExp += 34;
						brac.nAggLenBelowLineExp += 34;
					}
				}

				if (vecBrackets[bracId].sExponentialString != "-1")
				{
					for (sSpaceText PrePaginated : vecBrackets[bracId].PaginatedText)
					{
						PrePaginated.x += brac.nAggLenAboveLineExp;
						brac.PrePaginatedExp.push_back(PrePaginated);
					}
					brac.nAggLenAboveLineExp += vecBrackets[bracId].nTotalLength;

					//Checking for taller brackets
					if (vecBrackets[bracId].nTotalHeight > brac.nTotalHeight)
					{
						brac.nTotalHeight = vecBrackets[bracId].nTotalHeight;
					}
				}
				else
				{
					Smallesty = INFINITY;
					for (sSpaceText PrePaginated : vecBrackets[bracId].PaginatedText)
					{
						PrePaginated.x += brac.nAggLenBelowLineExp;
						PrePaginated.y += brac.nTotalHeight;
						PrePaginated.y += vecBrackets[bracId].nTotalHeight;
						//if (PrePaginated.y < Smallesty)
						//{
						//	Smallesty = PrePaginated.y;
						//}
						brac.PrePaginatedExp.push_back(PrePaginated);
					}
					
					//sSpaceText MyDivisionBar;
					//MyDivisionBar.nFontSize = 1;
					//bbCounter++;
					//MyDivisionBar.str = std::to_string(bbCounter);
					//MyDivisionBar.bImADivisionBar = true;
					//MyDivisionBar.nHeight = 1; // try that for now.
					//MyDivisionBar.nWidth = vecBrackets[bracId].nTotalLength - 8;
					//MyDivisionBar.x = ScreenWidth + brac.nAggLenAboveLine;
					//MyDivisionBar.y = Smallesty;
					//MyDivisionBar.x = ScreenWidth + brac.nAggLenBelowLine;
					//MyDivisionBar.dx = -200;
					//brac.PrePaginatedExp.push_back(MyDivisionBar);

					//brac.PrePaginatedExp.push_back(MyDivisionBar);

					//brac.nAggLenBelowLineExp += vecBrackets[bracId].nTotalLength;
					//brac.nTotalHeight += vecBrackets[bracId].nTotalHeight;
				}
			}
			else
				//
				// PrePaginate 'Variable' -- this could actually be a variable or a series of terms.
				//
			{
				if (vecBrackets[bracId].bNewTerm == true)
				{
					if (vecBrackets[bracId].bNegative != true)
					{//	Add a plus
						sSpaceText Plus;
						Plus.str = "+";
						Plus.nFontSize = 2;
						Plus.x = (ScreenWidth / 2) + brac.nAggLenAboveLine + 4;
						Plus.y = (ScreenHeight / 2); //+ vecBrackets[bracId].nTotalHeight / 2;
						Plus.dx = -200;
						brac.PaginatedText.push_back(Plus);
						brac.nAggLenAboveLine += 34;
						brac.nAggLenBelowLine += 34;
					}
					else
					{
						sSpaceText Minus;
						Minus.str = "-";
						Minus.nFontSize = 2;
						Minus.x = (ScreenWidth / 2) + brac.nAggLenAboveLine + 4;
						Minus.y = (ScreenHeight / 2); //+ vecBrackets[bracId].nTotalHeight / 2;
						Minus.dx = -200;
						brac.PaginatedText.push_back(Minus);
						brac.nAggLenAboveLine += 34;
						brac.nAggLenBelowLine += 34;
					}
				}

				if (vecBrackets[bracId].sExponentialString != "-1")
				{
					for (sSpaceText PrePaginated : vecBrackets[bracId].PaginatedText)
					{
						PrePaginated.x += brac.nAggLenAboveLine;
						brac.PrePaginatedVar.push_back(PrePaginated);
					}
					brac.nAggLenAboveLine += vecBrackets[bracId].nTotalLength;
				}
				else
				{
					brac.bBracIsSurd = true;
					Smallesty = INFINITY;
					Largesty = 0;
					PushedFirst = false;
					for (sSpaceText& PrePaginated : vecBrackets[bracId].PaginatedText)
					{
						//if (PrePaginated.bImADivisionBar != true)
						//{
							//PrePaginated.y += brac.nTotalHeight;

						PrePaginated.y += vecBrackets[bracId].nTotalHeight +4;
						if (PrePaginated.y < Smallesty)
						{
							Smallesty = PrePaginated.y;
						}
						if (PrePaginated.y > Largesty)
						{
							Largesty = PrePaginated.y;
						}
						//PrePaginated.x += brac.nAggLenBelowLine;
						PrePaginated.x += brac.nAggLenBelowLine;
						//PrePaginated.y += brac.nTotalHeight;
						brac.PrePaginatedVar.push_back(PrePaginated);
						
					}
					sSpaceText MyDivisionBar;
					MyDivisionBar.nFontSize = 1;
					//bbCounter++;
					MyDivisionBar.str = std::to_string(bbCounter);
					MyDivisionBar.bImADivisionBar = true;
					MyDivisionBar.nHeight = 1; // try that for now.
					MyDivisionBar.nWidth = vecBrackets[bracId].nTotalLength - 8;
					//MyDivisionBar.x = ScreenWidth + brac.nAggLenAboveLine;
					MyDivisionBar.y = Smallesty -3;
					MyDivisionBar.x = (ScreenWidth/2) + brac.nAggLenBelowLine;
					MyDivisionBar.dx = -200;
					brac.PrePaginatedVar.push_back(MyDivisionBar);
					//}//Lets Try putting the Divisor bars in here after the full parameters of the brac are calculated.
					//for (sSpaceText& PrePaginated : vecBrackets[bracId].PaginatedText)
					//{

						//if (PrePaginated.y > float(Smallesty))
						//{
							//Lets push back the first value - hoping it is the lowest
							
							//PushedFirst = true;
						//}
					//}
				
				
					
					brac.nTotalHeight += vecBrackets[bracId].nTotalHeight + 4;
					brac.nAggLenBelowLine += vecBrackets[bracId].nTotalLength;
					//brac.nTotalHeight += brac.nTotalHeight + 8;
					//brac.nTotalHeight += vecBrackets[bracId].nTotalHeight + 8;
					
				}
			}

			if (brac.nAggLenAboveLine > brac.nAggLenBelowLine)
			{
				brac.nTotalLength = brac.nAggLenAboveLine;
			}
			else
			{
				brac.nTotalLength = brac.nAggLenBelowLine;
			}

			vecBrackets[bracId].bCopied = true;
		}

	}

	//
	// Paginate variable
	//
	if (brac.bBracIsSurd == false)
	{
		for (sSpaceText PrePaginated : brac.PrePaginatedVar)
		{
			brac.PaginatedText.push_back(PrePaginated);
		}
	}
	else
	{
		for (sSpaceText PrePaginated : brac.PrePaginatedVar)
		{
			//if (PrePaginated.bImADivisionBar == false)
			//{
			//	PrePaginated.y -= brac.nTotalHeight / 4;
			//}
			//else
			//{
			//	PrePaginated.y += brac.nTotalHeight / 2;
			//}
			brac.PaginatedText.push_back(PrePaginated);
		}
	}

	//
	// Paginate Exponent
	//
	for (sSpaceText PrePaginated : brac.PrePaginatedExp)
	{
		WriteCounter++;
		PrePaginated.x += brac.nTotalLength;
		PrePaginated.y -= brac.nTotalHeight / 2;
		brac.PaginatedText.push_back(PrePaginated);
	}

	//brac.nTotalHeight += brac.nTotalHeight / 2;

	if (brac.nAggLenAboveLineExp > brac.nAggLenBelowLineExp)
	{
		brac.nTotalLength += brac.nAggLenAboveLineExp;
	}
	else
	{
		brac.nTotalLength += brac.nAggLenBelowLineExp;
	}

	if (brac.bBracDisplaysBrackets == true)
	{
		sSpaceText MyOpeningBracket;
		
		if (brac.nTotalHeight != NULL)
		{
			MyOpeningBracket.str = std::to_string(brac.nTotalHeight);
		}

		MyOpeningBracket.bImABracket = true;
		MyOpeningBracket.bImAnOpeningBracket = true;
		MyOpeningBracket.nFontSize = 1;
		MyOpeningBracket.nHeight = brac.nTotalHeight; // try that for now.
		MyOpeningBracket.nWidth = 7;
		MyOpeningBracket.y = ScreenHeight/2; //Not entirely sure where to get the value for this
		MyOpeningBracket.x = (ScreenWidth/2);// -(MyOpeningBracket.nWidth* brac.nTotalHeight / 48);
		MyOpeningBracket.dx = -200;

		for (sSpaceText &sText : brac.PaginatedText)
		{
			sText.x += (MyOpeningBracket.nWidth * brac.nTotalHeight /48);
		}

		brac.PaginatedText.push_back(MyOpeningBracket);

		sSpaceText MyClosingBracket;

		if (brac.nTotalHeight != NULL)
		{
			MyClosingBracket.str = std::to_string(brac.nTotalHeight);
		}

		MyClosingBracket.bImABracket = true;
		MyClosingBracket.nFontSize = 1;
		MyClosingBracket.nHeight = brac.nTotalHeight; // try that for now.
		MyClosingBracket.nWidth = 7;
		MyClosingBracket.y = ScreenHeight / 2; //Not entirely sure where to get the value for this
		MyClosingBracket.x = (ScreenWidth/2) + brac.nTotalLength + (MyOpeningBracket.nWidth * brac.nTotalHeight / 48);
		MyClosingBracket.dx = -200;
		brac.PaginatedText.push_back(MyClosingBracket);

		brac.nTotalLength += (MyOpeningBracket.nWidth * brac.nTotalHeight / 24);

		//ScreenOutputFlag4 = "Got here Cap'n brac.nAggLenBelowLine " + std::to_string(brac.nAggLenBelowLine);
	}


	//brac.PrePaginatedVar.push_back(MyDivisionBar);
	//ScreenOutputFlag4 = std::to_string(bbCounter);
}



bool Paginator::PaginateExpression(Bracket& brac, std::string StrToPaginate)
{
	//ALGORITHM
	// So we call this function, it checks to see if there are any unpaginated brackets inside the bracket
	// If there are it calls itself with the contained bracket; it then calls itself with the original bracket;
	// It returns when no unpaginated brackets remain in the bracket.

	// When considering expressions in exponent we would like to see the order of pagination

	LoopCount++;

	bool ContainedBracketsExhausted = true;

	if (brac.ContainedBrackets.empty() == true)
	{
		//ScreenOutputFlag3 = "Got Here";
		ContainedBracketsExhausted = true;
	}
	else
	{
		for (auto& a : brac.ContainedBrackets)
		{

			if (vecBrackets[a].bPaginated == false)
			{
				ContainedBracketsExhausted = false;
			}
		}
	}

	if (ContainedBracketsExhausted == true)
	{
		PaginateBracket(brac, StrToPaginate);
		brac.bPaginated = true;
		ScreenOutputFlag2 += " " + std::to_string(brac.nId);
		return true;
	}
	else
	{
		for (auto& a : brac.ContainedBrackets)
		{
			if (vecBrackets[a].bPaginated == false)
			{
				PaginateExpression(vecBrackets[a], StrToPaginate);
			}
		}
		PaginateExpression(brac, StrToPaginate);
	}
}