#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "Paginator.h"
//#include <iostream>
#include <random>

// APPLICATION STARTS HERE
//#include "Paginator.h"


// Plan
//
// We want Bombs these will have a vecModel and also bounding boxes for the purpopses of collision detection
// What we want know is if a non-regular convex polygon will work with our collision detection
// Also we want to drop a bomb! - Implement the display as a space object for now

class EquationBlaster : public olc::PixelGameEngine
{
public:
	EquationBlaster()
	{
		sAppName = "EquationBlaster V1.1";
	}

	//int bbLevel = 1;

private:
	olc::Sprite* sprDemo = nullptr;

	struct Variable {

		Variable(std::string ini_Name, int ini_Power) {
			Name = ini_Name; Power = ini_Power;
		}

		std::string Name;
		int Power;
	};

	struct Term {

		std::vector<Variable> vecVariable;
		std::vector<Variable> vecNumerator;
		std::vector<Variable> vecDenominator;
		int Coefficient; //We will keep this as an integer for now
		bool Aggregated = false;
	};

	struct SideOfEquation {
		std::vector<Term> vecTerm;
		std::string SideOfEquationStr;
		std::string Name;
	};

	// Right so we want an aggregate function that operates on SideOfEquation 
	// This function will remove terms after aggregating the co-efficients of like terms
	// In the case of two coefficients cancelling both terms are removed.

	struct Equation {
		std::string Name;
		SideOfEquation LHS;
		SideOfEquation RHS;
	};

	struct Logarithm {

		Logarithm() {}

		Logarithm (long ini_Base, long ini_Log, long ini_AntiLog, bool ini_Achieved) {
			Base = ini_Base; Log = ini_Log; AntiLog = ini_AntiLog; Achieved = ini_Achieved;
		}

		long Base;
		long Log;
		long AntiLog;
		bool Achieved;
	};

	struct sSpaceObject {
		sSpaceObject(olc::vf2d ini_pos, olc::vf2d ini_vel, int ini_nSize, float ini_angle) {
			pos = ini_pos; vel = ini_vel; nSize = ini_nSize; angle = ini_angle;
		}

		sSpaceObject(float ini_pos_x, float ini_pos_y, float ini_vel_x, float ini_vel_y, int ini_nSize) {
			pos.x = ini_pos_x; pos.y = ini_pos_y; vel.x = ini_vel_x; vel.y = ini_vel_y; angle = ini_nSize;
		}

		sSpaceObject(float ini_pos_x, float ini_pos_y, float ini_vel_x, float ini_vel_y, int ini_nSize, float ini_angle) {
			pos.x = ini_pos_x; pos.y = ini_pos_y; vel.x = ini_vel_x; vel.y = ini_vel_y; nSize = ini_nSize; angle = ini_angle;
		}

		sSpaceObject(float ini_pos_x, float ini_pos_y, float ini_vel_x, float ini_vel_y, int ini_nSize, float ini_angle, std::string str) {
			pos.x = ini_pos_x; pos.y = ini_pos_y; vel.x = ini_vel_x; vel.y = ini_vel_y; nSize = ini_nSize; angle = ini_angle; OpStr = str;
		}

		olc::vf2d pos;
		olc::vf2d vel;
		int nSize;
		float angle;
		// For now we will put it in here, not an awful thing to do 
		std::string OpStr;
	};

	//A little bit of hacking here

	struct vec2d {
		float x;
		float y;
	};

	struct polygon {
		std::vector<vec2d> p;	// Transformed Points
		vec2d pos;				// Position of shape
		float angle;			// Direction of shape
		std::vector<vec2d> o;	// "Model" of shape							
		bool overlap = false;	// Flag to indicate if overlap has occurred
	};


	std::string ScreenOutputFlag1 = "Not there yet";
	std::string ScreenOutputFlag2 = "Not there yet";
	std::string ScreenOutputFlag3 = "Cap'n";
	std::string ScreenOutputFlag4 = "Not there yet";
	std::string ScreenOutputFlag5 = "Not there yet";
	std::string ScreenOutputFlag6 = "Not there yet";

	bool InMenu = true;
	bool InMainMenu = true;
	int nMenuSelect = 0;
	int DisplaySwitch = 0;
	std::string MenuDisplay = "";
	std::vector<std::string> Licence;
	int numLines = 1;
	bool DoingLogarithms = false;
	bool DroppingTerms = false;
	bool MultiplicationDivision = false;
	std::string OperationsMaster = "abcdefghijklmnopqrstuvwyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	std::vector<std::string> vecChosenVariables;

	Equation bbEquationPlayer1;
	Equation bbEquationPlayer2;
	Equation bbEquationSolution;
	std::vector<std::string> vecCorrectOrderOfOperations;
	Logarithm TriangleLogarithm;
	int bbAsteroidLevel = 1;
	int bbEquationLevel = 1;
	int LoadedEquationLevels = 8;
	int bbLogarithmLevel = 1;
	int MaxTerms = 3;
	int MaxComplexity = 100;
	int Complexity = 0;
	std::vector<std::string> vecAppropriateVariables;
	std::vector<std::string> vecAppropriateVariablesCycle;
	std::vector<Logarithm> vecAppropriateLogarithms; //This probably wants to be called something else
	std::vector<std::string> vecAppropriateBases;
	std::vector<std::string> vecAppropriateLogarithmsCycle; //This probably wants to be called something else
	std::vector<int> vecLogs;
	std::string sLevelInstruction = "";
	std::string sCongratulations = "";
	bool LevelComplete = false;
	bool InReverse = false;
	std::vector<std::string> vecConductedOperations;
	bool BetweenEquationLevels = false;	// Question of wether we want separate variables for different level types.
	std::string bbTestStr = "123456789112345678921234567893123456789412345678951234567896";
	int bbStrLen = 0;
	float bbRnd = 0;
	std::string bbOpStr = "";
	int bbi = 0;
	int AvailableBoosts = 0;
	int AvailableSpaceAnchorsPlayer1 = 0;
	int AvailableSpaceAnchorsPlayer2 = 0;

	std::vector<sSpaceObject> vecAsteroids;
	std::vector<sSpaceObject> vecBullets;
	std::vector<sSpaceObject> vecOperations;
	std::vector<sSpaceObject> vecSpaceAnchors;
	std::vector<sSpaceObject> vecTicks;
	std::vector<sSpaceObject> vecBombs;
	std::vector<sSpaceObject> vecBouncingMessages;

	std::vector<sSpaceObject> vecPlayers;


	int nScore = 0;
	bool bDead = false;
	float fScale = 3;
	float fScaleLogarithmTriangle = 10;
	float AsteroidSpeed = 20;
	int nScale = 3;
	float fOperationStrOffsetx = 10;	//We can do this precisely now we know the dimensions of strs
	float fOperationStrOffsety = 5;
	float Pi = 3.1416f;
	std::string PresentOperation = "";
	std::string PresentOperationVariable = "";
	std::string RequiredOpStr;
	float fAccruedTime;

	std::vector<std::pair<float, float>> vecModelShip;
	std::vector<std::pair<float, float>> vecModelAsteroid;
	std::vector<std::pair<float, float>> vecModelSpaceAnchor;
	std::vector<std::pair<float, float>> vecModelTick;
	std::vector<std::pair<float, float>> vecModelLogarithmTriangle;
	std::vector<std::pair<float, float>> vecModelBomb;
	std::vector<std::pair<float, float>> vecBoundingBoxBomb;

	polygon BoundingBoxBomb;
	polygon Ship;

	Paginator PaginatorPlayer1LHS; 
	Paginator PaginatorPlayer1RHS;

	Paginator PaginatorPlayer2LHS;
	Paginator PaginatorPlayer2RHS;

	float LHSxOffset;
	float LHSyOffset;
	float RHSxOffset;
	float RHSyOffset;

public:
	bool OnUserCreate() override
	{
		//olc::vf2d Piss = { 20.0f, 20.0f };

		vecAsteroids.emplace_back(20.0f, 20.0f, 8.0f, -6.0f, 16, 0.0f); //Why are we doing this again?   

		// Initialise Equations
		bbEquationPlayer1.Name = "Player1";
		bbEquationPlayer1.LHS.Name = "LHS";
		bbEquationPlayer1.RHS.Name = "RHS";
		bbEquationPlayer2.Name = "Player2";
		bbEquationPlayer2.LHS.Name = "LHS";
		bbEquationPlayer2.RHS.Name = "RHS";
		bbEquationSolution.Name = "Solution";
		bbEquationSolution.LHS.Name = "LHS";
		bbEquationSolution.RHS.Name = "RHS";
			
		//Initialise Players - we will worry about this later
		vecPlayers.emplace_back(ScreenWidth() / 2.0f, ScreenHeight() / 2.0, 0.0f, 0.0f, 0.0f, 0.0f);
		//vecPlayers.emplace_back(ScreenWidth() / 3.0f, ScreenHeight() / 2.0f, 0.0f, 0.0f, 0.0f , 0.0f);
		//vecPlayers.emplace_back(2 * ScreenWidth() / 3.0f, 2 * ScreenHeight() / 2.0f, 0.0f, 0.0f, 0.0f, 0.0f);

		vecModelShip = {
			{fScale * 0.0f, fScale * -5.0f},
			{fScale * -2.5f, fScale * +2.5f},
			{fScale * +2.5f, fScale * +2.5f}
		}; // A simple Isoceles Triangle 

		vec2d Initialiser = { 10,10 };
		Ship.p.push_back(Initialiser);

		// Construct Asteroid
		int verts = 20;
		for (int i = 0; i < verts; i++) {
			float radius = (float)rand() / (float)RAND_MAX * fScale * 0.4f + fScale * 0.8f;
			float a = ((float)i / (float)verts) * 6.28318f;
			vecModelAsteroid.push_back(std::make_pair(radius * sinf(a), radius * cosf(a)));
		}

		vecModelSpaceAnchor = {
			{ -0.5f,  2.0f},
			{ -0.75f, 1.75f},
			{ -0.5f,  1.5f},
			{ -0.5f, 1.0f },
			{ -0.75f, 1.0f},
			{ -0.75f, 0.5f},
			{ -0.5f, 0.5f},
			{ -0.5f, -2.0f},
			{ -0.75f, -1.75f},
			{ -1.0f, -1.0f},
			{ -1.25f, -1.25f},
			{ -1.0f, -2.25f},
			{ -0.5f, -2.5f},
			{  0.5f, -2.5f},
			{  1.0f, -2.25f},
			{ 1.25f, -1.25f},
			{ 1.0f, -1.0f},
			{ 0.75f, -1.75f},
			{ 0.5f, -2.0f},
			{ 0.5f, 0.5f},
			{ 0.75f, 0.5f },
			{ 0.75f, 1.0f},
			{ 0.5f, 1.0f},
			{ 0.5f, 1.5f},
			{ 0.75f, 1.75f},
			{ 0.5f, 2.0f}
		};

		vecModelTick = {
			{2.0f,-2.0f},
			{(float)(2 + 1/sqrt(2)), float(-(1 + 1/sqrt(2)))},
			{-1.0f, 2.0f},
			{(float)(-(2 + 1/sqrt(2))),(float)( 1/sqrt(2))},
			{-2.0f,0.0f},
			{-1.0f,1.0f}
		};

		vecModelBomb = {
			{1.0f,-2.0f},
			{1.0f,-1.8f},
			{0.8f,-1.8f},
			{0.8f,-1.6f},
			{1.0f,-1.6f},
			{1.0f, 1.6f},
			{(float) 2/3,1.8f},
			{0.25f,2.0f},
			{-0.25f,2.f}, 
			{(float) -2/3,1.8f},
			{-1.0f,1.6f},
			{-1.0f,-1.6f},
			{-0.8f,-1.6f},
			{-0.8f,-1.8f},
			{-1.0f,-1.8f},
			{-1.0f,-2.0f}
		};

		vecBoundingBoxBomb = {
			{1.0f,-2.0f},
			{1.0f, 2.0f},
			{-1.0f, 2.0f},
			{-1.0f, -2.0f}
		};

		vecModelLogarithmTriangle = {
			{fScaleLogarithmTriangle * 0.0f, (float)(fScaleLogarithmTriangle * -sqrt(3)/2)},
			{fScaleLogarithmTriangle * -1.0f, (float)(fScaleLogarithmTriangle * sqrt(3)/2)},
			{fScaleLogarithmTriangle * 1.0f, (float)(fScaleLogarithmTriangle * sqrt(3)/2)}
		};

		ResetGame();

		return true;

	}


	bool IsPointInsideCircle(float cx, float cy, float radius, float x, float y) {
		return sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy)) < fScale * radius;
	}

	void LoadAsteroidLevel(int CurrentAsteroidLevel)
	{

		//Initialise Player Position
		vecPlayers[0].pos.x = ScreenWidth() / 2.0f;
		vecPlayers[0].pos.y = ScreenHeight() / 2.0f;
		vecPlayers[0].vel.x = 0.0f;
		vecPlayers[0].vel.y = 0.0f;
		vecPlayers[0].angle = 0.0f;

		//Clear bullets - maybe also clear operations - tomorrow implement asteroid levels and Equation Level
		vecBullets.clear();

		//bbEquation.LHS.vecTerm.push_back(Term1);
		//bbEquation.LHS.vecTerm.clear();

		//int bbInt = bbEquation.LHS.vecTerm.capacity();

		switch (CurrentAsteroidLevel)
		{
		case 1:
			AsteroidSpeed = 20;
			vecAsteroids.emplace_back( 220.0f, 220.0f, 8.0f, -6.0f, (int)16, 0.0f );
			vecAsteroids.emplace_back( 900.0f, 300.0f, -5.0f, 3.0f, (int)16, 0.0f );
			//vecAsteroids.push_back({ 220.0f, 220.0f, 8.0f, -6.0f, (int)16, 0.0f });
			//vecAsteroids.push_back({ 900.0f, 300.0f, -5.0f, 3.0f, (int)16, 0.0f });
			break;
		case 2:
			AsteroidSpeed = 30;
			vecAsteroids.emplace_back( 150.0f, 600.0f, 8.0f, -6.0f, (int)16, 0.0f );
			vecAsteroids.emplace_back( 260.0f, 120.0f, -5.0f, 3.0f, (int)16, 0.0f );
			vecAsteroids.emplace_back( 500.0f, 220.0f, -7.0f, 7.0f, (int)16, 0.0f );
			break;
		case 3:
			AsteroidSpeed = 40;
			vecAsteroids.emplace_back( 150.0f, 600.0f, 8.0f, -6.0f, (int)16, 0.0f );
			vecAsteroids.emplace_back( 260.0f, 120.0f, -5.0f, 3.0f, (int)16, 0.0f );
			vecAsteroids.emplace_back( 500.0f, 220.0f, -7.0f, 7.0f, (int)16, 0.0f );
			break;
		default:
			AsteroidSpeed = 10;
			vecAsteroids.emplace_back( 220.0f, 220.0f, 8.0f, -6.0f, (int)16, 0.0f );
			vecAsteroids.emplace_back( 900.0f, 300.0f, -5.0f, 3.0f, (int)16, 0.0f );
			;
		}
	}

	void LoadEquationLevel(int CurrentEquationLevel, bool Reload) {
		//Initialise	
		vecOperations.clear();
		vecTicks.clear();
		LevelComplete = false;
		BetweenEquationLevels = false;

		vecAppropriateVariables.clear();
		vecAppropriateVariablesCycle.clear();

		switch (CurrentEquationLevel) {
		case 1: // a = b + c
			MaxTerms = 6;
			if (!Reload) ChooseVariables(3);

			sLevelInstruction = "Find " + vecChosenVariables[1];
			sCongratulations = vecChosenVariables[1] + " found!";

			LoadEquationStart(bbEquationPlayer1, CurrentEquationLevel);
			LoadEquationStart(bbEquationPlayer2, CurrentEquationLevel);
			LoadEquationSolution(bbEquationSolution, CurrentEquationLevel);

			// Appropriate Operations
			vecAppropriateVariables.push_back("-" + vecChosenVariables[2]);
			vecAppropriateVariables.push_back("+" + vecChosenVariables[2]);

			break;
		case 2: // e = f - g
			MaxTerms = 6;
			if (!Reload) ChooseVariables(3);

			sLevelInstruction = "Find " + vecChosenVariables[1];
			sCongratulations = vecChosenVariables[1] + " found!";

			LoadEquationStart(bbEquationPlayer1, CurrentEquationLevel);
			LoadEquationStart(bbEquationPlayer2, CurrentEquationLevel);
			LoadEquationSolution(bbEquationSolution, CurrentEquationLevel);

			// Appropriate Operations
			vecAppropriateVariables.push_back("-" + vecChosenVariables[2]);
			vecAppropriateVariables.push_back("+" + vecChosenVariables[2]);

			break;
		case 3: // h = G/k
			MaxTerms = 4;
			if (!Reload) ChooseVariables(3);

			sLevelInstruction = "Find " + vecChosenVariables[1];
			sCongratulations = vecChosenVariables[1] + " found!";

			LoadEquationStart(bbEquationPlayer1, CurrentEquationLevel);
			LoadEquationStart(bbEquationPlayer2, CurrentEquationLevel);
			LoadEquationSolution(bbEquationSolution, CurrentEquationLevel);

			// Appropriate operations
			vecAppropriateVariables.push_back("x" + vecChosenVariables[2]);
			vecAppropriateVariables.push_back("%" + vecChosenVariables[2]);

			break;
		case 4: // m = pq
			MaxTerms = 4;
			if (!Reload) ChooseVariables(3);
			
			sLevelInstruction = "Find " + vecChosenVariables[1];
			sCongratulations = vecChosenVariables[1] + " found!";

			LoadEquationStart(bbEquationPlayer1, CurrentEquationLevel);
			LoadEquationStart(bbEquationPlayer2, CurrentEquationLevel);
			LoadEquationSolution(bbEquationSolution, CurrentEquationLevel);

			// Appropriate Operations
			vecAppropriateVariables.push_back("x" + vecChosenVariables[2]);
			vecAppropriateVariables.push_back("%" + vecChosenVariables[2]);

			break;
		case 5: // W = o + A + L
			MaxTerms = 6;
			if (!Reload) ChooseVariables(4);
			
			sLevelInstruction = "Find " + vecChosenVariables[1];
			sCongratulations = vecChosenVariables[1] + " found!";

			LoadEquationStart(bbEquationPlayer1, CurrentEquationLevel);
			LoadEquationStart(bbEquationPlayer2, CurrentEquationLevel);
			LoadEquationSolution(bbEquationSolution, CurrentEquationLevel);

			// Appropriate Operations
			vecAppropriateVariables.push_back("-" + vecChosenVariables[2]);
			vecAppropriateVariables.push_back("+" + vecChosenVariables[2]);
			vecAppropriateVariables.push_back("-" + vecChosenVariables[3]);
			vecAppropriateVariables.push_back("+" + vecChosenVariables[3]);
			break;
		case 6: // H = f - g + D
			MaxTerms = 6;
			if (!Reload) ChooseVariables(4);
		
			sLevelInstruction = "Find " + vecChosenVariables[2];
			sCongratulations = vecChosenVariables[2] + " found!";

			LoadEquationStart(bbEquationPlayer1, CurrentEquationLevel);
			LoadEquationStart(bbEquationPlayer2, CurrentEquationLevel);
			LoadEquationSolution(bbEquationSolution, CurrentEquationLevel);

			// Appropriate Operations
			vecAppropriateVariables.push_back("+" + vecChosenVariables[0]);
			vecAppropriateVariables.push_back("-" + vecChosenVariables[0]);
			vecAppropriateVariables.push_back("+" + vecChosenVariables[2]);
			vecAppropriateVariables.push_back("-" + vecChosenVariables[2]);

			break;
		case 7:
			MaxTerms = 6;
			if (!Reload) ChooseVariables(5);

			sLevelInstruction = "Find " + vecChosenVariables[0];
			sCongratulations = vecChosenVariables[0] + " found!";

			LoadEquationStart(bbEquationPlayer1, CurrentEquationLevel);
			LoadEquationStart(bbEquationPlayer2, CurrentEquationLevel);
			LoadEquationSolution(bbEquationSolution, CurrentEquationLevel);

			// Appropriate Operations
			vecAppropriateVariables.push_back("%" + vecChosenVariables[1]);
			vecAppropriateVariables.push_back("x" + vecChosenVariables[1]);

			break;
		case 8:
			MaxTerms = 6;
			if (!Reload) ChooseVariables(4);

			sLevelInstruction = "Find " + vecChosenVariables[1];
			sCongratulations = vecChosenVariables[1] + " found!";

			LoadEquationStart(bbEquationPlayer1, CurrentEquationLevel);
			LoadEquationStart(bbEquationPlayer2, CurrentEquationLevel);
			LoadEquationSolution(bbEquationSolution, CurrentEquationLevel);

			// Appropriate Operations
			vecAppropriateVariables.push_back("+" + vecChosenVariables[3]);
			vecAppropriateVariables.push_back("-" + vecChosenVariables[3]);
			vecAppropriateVariables.push_back("x" + vecChosenVariables[2]);
			vecAppropriateVariables.push_back("%" + vecChosenVariables[2]);

			break;

		}
	}

	void LoadEquationStart(Equation& bbEquation, int CurrentEquationLevel)
	{

		//So here we load in starting state of Equation
		bbEquation.LHS.vecTerm.clear();
		bbEquation.RHS.vecTerm.clear();

		Term Term1;	

		switch (CurrentEquationLevel) {
		case 1: // a = b + c
			// bbEquation LHS
			Term1.vecVariable.emplace_back( vecChosenVariables[0], 1 );
			Term1.Coefficient = 1;
			bbEquation.LHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();

			// bbEquation RHS
			Term1.vecVariable.emplace_back( vecChosenVariables[1], 1 );
			Term1.Coefficient = 1;
			bbEquation.RHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			Term1.vecVariable.emplace_back( vecChosenVariables[2], 1 );
			Term1.Coefficient = 1;
			bbEquation.RHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();

			break;
		case 2: // e = f - g
			
			// bbEquation
			Term1.vecVariable.emplace_back( vecChosenVariables[0], 1 );
			Term1.Coefficient = 1;
			bbEquation.LHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();

			Term1.vecVariable.emplace_back( vecChosenVariables[1], 1 );
			Term1.Coefficient = 1;
			bbEquation.RHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			Term1.vecVariable.emplace_back( vecChosenVariables[2], 1 );
			Term1.Coefficient = -1;
			bbEquation.RHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();

			break;
		case 3: // h = G/k

		// bbEquation
			Term1.vecVariable.emplace_back( vecChosenVariables[0], 1 );
			Term1.Coefficient = 1;
			bbEquation.LHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();

			Term1.vecVariable.emplace_back( vecChosenVariables[1], 1 );
			Term1.vecVariable.emplace_back( vecChosenVariables[2], -1 );
			Term1.Coefficient = 1;
			bbEquation.RHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();

			break;
		case 4: // m = pq
		// bbEquation
			Term1.vecVariable.emplace_back( vecChosenVariables[0], 1 );
			Term1.Coefficient = 1;
			bbEquation.LHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();

			Term1.vecVariable.emplace_back( vecChosenVariables[1], 1 );
			Term1.vecVariable.emplace_back( vecChosenVariables[2], 1 );
			Term1.Coefficient = 1;
			bbEquation.RHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();

			break;
		case 5: // W = o + A + L
			// bbEquation
			Term1.vecVariable.emplace_back( vecChosenVariables[0], 1 );
			Term1.Coefficient = 1;
			bbEquation.LHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();

			Term1.vecVariable.emplace_back( vecChosenVariables[1], 1 );
			Term1.Coefficient = 1;
			bbEquation.RHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			Term1.vecVariable.emplace_back( vecChosenVariables[2], 1 );
			Term1.Coefficient = 1;
			bbEquation.RHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			Term1.vecVariable.emplace_back( vecChosenVariables[3], 1 );
			Term1.Coefficient = 1;
			bbEquation.RHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();

			break;
		case 6: // H = f - g + D
			// bbEquation
			Term1.vecVariable.emplace_back( vecChosenVariables[0], 1 );
			Term1.Coefficient = 1;
			bbEquation.LHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();

			Term1.vecVariable.emplace_back( vecChosenVariables[1], 1 );
			Term1.Coefficient = 1;
			bbEquation.RHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			Term1.vecVariable.emplace_back( vecChosenVariables[2], 1 );
			Term1.Coefficient = -1;
			bbEquation.RHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			Term1.vecVariable.emplace_back( vecChosenVariables[3], 1 );
			Term1.Coefficient = 1;
			bbEquation.RHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();

			break;
		case 7: 
			// bbEquation  ab = c + d + e
			Term1.vecVariable.emplace_back( vecChosenVariables[0], 1 );
			Term1.vecVariable.emplace_back( vecChosenVariables[1], 1 );
			Term1.Coefficient = 1;
			bbEquation.LHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();

			Term1.vecVariable.emplace_back( vecChosenVariables[2], 1 );
			Term1.Coefficient = 1;
			bbEquation.RHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			Term1.vecVariable.emplace_back( vecChosenVariables[3], 1 );
			Term1.Coefficient = 1;
			bbEquation.RHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			Term1.vecVariable.emplace_back( vecChosenVariables[4], 1 );
			Term1.Coefficient = 1;
			bbEquation.RHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();

			break;
		case 8:
			// bbEquation y = mx + c
			Term1.vecVariable.emplace_back( vecChosenVariables[0], 1 );
			Term1.Coefficient = 1;
			bbEquation.LHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();

			Term1.vecVariable.emplace_back( vecChosenVariables[1], 1 );
			Term1.vecVariable.emplace_back( vecChosenVariables[2], 1 );
			Term1.Coefficient = 1;
			bbEquation.RHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			Term1.vecVariable.emplace_back( vecChosenVariables[3], 1 );
			Term1.Coefficient = 1;
			bbEquation.RHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();

			break;
		
		default:
			;
			/*
			bbEquationLevel = 1;
			DroppingTerms = false;
			MultiplicationDivision = true;
			sLevelInstruction = "Find m";
			sCongratulations = "m Found!";
			//We need a suitable way to end the game
			Term1.vecVariable.push_back({ "D", 1 });
			Term1.Coefficient = 1;
			bbEquation.LHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			Term1.vecVariable.push_back({ "m", 1 });
			Term1.vecVariable.push_back({ "V", -1 });
			Term1.Coefficient = 1; 
			bbEquation.RHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			//bbEquationSolution
			Term1.vecVariable.push_back({ "D", 1 });
			Term1.vecVariable.push_back({ "V", 1 });
			Term1.Coefficient = 1;
			bbEquationSolution.LHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			Term1.vecVariable.push_back({ "m", 1 });
			Term1.Coefficient = 1;
			bbEquationSolution.RHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			//AppropriateVariables
			//vecAppropriateVariables.push_back({ "D"});
			//vecAppropriateVariables.push_back({ "m"});
			vecAppropriateVariables.push_back( "xV");
			vecAppropriateVariables.push_back( "%V"); 
			*/
		}
	}

	void LoadEquationSolution(Equation& bbEquationSolution, int CurrentEquationLevel)
	{
		bbEquationSolution.LHS.vecTerm.clear();
		bbEquationSolution.RHS.vecTerm.clear();

		Term Term1;
		switch (CurrentEquationLevel) {
		case 1: // a = b + c
			// bbEquationSolution
			Term1.vecVariable.emplace_back( vecChosenVariables[0], 1 );
			Term1.Coefficient = 1;
			bbEquationSolution.LHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			Term1.vecVariable.emplace_back( vecChosenVariables[2], 1 );
			Term1.Coefficient = -1;
			bbEquationSolution.LHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();

			Term1.vecVariable.emplace_back( vecChosenVariables[1], 1 );
			Term1.Coefficient = 1;
			bbEquationSolution.RHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			break;
		case 2:
			// bbEquationSolution
			Term1.vecVariable.emplace_back( vecChosenVariables[0], 1 );
			Term1.Coefficient = 1;
			bbEquationSolution.LHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			Term1.vecVariable.emplace_back( vecChosenVariables[2], 1 );
			Term1.Coefficient = 1;
			bbEquationSolution.LHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();

			Term1.vecVariable.emplace_back( vecChosenVariables[1], 1 );
			Term1.Coefficient = 1;
			bbEquationSolution.RHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			break;
		case 3:
			// bbEquationSolution
			Term1.vecVariable.emplace_back( vecChosenVariables[0], 1 );
			Term1.vecVariable.emplace_back( vecChosenVariables[2], 1 );
			Term1.Coefficient = 1;
			bbEquationSolution.LHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();

			Term1.vecVariable.emplace_back( vecChosenVariables[1], 1 );
			Term1.Coefficient = 1;
			bbEquationSolution.RHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			break;
		case 4:
			// bbEquationSolution
			Term1.vecVariable.emplace_back( vecChosenVariables[0], 1 );
			Term1.vecVariable.emplace_back( vecChosenVariables[2], -1 );
			Term1.Coefficient = 1;
			bbEquationSolution.LHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();

			Term1.vecVariable.emplace_back( vecChosenVariables[1], 1 );
			Term1.Coefficient = 1;
			bbEquationSolution.RHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			break;
		case 5:
			// bbEquationSolution
			Term1.vecVariable.emplace_back( vecChosenVariables[0], 1 );
			Term1.Coefficient = 1;
			bbEquationSolution.LHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			Term1.vecVariable.emplace_back( vecChosenVariables[2], 1 );
			Term1.Coefficient = -1;
			bbEquationSolution.LHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			Term1.vecVariable.emplace_back( vecChosenVariables[3], 1 );
			Term1.Coefficient = -1;
			bbEquationSolution.LHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();

			Term1.vecVariable.emplace_back( vecChosenVariables[1], 1 );
			Term1.Coefficient = 1;
			bbEquationSolution.RHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			break;
		case 6:
			// bbEquationSolution
			Term1.vecVariable.emplace_back( vecChosenVariables[2], 1 );
			Term1.Coefficient = 1;
			bbEquationSolution.LHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();

			Term1.vecVariable.emplace_back( vecChosenVariables[1], 1 );
			Term1.Coefficient = 1;
			bbEquationSolution.RHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			Term1.vecVariable.emplace_back( vecChosenVariables[3], 1 );
			Term1.Coefficient = 1;
			bbEquationSolution.RHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			Term1.vecVariable.emplace_back( vecChosenVariables[0], 1 );
			Term1.Coefficient = -1;
			bbEquationSolution.RHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			break;
		case 7:
			// bbEquationSolution
			Term1.vecVariable.emplace_back( vecChosenVariables[0], 1 );
			Term1.Coefficient = 1;
			bbEquationSolution.LHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();

			Term1.vecVariable.emplace_back( vecChosenVariables[2], 1 );
			Term1.vecVariable.emplace_back( vecChosenVariables[1], -1 );
			Term1.Coefficient = 1;
			bbEquationSolution.RHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			Term1.vecVariable.emplace_back( vecChosenVariables[3], 1 );
			Term1.vecVariable.emplace_back( vecChosenVariables[1], -1 );
			Term1.Coefficient = 1;
			bbEquationSolution.RHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			Term1.vecVariable.emplace_back( vecChosenVariables[4], 1 );
			Term1.vecVariable.emplace_back( vecChosenVariables[1], -1 );
			Term1.Coefficient = 1;
			bbEquationSolution.RHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			break;
		case 8:
			// bbEquationSolution
			Term1.vecVariable.emplace_back( vecChosenVariables[0], 1 );
			Term1.vecVariable.emplace_back( vecChosenVariables[2], -1 );
			Term1.Coefficient = 1;
			bbEquationSolution.LHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			Term1.vecVariable.emplace_back( vecChosenVariables[3], 1 );
			Term1.vecVariable.emplace_back( vecChosenVariables[2], -1 );
			Term1.Coefficient = -1;
			bbEquationSolution.LHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();

			Term1.vecVariable.emplace_back( vecChosenVariables[1], 1 );
			Term1.Coefficient = 1;
			bbEquationSolution.RHS.vecTerm.push_back(Term1);
			Term1.vecVariable.clear();
			break;
		}
	}

	void ChooseVariables(int numVariables)
	{
		// So we are selecting a requested number of substrings from a string and we don't want duplicates
		int numChosen = 0;
		int lenOperationMaster = 0;
		int bbIndex;
		std::string OperationsWorking = OperationsMaster;
		vecChosenVariables.clear();

		for (char a : OperationsMaster) lenOperationMaster++;

		std::random_device rd; // obtain a random number from hardware
		std::mt19937 gen(rd()); // seed the generator

		while (numChosen < numVariables) {
			std::uniform_int_distribution<> distr(0, lenOperationMaster - (numChosen +1)); // define the range
			bbIndex = distr(gen);
			std::cout << bbIndex << ' '; // generate number
			std::cout << OperationsWorking.substr(bbIndex, 1) << ' ';
			vecChosenVariables.push_back(OperationsWorking.substr(bbIndex, 1));
			OperationsWorking.erase(bbIndex, 1);
			numChosen++;
		}
	}

	void PopulatevecAppropriateVariablesCycle()
	{
		for (std::string bbOperation : vecAppropriateVariables) vecAppropriateVariablesCycle.push_back(bbOperation);
	}

	void LoadLogarithmLevel(int CurrentEquationLevel)
	{
		//Initialise
		vecOperations.clear();
		vecTicks.clear();
		LevelComplete = false;
		BetweenEquationLevels = false;

		vecAppropriateLogarithms.clear();
		vecAppropriateBases.clear();
		vecAppropriateLogarithmsCycle.clear();

		TriangleLogarithm.Base = NULL;
		TriangleLogarithm.Log = NULL;
		TriangleLogarithm.AntiLog = NULL;


		switch (CurrentEquationLevel)
		{
		case 1:
			GenLogs(10, 1, 5);
			for (int a : vecLogs) vecAppropriateLogarithms.emplace_back( 10, a, (long)pow(10, a), false );
			vecAppropriateBases.push_back("b10");
			break;
		case 2:
			GenLogs(2, 1, 5);
			for (int a : vecLogs) vecAppropriateLogarithms.emplace_back( 2, a, (long)pow(2, a), false );
			vecAppropriateBases.push_back("b2");
			break;
		case 3:
			GenLogs(2, 1, 2);
			for (int a : vecLogs) vecAppropriateLogarithms.emplace_back( 2, a, (long)pow(2, a), false );
			GenLogs(3, 1, 3);
			for (int a : vecLogs) vecAppropriateLogarithms.emplace_back( 3, a, (long)pow(3, a) , false);
			vecAppropriateBases.push_back("b2");
			vecAppropriateBases.push_back("b3");
			break;
		case 4:
			GenLogs(2, 1, 1);
			for (int a : vecLogs) vecAppropriateLogarithms.emplace_back( 2, a, (long)pow(2, a) , false);
			GenLogs(5, 1, 2);
			for (int a : vecLogs) vecAppropriateLogarithms.emplace_back( 5, a, (long)pow(5, a) , false);
			GenLogs(3, 1, 1);
			for (int a : vecLogs) vecAppropriateLogarithms.emplace_back( 3, a, (long)pow(3, a) , false);
			GenLogs(5, 1, 1);
			for (int a : vecLogs) vecAppropriateLogarithms.emplace_back( 5, a, (long)pow(5, a) , false);
			vecAppropriateBases.push_back("b2");
			vecAppropriateBases.push_back("b3");
			vecAppropriateBases.push_back("b5");
			break;
		default:
			GenLogs(2, 1, 5);
			for (int a : vecLogs) vecAppropriateLogarithms.emplace_back( 2, a, (long)pow(2, a), false);
			vecAppropriateBases.push_back("b2");
		}
	}

	void PopulatevecAppropriateLogarithmsCycle()
	{
		for (auto AppropriateBase : vecAppropriateBases) vecAppropriateLogarithmsCycle.push_back(AppropriateBase);
		

		for (auto AppropriateLog : vecAppropriateLogarithms) { 
			std::string LogStr = "L" + std::to_string(AppropriateLog.Log);
			vecAppropriateLogarithmsCycle.push_back(LogStr);
		}
	}

	void GenLogs(int base, long Floor, int NumLogs)
	{
		long bbNumber;
		int bbCounter;
		bool AllPositive = false;

		while (!AllPositive) {
			bbNumber = Floor;
			bbCounter = 0;
			vecLogs.clear();
			while (bbCounter < NumLogs) {
				bbRnd = (float)rand() / (float)RAND_MAX;
				if (bbRnd < 0.3) { vecLogs.push_back(bbNumber); bbCounter++; }
				bbNumber++;
			}
			if ((long) pow(base, bbNumber) > 0)
				AllPositive = true;
		}
	}

	std::string GenerateOpStr()
	{
		//In the first instance we are only going to generate xVariable and %Variable

		float MaxRnd = 0;
		std::string OpStr = "";

		if (!DoingLogarithms) {
			if (!InReverse) {
				if (vecAppropriateVariablesCycle.empty()) PopulatevecAppropriateVariablesCycle();

				// Generate a random number for each appropriate operation
				int bbCounter = 0;
				int bbPlaceHolder;
				for (std::string bbVar : vecAppropriateVariablesCycle) {
					bbRnd = (float)rand() / (float)RAND_MAX;
					if (bbRnd > MaxRnd) {
						MaxRnd = bbRnd;
						OpStr = bbVar;
						bbPlaceHolder = bbCounter;
					}
					bbCounter++;
				}
				vecAppropriateVariablesCycle.erase(vecAppropriateVariablesCycle.begin() + bbPlaceHolder);
				return OpStr;
			} else {
				return RequiredOpStr;
			}		
		} else {
			if (vecAppropriateLogarithmsCycle.empty()) PopulatevecAppropriateLogarithmsCycle();
			
			// Generate a random number for each appropriate operation
			int bbCounter = 0;
			int bbPlaceHolder;
			for (std::string bbVar : vecAppropriateLogarithmsCycle) {
				bbRnd = (float)rand() / (float)RAND_MAX;
				if (bbRnd > MaxRnd) {
					MaxRnd = bbRnd;
					OpStr = bbVar;
					bbPlaceHolder = bbCounter;
				}
				bbCounter++;
			}
			vecAppropriateLogarithmsCycle.erase(vecAppropriateLogarithmsCycle.begin() + bbPlaceHolder);
			return OpStr;
		}

	}

	void PerformOperationControl(Equation& bbEquation, std::string opStr)
	{
		PresentOperation = opStr.substr(0, 1);
		PresentOperationVariable = opStr.substr(1, opStr.length());

		vecConductedOperations.push_back(opStr);

		if (PresentOperation == "x") OperationMultiplication(bbEquation, PresentOperationVariable);
		if (PresentOperation == "%") OperationDivision(bbEquation, PresentOperationVariable);
		if (PresentOperation == "+") OperationAddition(bbEquation, PresentOperationVariable);
		if (PresentOperation == "-") OperationSubtraction(bbEquation, PresentOperationVariable);

		if (PresentOperation == "b") OperationPlaceBase(PresentOperationVariable);
		if (PresentOperation == "L") OperationPlaceLog(PresentOperationVariable);

		CheckIfSolutionAchieved(bbEquation); //Also handles situation if solution achieved

	}

	void OperationMultiplication(Equation& bbEquation, std::string varStr)
	{
		int bbCounter = 0;

		bool VariableFound = false;
		//Go through all terms in bbEquation, if variable names match increase power by 1
		//We should make a vector of equation sides but that is tomorrow's business
		for (Term &bbTerm : bbEquation.LHS.vecTerm) {
			for (Variable &bbVar : bbTerm.vecVariable) {
				if (bbVar.Name == varStr) {
					bbVar.Power++;
					VariableFound = true;
					if (bbVar.Power == 0) bbTerm.vecVariable.erase(bbTerm.vecVariable.begin() + bbCounter);
				}
				bbCounter++;
			}
			if (VariableFound == false) bbTerm.vecVariable.emplace_back( varStr, 1 );
			VariableFound = false;
			bbCounter = 0;
		}


		for (Term &bbTerm : bbEquation.RHS.vecTerm) {
			for (Variable &bbVar : bbTerm.vecVariable) {
				if (bbVar.Name == varStr) {
					bbVar.Power++;
					VariableFound = true;
					if (bbVar.Power == 0) bbTerm.vecVariable.erase(bbTerm.vecVariable.begin() + bbCounter);	
				}
				bbCounter++;
			}
			if (VariableFound == false) bbTerm.vecVariable.emplace_back(varStr, 1 );
			VariableFound = false;
			bbCounter = 0;
		}
		GroupByDenominator(bbEquation.LHS);
		GroupByDenominator(bbEquation.RHS);
	}

	void OperationDivision(Equation& bbEquation, std::string varStr)
	{
		int bbCounter = 0;
		bool VariableFound = false;
		for (Term& bbTerm : bbEquation.LHS.vecTerm) {
			for (Variable& bbVar : bbTerm.vecVariable) {
				if (bbVar.Name == varStr) {
					bbVar.Power--;
					VariableFound = true;
					if (bbVar.Power == 0) bbTerm.vecVariable.erase(bbTerm.vecVariable.begin() + bbCounter);
				}
				bbCounter++;
			}
			if (VariableFound == false) bbTerm.vecVariable.emplace_back( varStr, -1 );
			VariableFound = false;
			bbCounter = 0;
		}


		for (Term& bbTerm : bbEquation.RHS.vecTerm) {
			for (Variable& bbVar : bbTerm.vecVariable) {
				if (bbVar.Name == varStr) {
					bbVar.Power--;
					VariableFound = true;
					if (bbVar.Power == 0) bbTerm.vecVariable.erase(bbTerm.vecVariable.begin() + bbCounter);
				}
				bbCounter++;
			}
			if (VariableFound == false) bbTerm.vecVariable.emplace_back( varStr, -1 );
			VariableFound = false;
			bbCounter = 0;
		}
		GroupByDenominator(bbEquation.LHS);
		GroupByDenominator(bbEquation.RHS);
	}
	
	void AggregateTerms(SideOfEquation& Expression)
	{
		SideOfEquation ExpressionCopy;
		SideOfEquation Aggregated;
		int Coefficient = 0;

		for (Term Term1 : Expression.vecTerm) ExpressionCopy.vecTerm.push_back(Term1);

		for (Term& Term1 : Expression.vecTerm) {
			Coefficient = 0;
			for (Term& Term2 : ExpressionCopy.vecTerm) {
				if (Term2.Aggregated == false) {
					if (TermsEquivalent(Term1, Term2) == true) {
						Coefficient += Term2.Coefficient;
						Term2.Aggregated = true;
						// This works because we only set the aggregate flag on the copy and leave
						// the original that gets passed on untouched.
					}
				}
			}
			if (Coefficient != 0) {
				Term1.Coefficient = Coefficient;
				Aggregated.vecTerm.push_back(Term1);
			}
		}
		Expression.vecTerm.clear();
		for (Term bbTerm : Aggregated.vecTerm) Expression.vecTerm.push_back(bbTerm);
	}

	void PrepareDenominator(Term& Term)
	{
		Term.vecDenominator.clear();
		for (Variable Var : Term.vecVariable) 
			if (Var.Power < 0) Term.vecDenominator.push_back(Var);
	}

	void PrepareNumerator(Term& Term)
	{
		Term.vecNumerator.clear();
		for (Variable Var : Term.vecVariable)
			if (Var.Power > 0) Term.vecNumerator.push_back(Var);
	}

	void GroupByDenominator(SideOfEquation& Expression)
	{
		SideOfEquation ExpressionCopy;
		SideOfEquation GroupedByDenominator;

		for (Term& Term1 : Expression.vecTerm) {
			PrepareDenominator(Term1);
			ExpressionCopy.vecTerm.push_back(Term1);
		}

		for (Term& Term1 : Expression.vecTerm) {
			for (Term& Term2 : ExpressionCopy.vecTerm) {
				if (Term2.Aggregated == false) {
					if (DenominatorsEquivalent(Term1, Term2)) {
						GroupedByDenominator.vecTerm.push_back(Term2);
						Term2.Aggregated = true;
					}
				}
			}
		}
		
		Expression.vecTerm.clear();
		for (Term Term1 : GroupedByDenominator.vecTerm) Expression.vecTerm.push_back(Term1);
		
	}

	void OperationAddition(Equation& bbEquation, std::string varStr)
	{
		Term Term1;
		Term1.vecVariable.emplace_back( varStr, 1 );
		Term1.Coefficient = 1;
		bbEquation.LHS.vecTerm.push_back(Term1);
		Term Term2;
		Term2.vecVariable.emplace_back( varStr, 1 );
		Term2.Coefficient = 1;
		bbEquation.RHS.vecTerm.push_back(Term2);
		AggregateTerms(bbEquation.LHS);
		AggregateTerms(bbEquation.RHS);
	}

	void OperationSubtraction(Equation& bbEquation, std::string varStr)
	{
		Term Term1;
		Term1.vecVariable.emplace_back( varStr, 1 );
		Term1.Coefficient = -1;
		bbEquation.LHS.vecTerm.push_back(Term1);
		Term Term2;
		Term2.vecVariable.emplace_back( varStr, 1 );
		Term2.Coefficient = -1;
		bbEquation.RHS.vecTerm.push_back(Term2);
		AggregateTerms(bbEquation.LHS);
		AggregateTerms(bbEquation.RHS);
	}

	void OperationPlaceBase(std::string bbBase)
	{
		float fBase = std::stof(bbBase);
		int iBase = std::stoi(bbBase);
		long lBase = std::stol(bbBase);

		TriangleLogarithm.Base = lBase;
	}

	void OperationPlaceLog(std::string bbLog)
	{
		long lLog = std::stol(bbLog);

		TriangleLogarithm.Log = lLog; 
	}

	void OperationPlaceAntiLog(float bbAntilog)
	{

	}

	void ClearTriangleLogarithm()
	{
		TriangleLogarithm.Base = NULL;
		TriangleLogarithm.Log = NULL;
		TriangleLogarithm.AntiLog = NULL;
	}

	void CheckIfSolutionAchieved(Equation& bbEquation)
	{
		//Here we might want a EquationSolution to go with Equation data type.
		//We want to call this after every operation is performed
		//Increments bbEquationLevel if this function returns true

		// Check LHS against LHS and RHS against RHS then swap and check
		// LHS against RHS and RHS against LHS

		// This is a bit spaghetti code but there are justifications 
		if (!DoingLogarithms) {
			bool SolutionAchieved = false;
			if ((SidesEquivalent(bbEquation.LHS, bbEquationSolution.LHS) and SidesEquivalent(bbEquation.RHS, bbEquationSolution.RHS))
				or (SidesEquivalent(bbEquation.LHS, bbEquationSolution.RHS) and SidesEquivalent(bbEquation.RHS, bbEquationSolution.LHS)))
				SolutionAchieved = true;
			if (SolutionAchieved) ChangeOperationsToTicks();
			
			BetweenEquationLevels = SolutionAchieved;

			if (!SolutionAchieved) {
				// Check if MaxComplexity reached
				if (MaxComplexityReached(bbEquation, MaxComplexity) == true) {
					// Restart Level - Player Specific
					LoadEquationStart(bbEquation, bbEquationLevel);
					DisplayBouncingMessage("Starting Again");
				} else {
					InReverse = TooManyTerms(bbEquation, MaxTerms);
					if (InReverse) ChangeOperationsReverse(GetLastPerformedOperation());
				}
			}

		} else {
			// If Triangle Logarithm is the first instance of an 'appropriate' logarithm
			// Then that Log is flagged as achieved (and coloured blue)
			// If the Triangle Logarithm is not in the list or is a second instance
			// Release  bomb wave

			//Logarithm& TargetLog = vecAppropriateLogarithms.front();
			
			if ((std::to_string(TriangleLogarithm.Base) != "0") && (std::to_string(TriangleLogarithm.Log) != "0")) {
				if ((long)pow(TriangleLogarithm.Base, TriangleLogarithm.Log) > 0) {
					bool BombsNeeded = true;
					TriangleLogarithm.AntiLog = (long)pow(TriangleLogarithm.Base, TriangleLogarithm.Log);
					for (Logarithm& bbLog : vecAppropriateLogarithms) {
						if (bbLog.AntiLog == TriangleLogarithm.AntiLog) {
							BombsNeeded = false;
							if (bbLog.Achieved == false) bbLog.Achieved = true;
							else BombsNeeded = true;
						}
					}
					if (BombsNeeded == true) BombWave(0, 0);
				} else {
					ClearTriangleLogarithm();
				}
			}
			//Check for solution
			bool bAllFound = true;
			for (Logarithm& bbLog : vecAppropriateLogarithms) {
				if (bbLog.Achieved == false) bAllFound = false;
			}
			if (bAllFound == true) {
				ChangeOperationsToTicks();
				BetweenEquationLevels = true;
			}
		}
	}

	bool SidesEquivalent(SideOfEquation Side1, SideOfEquation Side2)
	{
		//Assuming any gathering has taken place

		// Check first number of terms is equal
		// If there are the same number of terms and all the terms have a match then
		// SidesEquivalent is true
		
		bool TermMatch = false;
		int nNumTerm1 = 0;
		int nNumTerm2 = 0;

		for (Term Term1 : Side1.vecTerm) nNumTerm1++;
		for (Term Term2 : Side2.vecTerm) nNumTerm2++;

		if (nNumTerm1 != nNumTerm2) return false;

		for (Term Term1 : Side1.vecTerm) {
			TermMatch = false;
			for (Term Term2 : Side2.vecTerm) {
				if (TermsEquivalent(Term1, Term2)) TermMatch = true;
			}
			if (TermMatch == false) return false;
		}
		return true;
	}

	bool TermsEquivalent(Term Term1, Term Term2)
	{
		bool VariableMatch = false;
		int nNumVar1 = 0;
		int nNumVar2 = 0;

		// Check first for same number of variables - we will implement coefficients later
		// If there are the same number of variables and all the variables have a match then
		// TermsEquivalent is true.

		for (Variable Var1 : Term1.vecVariable) nNumVar1++;
		for (Variable Var2 : Term2.vecVariable) nNumVar2++;

		if (nNumVar1 != nNumVar2) return false;

		// So if there are a different number of variables the terms are not equivalent

		for (Variable Var1 : Term1.vecVariable) {
			VariableMatch = false;
			// Here we are checking every variable for a match - of variable and power
			for (Variable Var2 : Term2.vecVariable) {
				if (Var1.Name == Var2.Name)
					if (Var1.Power == Var2.Power)
						VariableMatch = true;
			}
			// If we don't find a match we return false
			if (VariableMatch == false) return false;
		}
		
		return true;
		
	}

	bool DenominatorsEquivalent(Term Term1, Term Term2)
	{
		bool VariableMatch = false;
		int nNumVar1 = 0;
		int nNumVar2 = 0;

		for (Variable Var1 : Term1.vecDenominator) nNumVar1++;
		for (Variable Var2 : Term2.vecDenominator) nNumVar2++;

		if (nNumVar1 != nNumVar2) return false;

		//Case of no denominator
		if ((nNumVar1 == 0) and (nNumVar2 == 0))
			return true;


		for (Variable Var1 : Term1.vecDenominator) {
			VariableMatch = false;
			for (Variable Var2 : Term2.vecDenominator) {
				if (Var1.Name == Var2.Name)
					if (Var1.Power == Var2.Power)
						VariableMatch = true;
			}
			if (VariableMatch == false)
				return false;
		}
		return true;
	}

	bool DenominatorExists(Term Term1)
	{
		int VariablesInDenominator = 0;

		//Term1.vecDenominator.empty();

		for (Variable v : Term1.vecDenominator)
			VariablesInDenominator++;

		if (VariablesInDenominator > 0) return true;
		else return false;
	
	}

	//
	//	Teaching functions
	//
	bool MaxComplexityReached(Equation& bbEquation, int MaxComplexity)
	{
		Complexity = 0;

		for (Term bbTerm : bbEquation.LHS.vecTerm) {
			Complexity += abs(bbTerm.Coefficient);
			for (Variable bbVariable : bbTerm.vecVariable) {
				Complexity += abs(bbVariable.Power);
			}
		}
	
		for (Term bbTerm : bbEquation.RHS.vecTerm) {
			Complexity += abs(bbTerm.Coefficient);
			for (Variable bbVariable : bbTerm.vecVariable) {
				Complexity += abs(bbVariable.Power);
			}
		}
		if (Complexity >= MaxComplexity) return true;
		else return false;
		
	}

	void DisplayBouncingMessage(std::string BouncingMessage)
	{
		//sSpaceObject soBouncingMessage;
		float xOffset = (float)-1 * BouncingMessage.length() * 20;
		vecBouncingMessages.push_back({ (float) ((ScreenWidth() / 2.0) + xOffset), -30, 0, 300, 0, 0, BouncingMessage });
	}

	bool TooManyTerms(Equation& bbEquation, int TermLimit)
	{
		int counter = 0;
		for (Term Term1 : bbEquation.LHS.vecTerm) counter++;
		
		for (Term Term1 : bbEquation.RHS.vecTerm) counter++;
		
		if (counter >= TermLimit) return true;
		return false;
	}

	std::string GetLastPerformedOperation()
	{
		std::string RequiredOp;
		RequiredOp = vecConductedOperations.back();
		vecConductedOperations.pop_back();
		return RequiredOp;
	}

	void PrepareEquationSide(SideOfEquation& EquationSide)
	{
		EquationSide.SideOfEquationStr = "";
		// Right, we want to assemble things into denominators and numerators
		std::string NumeratorStr = "";
		std::string DenominatorStr = "";
		bool CurrentTermHasDenominator = false;
		bool DenominatorLoaded = false;
		bool TermToSend = false;
		Term LoadedDenominator;
		Term NextDenominator;
		Term LastTerm;
		Term Numerator;
		std::vector<Term> vecNumerators;
		int NumberOfTimes = 0;

		for (Term& bbTerm : EquationSide.vecTerm)
		{
			PrepareNumerator(bbTerm);
			PrepareDenominator(bbTerm);
			//We are starting here as this is the crucial part of the algorithm
			if (DenominatorLoaded == false)
			{
				CurrentTermHasDenominator = !bbTerm.vecDenominator.empty();
				if (CurrentTermHasDenominator == true)
				{
					// 'Load' denominator
					LoadedDenominator.vecDenominator = bbTerm.vecDenominator;
					DenominatorLoaded = true;
					// 'Load' Numerator
					Numerator.vecNumerator = bbTerm.vecNumerator;
					Numerator.Coefficient = bbTerm.Coefficient;
					vecNumerators.push_back(Numerator);
					TermToSend = true;
				} else {
					// Clear loaded denominator
					LoadedDenominator.vecDenominator = bbTerm.vecDenominator; //this will clear denominator as bbTerm has none
					// 'Load' Numerator
					Numerator.vecNumerator = bbTerm.vecNumerator;
					Numerator.Coefficient = bbTerm.Coefficient;
					vecNumerators.push_back(Numerator);
					// Paginate Numerator only term
					ConstructTermStr(vecNumerators, LoadedDenominator, EquationSide);
					TermToSend = false;
					vecNumerators.clear();
				}
			} else {
				// Denominator loaded
				// Now check if current term has denominator
				CurrentTermHasDenominator = !bbTerm.vecDenominator.empty();
				if (CurrentTermHasDenominator == true) {
					// Compare current denominator and loaded denominator
					// If they are the same concatenate numerators
					NextDenominator.vecDenominator = bbTerm.vecDenominator;
					if (DenominatorsEquivalent(LoadedDenominator, NextDenominator) == true) {
						// Concatenate Numerators 
						Numerator.vecNumerator = bbTerm.vecNumerator;
						Numerator.Coefficient = bbTerm.Coefficient;
						vecNumerators.push_back(Numerator);
						TermToSend = true;
						//NumberOfTimes++;
						//ScreenOutputFlag3 = "Got here Cap'n " + std::to_string(NumberOfTimes) + " times.";
					} else {
						// If they are different paginate last term and load new denominator
						// and numerator
						ConstructTermStr(vecNumerators, LoadedDenominator, EquationSide);
						
						//NumberOfTimes++;
						//ScreenOutputFlag3 = "Got here Cap'n " + std::to_string(NumberOfTimes) + " times.";
						// Clear from last term
						DenominatorStr = "";
						vecNumerators.clear();
						// 'Load' denominator
						LoadedDenominator.vecDenominator = bbTerm.vecDenominator;
						DenominatorLoaded = true;
						// 'Load' Numerator
						Numerator.vecNumerator = bbTerm.vecNumerator;
						Numerator.Coefficient = bbTerm.Coefficient;
						vecNumerators.push_back(Numerator);

						TermToSend = true;
					}
				} else {
					// If a denominated is loaded and there is no current denominator  
					// paginate last term  and load numerator only term.

					DenominatorLoaded = false;
					ConstructTermStr(vecNumerators, LoadedDenominator, EquationSide);
					// Clear from last term
					LoadedDenominator.vecDenominator = bbTerm.vecDenominator;
					DenominatorStr = "";
					vecNumerators.clear();

					// ALERT: These compound terms will have multiple coefficients within them
					// therefore that coefficient will need to be included in the TermStr

					Numerator.vecNumerator = bbTerm.vecNumerator;
					Numerator.Coefficient = bbTerm.Coefficient;
					vecNumerators.push_back(Numerator);

					TermToSend = true;
				}

			}
			//LastTerm = bbTerm;
		}

		// Taking care of last term which does not paginate in loop 
		if (TermToSend == true) ConstructTermStr(vecNumerators, LoadedDenominator, EquationSide);
		//Deal with first + or -
		if (EquationSide.SideOfEquationStr[0] != '-') EquationSide.SideOfEquationStr = EquationSide.SideOfEquationStr.erase(0, 1);
		
		EquationSide.SideOfEquationStr = "[{1}" + EquationSide.SideOfEquationStr + "]";

	}

	//void ConstructTermStr(std::string Numerator, std::string Denominator, SideOfEquation& Side, Term bbTerm)
	//void ConstructTermStr(std::vector<Term> vecNumerators, std::string DenominatorStr, SideOfEquation& Side)
	void ConstructTermStr(std::vector<Term> vecNumerators, Term Denominator, SideOfEquation& Side)
	{
		std::string NumeratorStr = "";
		std::string CurrentNumeratorStr = "";
		bool FirstCoefficientNegative = false;
		bool FirstNumerator = true;
		int HeldCoefficient;
		
		std::string DenominatorStr = "";
		int LenDenominator = 0;
		int NumCompoundNumerators = 0;
		int NumCharactersInCompoundNumerators = 0;
		int TotalPadding = 0;
		std::string LeftPadding = "";
		std::string RightPadding = "";
		
		// So we want to pad by NumCharactersInCompound Numerator + 2 for every Compound Numerator > 1

		// Then we want to pad by one more for every extra variable in the numerator
		// Then we want to subtract the number of variables in the denominator
		// Then we want to pad evenly on each side

		// As our terms may now be compound we will want to build up a TermStr before we add directly to the SideStr
		// We work up a NumeratorStr and a DenominatorStr and this will then be passed to a
		// CombineNumeratorAndDenominator function or maybe CombineTermStr

		// We shall have a vector vecNumerators, if the lead Numerator is negative the term becomes negative
		// and all the coefficients are negated

		//if (Numerator == "")
		// Case where we have only a coefficient
		// We want to cylce through vecNumerators...

		// See if first coefficient is negative
		if (vecNumerators[0].Coefficient < 0) FirstCoefficientNegative = true;

		//EquationBlaster::ScreenOutputFlag3 = "Here Cap'n";
		//ScreenOutputFlag4 = "Cap'n";
		for (Term Numerator : vecNumerators) NumCompoundNumerators++;
		//EquationBlaster::ScreenOutputFlag5 += " " + std::to_string(Numerator.Coefficient) + "Cap'n";
		

		if(Side.Name == "LHS") EquationBlaster::ScreenOutputFlag5 = " " + Side.Name + " " + std::to_string(NumCompoundNumerators);

		for (Term Numerator : vecNumerators) {
			//CurrentNumeratorStr = "";
			if (Numerator.vecNumerator.empty()) {
				// Numerator has no variable, therefore only coefficient returned
				if (abs(Numerator.Coefficient) < 10) NumCharactersInCompoundNumerators++; //This is to count the coefficient
				else {
					NumCharactersInCompoundNumerators++;
					NumCharactersInCompoundNumerators++; //We will allow then to muck up the formatting with a coeff of 100+ 
				}
				if (FirstNumerator) {
					if (std::to_string(Numerator.Coefficient)[0] == '-')
					{//Numerator = "[{1}1]";
						//Minuses and pluses put outside of variable brackets
						NumeratorStr += "[{1}" + std::to_string(Numerator.Coefficient).erase(0, 1) + "]";
					} else {
						NumeratorStr += "[{1}" + std::to_string(Numerator.Coefficient) + "]";
					}
				} else {
					if (FirstCoefficientNegative) HeldCoefficient = Numerator.Coefficient * -1;
					else HeldCoefficient = Numerator.Coefficient;

					if (HeldCoefficient > 0) NumeratorStr += "+[{1}[{1}" + std::to_string(HeldCoefficient) + "]]";
					else NumeratorStr += "-[{1}[{1}" + std::to_string(HeldCoefficient).erase(0, 1) + "]]";
	
					
				}
			} else {
				//Variable Numerator exists
				CurrentNumeratorStr = "";
				for (Variable bbVar : Numerator.vecNumerator) {
					CurrentNumeratorStr += "[{" + std::to_string(bbVar.Power) + "}" + bbVar.Name + "]";
					NumCharactersInCompoundNumerators++; //All our variables are presently one letter
					//Lets handle powers
					if (bbVar.Power == 1) {} //Do nothing} 
					else {
						//The power should not be negative or it would not be in the Numerator.
						if (abs(bbVar.Power) < 10) NumCharactersInCompoundNumerators++;
						else {
							NumCharactersInCompoundNumerators++;
							NumCharactersInCompoundNumerators++;
						}
					}
				}

				if (FirstNumerator) {
					if (Numerator.Coefficient == 1) {
						NumeratorStr += "[{1}[{1}" + CurrentNumeratorStr + "]]";
					} else {
						if (Numerator.Coefficient == -1) {
							NumeratorStr += "[{1}[{1}" + CurrentNumeratorStr + "]]";
							// Minus should be handled at end
						} else {
							//Handle character padding
							if (abs(Numerator.Coefficient) < 10) {
								if (abs(Numerator.Coefficient) != 1) NumCharactersInCompoundNumerators++; //This is to count the coefficient
							} else {
								NumCharactersInCompoundNumerators++;
								NumCharactersInCompoundNumerators++; //We will allow then to muck up the formatting with a coeff of 100+ 
							}

							if (Numerator.Coefficient > 0)
							{
								NumeratorStr += "[{1}[{1}[{1}" + std::to_string(Numerator.Coefficient) + "]" + CurrentNumeratorStr + "]]";
							}
							else
							{
								NumeratorStr += "[{1}[{1}[{1}" + std::to_string(Numerator.Coefficient).erase(0,1) + "]" + CurrentNumeratorStr + "]]";
								//Again minus should be handled at the end.
							}
						}
					}
				} else {
					if (FirstCoefficientNegative) HeldCoefficient = Numerator.Coefficient * -1;
					else HeldCoefficient = Numerator.Coefficient;

					if (HeldCoefficient == 1) NumeratorStr += "+[{1}" + CurrentNumeratorStr + "]";
					else {
						if (HeldCoefficient == -1) NumeratorStr += "-[{1}" + CurrentNumeratorStr + "]";
						else {
							if (HeldCoefficient > 0)
							{
								NumeratorStr += "+[{1}[{1}[{1}" + std::to_string(HeldCoefficient) + "]" + CurrentNumeratorStr + "]]";
							} else {
								NumeratorStr += "-[{1}[{1}[{1}" + std::to_string(HeldCoefficient).erase(0, 1) + "]" + CurrentNumeratorStr + "]]";
							}
						}
					}
				}
			}
			FirstNumerator = false; // This should go false after the first loop
		}

		// Construct DenominatorStr
		for (Variable bbVar : Denominator.vecDenominator) {
			LenDenominator++;
			std::string sPower;
			sPower = std::to_string(bbVar.Power);
			if (abs(bbVar.Power) > 1) LenDenominator++;
			DenominatorStr += "[{" + sPower.erase(0, 1) + "}" + bbVar.Name + "]";
		}

		if (DenominatorStr == "") {
			if (!FirstCoefficientNegative) Side.SideOfEquationStr += "+[{1}[{1}" + NumeratorStr + "]]";
			else Side.SideOfEquationStr += "-[{1}[{1}" + NumeratorStr + "]]";
			
		} else {
			if (NumCompoundNumerators == 1) {
				if (NumCharactersInCompoundNumerators > LenDenominator) {
					TotalPadding = NumCharactersInCompoundNumerators - LenDenominator;
				} else {
					// Do nothing I guess
				}
				if (Side.Name == "LHS")
					EquationBlaster::ScreenOutputFlag4 = "What? " + std::to_string(NumCharactersInCompoundNumerators);
			}
				

			if (NumCompoundNumerators == 2) {
				TotalPadding = NumCharactersInCompoundNumerators + 1 - LenDenominator;
				if(Side.Name == "LHS")
					EquationBlaster::ScreenOutputFlag4 = "What? " + std::to_string(NumCharactersInCompoundNumerators);
			}

			if (NumCompoundNumerators == 3) {
				TotalPadding = NumCharactersInCompoundNumerators + 4 - LenDenominator;
				EquationBlaster::ScreenOutputFlag4 = "What? " + std::to_string(NumCharactersInCompoundNumerators);
			}
			//EquationBlaster::ScreenOutputFlag3 += " " + Side.Name + " " + std::to_string(NumCharactersInCompoundNumerators);
			//EquationBlaster::ScreenOutputFlag3 += " " + Side.Name + " " + std::to_string(TotalPadding);

			if (TotalPadding == 0) {} // Do nothing
			else {
				for (int i = 1; i <= TotalPadding; i++) {
					if (i % 2 == 1) LeftPadding += " ";
					else RightPadding += " ";
				}
			}

			if (Side.Name == "LHS")
			{
				//EquationBlaster::ScreenOutputFlag3 = "Cap'n LP " + LeftPadding + " RP " + RightPadding;
				//EquationBlaster::ScreenOutputFlag3 = DenominatorStr;
			}

			if (!FirstCoefficientNegative) {
				if (TotalPadding < 1) {
					Side.SideOfEquationStr += "+[{1}[{1}" + NumeratorStr + "][{-1}" + DenominatorStr + "]]";
				} else {
					Side.SideOfEquationStr += "+[{1}[{1}" + NumeratorStr + "][{-1}[{1}" + LeftPadding + "]" + DenominatorStr + "[{1}" + RightPadding + "]]]";
				}
			} else {
				if (TotalPadding < 1) {
					Side.SideOfEquationStr += "-[{1}[{1}" + NumeratorStr + "][{-1}" + DenominatorStr + "]]";
				} else {
					Side.SideOfEquationStr += "-[{1}[{1}" + NumeratorStr + "][{-1}[{1}" + LeftPadding + "]" + DenominatorStr + "[{1}" + RightPadding + "]]]";
				}
				//Side.SideOfEquationStr += "-[{1}[{1}" + NumeratorStr + "][{-1}[{1}" + DenominatorStr + "]]]";
				//EquationBlaster::ScreenOutputFlag3 = DenominatorStr;
			}
		}
		//EquationBlaster::ScreenOutputFlag3 += " " + std::to_string(NumVariablesInCompoundNumerators);
	}

	void ResetGame()
	{
		if (!DoingLogarithms) {

			BetweenEquationLevels = false;

			vecAsteroids.clear();
			vecBullets.clear();
			vecOperations.clear();
			vecSpaceAnchors.clear();
			vecTicks.clear();

			bbAsteroidLevel = 1;
			bbEquationLevel = 1;

			LoadAsteroidLevel(bbAsteroidLevel);
			LoadEquationLevel(bbEquationLevel, false);

			AvailableSpaceAnchorsPlayer1 = 0;
			AvailableSpaceAnchorsPlayer2 = 0;
		} else {

			BetweenEquationLevels = false;

			vecAsteroids.clear();
			vecBullets.clear();
			vecOperations.clear();
			vecSpaceAnchors.clear();
			vecTicks.clear();

			bbAsteroidLevel = 1;
			bbLogarithmLevel = 1;

			LoadAsteroidLevel(bbAsteroidLevel);
			LoadLogarithmLevel(bbLogarithmLevel);
		}
		//bDead = -false;
		//nScore = 0;
	}

	void ChangeOperationsToTicks()
	{
		for (auto& a : vecOperations) vecTicks.emplace_back( a.pos.x, a.pos.y, a.vel.x, a.vel.y, 8, 0.0f );

		vecOperations.clear();
	}

	void ChangeOperationsReverse(std::string LastOpStr)
	{
		//std::string RequiredOpStr;
		std::vector<sSpaceObject> vecOperationsHold;

		if (LastOpStr.substr(0,1) == "+") RequiredOpStr = "-" + LastOpStr.substr(1,1);
		if (LastOpStr.substr(0,1) == "-") RequiredOpStr = "+" + LastOpStr.substr(1,1);
		if (LastOpStr.substr(0,1) == "x") RequiredOpStr = "%" + LastOpStr.substr(1,1);
		if (LastOpStr.substr(0,1) == "%") RequiredOpStr = "x" + LastOpStr.substr(1,1);

		for (auto& a : vecOperations)
			vecOperationsHold.emplace_back( a.pos.x, a.pos.y, a.vel.x, a.vel.y,  8, 0.0f, RequiredOpStr );
			//vecOperationsHold.push_back({ a.x, a.y, a.dx, a.dy,  8, 0.0f, LastOpStr });

		vecOperations.clear();

		for (auto& a : vecOperationsHold) vecOperations.push_back(a);

		// Our problem is the dropping function just takes variables and determines the operations
		// randomly.
	}

	void BombWave(float BombAngle, float fElapsedTime)
	{
		float MaxRnd = 0;
		int bbBombWave;
		for (int i = 1; i < 7; i++) {
			bbRnd = (float)rand() / (float)RAND_MAX;
			if (bbRnd > MaxRnd) {
				MaxRnd = bbRnd;
				bbBombWave = i;
			}
		}

		switch (bbBombWave) {
		case 1:
			//Toward South
			for (int i = 0; i <= 11; i++) vecBombs.push_back({ (float)(i * 100) + 50, -50, 0, 50, 12, BombAngle });
			break;
		case 2:
			//Toward East
			for (int i = 0; i <= 5; i++) vecBombs.push_back({ -50, (float)(i * 100) + 50, 50, 0, 12, 3 * Pi / 2 });
			break;
		case 3:
			//Toward SouthEast
			for (int i = 0; i <= 5; i++) vecBombs.push_back({ -90 + (float)(i * sqrt(1250)), 90 - (float)(i * sqrt(1250)), -50 * sinf(7 * Pi / 4), 50 * cosf(7 * Pi / 4), 12, 7 * Pi / 4 });
			break;
		case 4:
			//Toward North
			for (int i = 0; i <= 11; i++) vecBombs.push_back({ (float)(i * 100) + 50, (float)(ScreenHeight() + 50), 0, -50, 12, Pi });
			break;
		case 5:
			//Toward West
			for (int i = 0; i <= 5; i++) vecBombs.push_back({ (float)(ScreenWidth() + 50), (float)(i * 100) + 50, -50, 0, 12, Pi / 2 });
			break;
		case 6:
			//Toward NorthWest
			for (int i = 0; i <= 5; i++) vecBombs.push_back({ -90 + ScreenWidth() + (float)(i * sqrt(1250)), 90 + ScreenHeight() - (float)(i * sqrt(1250)) , -50 * sinf(3 * Pi / 4), 50 * cosf(3 * Pi / 4), 12, 3 * Pi / 4 });
			break;
		default:
				;
		}
	}

	bool OnUserUpdate(float fElapsedTime) override
	{

		if (bDead) {//Work needed
			ResetGame();
			nScore = 0;
			bDead = false;
		}

		// Clear Screen
		//Fill(0, 0, ScreenWidth(), ScreenHeight(), olc::PIXEL_SOLID, FG_BLACK);
		Clear(olc::BLACK);

		if (InMenu == true){
			if (InMainMenu == true) {
				DrawStringDecal({ 40, 10 }, "Equation Blaster", olc::Pixel(128, 64, 0), { 2,2 });
				DrawStringDecal({ 40, 30 }, "Space to select", olc::Pixel(128, 64, 0), { 1,1 });
				DrawStringDecal({ 40, 80 }, "Arrows to navigate", olc::Pixel(128, 64, 0), { 1,1 });
				DrawStringDecal({ 40, 230 }, "Click on screen to focus", olc::Pixel(128, 64, 0), { 1,1 });
				(nMenuSelect == 0) ? DrawStringDecal({ 40, 60 }, "Start Game", olc::YELLOW, { 2,2 }) : DrawStringDecal({ 40, 60 }, "Start Game", olc::Pixel(128, 64, 0), { 2,2 });
				(nMenuSelect == 1) ? DrawStringDecal({ 40, 110 }, "Instructions", olc::YELLOW, { 2,2 }) : DrawStringDecal({ 40, 110 }, "Instructions", olc::Pixel(128, 64, 0), { 2,2 });
				(nMenuSelect == 2) ? DrawStringDecal({ 40, 160 }, "Licence", olc::YELLOW, { 2,2 }) : DrawStringDecal({ 40, 160 }, "Licence", olc::Pixel(128, 64, 0), { 2,2 });
				(nMenuSelect == 3) ? DrawStringDecal({ 40, 210 }, "Credits", olc::YELLOW, { 2,2 }) : DrawStringDecal({ 40, 210 }, "Credits", olc::Pixel(128, 64, 0), { 2,2 });
				if (GetKey(olc::DOWN).bPressed) (nMenuSelect < 3) ? nMenuSelect++ : nMenuSelect = 0;
				if (GetKey(olc::UP).bPressed) (nMenuSelect > 0) ? nMenuSelect-- : nMenuSelect = 3;
				if (GetKey(olc::SPACE).bPressed) InMainMenu = false;
			}
			else {
				switch (nMenuSelect) {
				case 0:
					InMenu = false;
					break;
				case 1:
					DrawStringDecal({ 40, 10 }, "Space to return to menu", olc::Pixel(128, 64, 0), { 1,1 });
					DrawStringDecal({ 40, 40 }, "A - ROTATE LEFT", olc::Pixel(128, 64, 0), { 1.5,1.5 });
					DrawStringDecal({ 40, 80 }, "D - ROTATE RIGHT", olc::Pixel(128, 64, 0), { 1.5,1.5 });
					DrawStringDecal({ 40, 120 }, "W - THRUST", olc::Pixel(128, 64, 0), { 1.5,1.5 });
					DrawStringDecal({ 40, 160 }, "RETURN - FIRE", olc::Pixel(128, 64, 0), { 1.5,1.5 });
					DrawStringDecal({ 40, 200 }, "SPACE - DEPLOY SPACE ANCHOR", olc::Pixel(128, 64, 0), { 1.5,1.5 });
					break;
				case 2:
					Licence.clear();
					Licence.push_back("Tesla Tales Demo - Based on a One Lone Coder kernel.");
					Licence.push_back("");
					Licence.push_back("");
					Licence.push_back("License(OLC - 3) ~~~~~~~~~~~~~~~");
					Licence.push_back("");
					Licence.push_back("Copyright 2018 - 2020 OneLoneCoder.com");
					Licence.push_back("");
					Licence.push_back("redistribution and use in source and binary forms, with or without modification,");
					Licence.push_back("are permitted provided that the following conditions are met :");
					Licence.push_back("");
					Licence.push_back("1. Redistributions or derivations of source code must retain the above copyright");
					Licence.push_back("notice, this list of conditions and the following disclaimer.");
					Licence.push_back("");
					Licence.push_back("2. Redistributions or derivative works in binary form must reproduce the above");
					Licence.push_back("copyright notice.This list of conditions and the following	disclaimer must be");
					Licence.push_back("reproduced in the documentation and /or other materials provided with the distribution.");
					Licence.push_back("");
					Licence.push_back("3. Neither the name of the copyright holder nor the names of its contributors may");
					Licence.push_back("be used to endorse or promote products derived from this software without specific");
					Licence.push_back("prior written permission.");
					Licence.push_back("");
					Licence.push_back("THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS	'AS IS' AND ANY");
					Licence.push_back("EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES");
					Licence.push_back("OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.IN NO EVENT");
					Licence.push_back("SHALL THE COPYRIGHT	HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,");
					Licence.push_back("INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES(INCLUDING, BUT NOT LIMITED");
					Licence.push_back("TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR");
					Licence.push_back("BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN");
					Licence.push_back("CONTRACT, STRICT LIABILITY, OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN");
					Licence.push_back("ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF");
					Licence.push_back("SUCH DAMAGE.");

					numLines = 1;

					for (std::string bbStr : Licence) { DrawStringDecal({ 0, (float)(20 + (9 * numLines)) }, bbStr, olc::Pixel(128, 64, 0), { 1,1 }); numLines++; }
					break;
				case 3:
					DrawStringDecal({ 40, 50 }, "Equation Blaster is written in the Pixel Game Engine", olc::Pixel(128, 64, 0), { 1,1 });
					DrawStringDecal({ 40, 60 }, "which is the work of One Lone Conder David Barr.", olc::Pixel(128, 64, 0), { 1,1 });
					DrawStringDecal({ 40, 70 }, "Concept and additional coding Benedict Brown.", olc::Pixel(128, 64, 0), { 1,1 });
					break;
				}
				if (GetKey(olc::SPACE).bPressed) InMainMenu = true;
			}
		}	
		else
		{
			// Return to menu
			if (GetKey(olc::ESCAPE).bReleased) { InMenu = true; InMainMenu = true;}

			//Toggle between Equations and Logarithms
			if (GetKey(olc::L).bReleased) { DoingLogarithms = !DoingLogarithms; ResetGame();}

			//Advance Level
			if (GetKey(olc::I).bReleased) {
				if (DoingLogarithms) {
					if (bbLogarithmLevel < 4) bbLogarithmLevel++;
					else bbLogarithmLevel = 1;
					LoadLogarithmLevel(bbLogarithmLevel);
				} else {
					if (bbEquationLevel < LoadedEquationLevels) bbEquationLevel++;
					else bbEquationLevel = 1;
					//Between = false;
					LoadEquationLevel(bbEquationLevel, false);
				}
			}

			// Update and draw BouncingMessage
			for (auto& a : vecBouncingMessages) {
				a.vel.y += -150 * fElapsedTime;
				a.pos.x += a.vel.x * fElapsedTime;
				a.pos.y += a.vel.y * fElapsedTime;
				DrawString((int32_t)a.pos.x,(int32_t)a.pos.y, a.OpStr, olc::WHITE, 5);
			}

			//Bomb Wave
			if (GetKey(olc::B).bReleased) { BombWave(0.0f, fElapsedTime); }

			//Steer
			if (GetKey(olc::A).bHeld) { vecPlayers[0].angle -= 5.0f * fElapsedTime; }
			if (GetKey(olc::D).bHeld) { vecPlayers[0].angle += 5.0f * fElapsedTime; }

			//Thrust
			if (GetKey(olc::W).bHeld) {
				vecPlayers[0].vel.x += sin(vecPlayers[0].angle) * 40.0f * fElapsedTime;
				vecPlayers[0].vel.y += -cos(vecPlayers[0].angle) * 40.0f * fElapsedTime;
			}

			//Deploy Space Anchor - Fire Weapon - which is actually how you would arrange to slow down
			if (GetKey(olc::Key::SPACE).bReleased) {
				if (AvailableSpaceAnchorsPlayer1 > 0) {
					AvailableSpaceAnchorsPlayer1--; 
					vecPlayers[0].vel.x = vecPlayers[0].vel.x / 2;
					vecPlayers[0].vel.y = vecPlayers[0].vel.y / 2;
					/*
					if (vecPlayers[0].vel.x > 0) {
						if (vecPlayers[0].vel.y > 0) {
							vecBombs.push_back({ vecPlayers[0].pos.x + 20 + (0.5f * vecPlayers[0].vel.x), vecPlayers[0].pos.y + 20 + (0.5f * vecPlayers[0].vel.y), 1.5f * vecPlayers[0].vel.x , 1.5f * vecPlayers[0].vel.y, 12, atanf(vecPlayers[0].vel.y / vecPlayers[0].vel.x) - Pi / 2});
						} else {
							vecBombs.push_back({ vecPlayers[0].pos.x + 20 + (0.5f * vecPlayers[0].vel.x), vecPlayers[0].pos.y - 20 + (0.5f * vecPlayers[0].vel.y), 1.5f * vecPlayers[0].vel.x , 1.5f * vecPlayers[0].vel.y, 12, atanf(vecPlayers[0].vel.y / vecPlayers[0].vel.x) - Pi / 2 });
						}
					} else {
						if (vecPlayers[0].vel.y > 0)
						{
							vecBombs.push_back({ vecPlayers[0].pos.x - 20 + (0.5f * vecPlayers[0].vel.x), vecPlayers[0].pos.y + 20 + (0.5f * vecPlayers[0].vel.y), 1.5f * vecPlayers[0].vel.x , 1.5f * vecPlayers[0].vel.y, 12, atanf(vecPlayers[0].vel.y / vecPlayers[0].vel.x) + Pi / 2 });
						} else {
							vecBombs.push_back({ vecPlayers[0].pos.x - 20 + (0.5f * vecPlayers[0].vel.x), vecPlayers[0].pos.y - 20 + (0.5f * vecPlayers[0].vel.y), 1.5f * vecPlayers[0].vel.x , 1.5f * vecPlayers[0].vel.y, 12, atanf(vecPlayers[0].vel.y / vecPlayers[0].vel.x) + Pi / 2});
						}
					}
					*/
				}
			}

			// VELOCITY changes POSITION (with respect to time)
			vecPlayers[0].pos.x += vecPlayers[0].vel.x * fElapsedTime;
			vecPlayers[0].pos.y += vecPlayers[0].vel.y * fElapsedTime;

			//Keep ship in gamespace
			WrapCoordinates(vecPlayers[0].pos.x, vecPlayers[0].pos.y, vecPlayers[0].pos.x, vecPlayers[0].pos.y);

			//Check for collisions with Asteroids
			for (auto& a : vecAsteroids)
				if (IsPointInsideCircle(a.pos.x, a.pos.y, (float)a.nSize, vecPlayers[0].pos.x, vecPlayers[0].pos.y)) 
					bDead = true; // Uh oh....

			//Check for collisions with Bombs
			ReturnTransformedCoordinates(Ship, vecModelShip, vecPlayers[0].pos.x, vecPlayers[0].pos.y, vecPlayers[0].angle, 1);
			for (auto& a : vecBombs)
				if (IsPointInsideCircle(a.pos.x, a.pos.y, (float)a.nSize, vecPlayers[0].pos.x, vecPlayers[0].pos.y))
				{
					//Draw bounding box //Use DrawWireFrame model now
					DrawWireFrameModel(vecBoundingBoxBomb, a.pos.x, a.pos.y, a.angle, (float)a.nSize, olc::RED, 0xFFFFFFFF);
					ReturnTransformedCoordinates(BoundingBoxBomb, vecBoundingBoxBomb, a.pos.x, a.pos.y, a.angle, (float)a.nSize);

					//if (ShapeOverlap_DIAGS(Ship, BoundingBoxBomb) == true)
					if (ShapeOverlap_DIAGS(BoundingBoxBomb, Ship) == true) { ScreenOutputFlag1 = "Should work."; bDead = true;}
				}
			//bDead = true; // This will be poor.

			//Bullet in direction of player
			if (GetKey(olc::ENTER).bReleased) {
				vecBullets.push_back({ vecPlayers[0].pos.x, vecPlayers[0].pos.y, 100.0f * sinf(vecPlayers[0].angle), -100.0f * cosf(vecPlayers[0].angle), 0, 0 });
			}
		
			//Update and draw asteroids
			for (auto& a : vecAsteroids) {
				a.pos.x += a.vel.x * fElapsedTime;
				a.pos.y += a.vel.y * fElapsedTime;
				a.angle += 0.5f * fElapsedTime;
				WrapCoordinates(a.pos.x, a.pos.y, a.pos.x, a.pos.y);
				DrawWireFrameModel(vecModelAsteroid, a.pos.x, a.pos.y, a.angle, (float)a.nSize, olc::WHITE, 0xFFFFFFFF);
			}

			//Update and draw operations
			for (auto& a : vecOperations) {
				a.pos.x += a.vel.x * fElapsedTime;
				a.pos.y += a.vel.y * fElapsedTime;
				a.angle += 0.5f * fElapsedTime;
				WrapCoordinates(a.pos.x, a.pos.y, a.pos.x, a.pos.y);
				DrawString((int32_t)a.pos.x, (int32_t)a.pos.y, a.OpStr, olc::WHITE, 5);
			}

			//Check for scooping up Operations
			int bbCounter = 0;
			std::string CarryOpStr = "";
			for (auto& a : vecOperations) {
				if (IsPointInsideCircle(a.pos.x + (fScale * fOperationStrOffsetx), a.pos.y + (fScale * fOperationStrOffsety), (float)a.nSize, vecPlayers[0].pos.x, vecPlayers[0].pos.y)){
					CarryOpStr = a.OpStr;
					a.pos.x = -100;		// Oh we are erasing it below...
					vecOperations.erase(vecOperations.begin() + bbCounter);
					//Now here we are working from the position that all OpStr are two characters
					PerformOperationControl(bbEquationPlayer1, CarryOpStr);
				}
				bbCounter++;
			}
			//Update and draw Space Anchors
			for (auto& a : vecSpaceAnchors) {
				a.pos.x += a.vel.x * fElapsedTime;
				a.pos.y += a.vel.y * fElapsedTime;
				a.angle += 2.5f * fElapsedTime;
				WrapCoordinates(a.pos.x, a.pos.y, a.pos.x, a.pos.y);
				DrawWireFrameModel(vecModelSpaceAnchor, a.pos.x, a.pos.y, a.angle, (float)a.nSize, olc::WHITE, 0xFFFFFFFF);
			}

			//Check for scooping up Space Anchors
			for (auto& a : vecSpaceAnchors)
				if (IsPointInsideCircle(a.pos.x, a.pos.y, (float)a.nSize, vecPlayers[0].pos.x, vecPlayers[0].pos.y)) {
					AvailableSpaceAnchorsPlayer1++;
					a.pos.x = -100;		// Send it off the screen
				}

			//Update and draw Ticks
			for (auto& a : vecTicks) {
				a.pos.x += a.vel.x * fElapsedTime;
				a.pos.y += a.vel.y * fElapsedTime;
				a.angle = 0;
				WrapCoordinates(a.pos.x, a.pos.y, a.pos.x, a.pos.y);
				DrawWireFrameModel(vecModelTick, a.pos.x, a.pos.y, a.angle, (float)a.nSize, olc::WHITE, 0xFFFFFFFF);
			}

			//Check for scooping up Ticks
			for (auto& a : vecTicks)
				if (IsPointInsideCircle(a.pos.x, a.pos.y, (float)a.nSize, vecPlayers[0].pos.x, vecPlayers[0].pos.y)) {
					//In Here
					vecTicks.clear();
					if (!DoingLogarithms){
						if (bbEquationLevel < LoadedEquationLevels){
							bbEquationLevel++;
							LoadEquationLevel(bbEquationLevel, false);
						} else {
							bbEquationLevel = 1;
							DoingLogarithms = true;
							bbLogarithmLevel = 1;
							LoadLogarithmLevel(bbLogarithmLevel);
						}
						BetweenEquationLevels = false;
					} else {
						if (bbLogarithmLevel < 4){
							bbLogarithmLevel++;
							LoadLogarithmLevel(bbLogarithmLevel);
						} else { 
							bbLogarithmLevel = 1;
							DoingLogarithms = false;
							bbEquationLevel = 1;
							LoadEquationLevel(bbEquationLevel, false);
						}
						BetweenEquationLevels = false;
					}
				}

			fAccruedTime += fElapsedTime;

			// Update and draw Bombs
			for (auto& a : vecBombs) {
				a.pos.x += a.vel.x * fElapsedTime;
				a.pos.y += a.vel.y * fElapsedTime;
				DrawWireFrameModel(vecModelBomb, a.pos.x, a.pos.y, a.angle, (float)a.nSize, olc::WHITE, 0xFFFFFFFF);
			}
			std::vector<sSpaceObject> newAsteroids;
		
			//Update and draw bullets
			for (auto& b : vecBullets) {
				b.pos.x += b.vel.x * fElapsedTime;
				b.pos.y += b.vel.y * fElapsedTime;
				//WrapCoordinates(b.pos.x, b.pos.y, b.pos.x, b.pos.y);
				Draw((int)b.pos.x, (int)b.pos.y);

				// Check collision with asteroids
				for (auto& a : vecAsteroids) {
					if (IsPointInsideCircle(a.pos.x, a.pos.y, (float)a.nSize, b.pos.x, b.pos.y)) {
						// Asteroid Hit
						b.pos.x = -100;
						if (a.nSize > 4) {
							// Create three child asteroids
							float angle1 = ((float)rand() / (float)RAND_MAX) * 6.283185f;
							float angle2 = ((float)rand() / (float)RAND_MAX) * 6.283185f;
							float angle3 = ((float)rand() / (float)RAND_MAX) * 6.283185f;

							newAsteroids.push_back({ a.pos.x, a.pos.y, AsteroidSpeed * sinf(angle1), -AsteroidSpeed * cosf(angle1), (int)a.nSize >> 1, 0.0f });
							newAsteroids.push_back({ a.pos.x, a.pos.y, AsteroidSpeed * sinf(angle2), -AsteroidSpeed * cosf(angle2), (int)a.nSize >> 1, 0.0f });
							newAsteroids.push_back({ a.pos.x, a.pos.y, AsteroidSpeed * sinf(angle3), -AsteroidSpeed * cosf(angle3), (int)a.nSize >> 1, 0.0f });
						}

						if (BetweenEquationLevels == false) {
							bbRnd = (float)rand() / (float)RAND_MAX;
							//Drop Operations or Space Achors
							if (bbRnd < 0.5) {
								bbOpStr = GenerateOpStr();

								// Seems we need count our vectors; they don't have a count method. Really? I must check.
								bbi = 0;
								for (auto& c : vecOperations) bbi++;

								float angle4 = ((float)rand() / (float)RAND_MAX) * 6.283185f;
								if (bbi <= 2) {
									vecOperations.push_back({ a.pos.x, a.pos.y, 10.0f * sinf(angle4), -10.0f * cosf(angle4),  8, 0.0f, bbOpStr });
								} else {
									vecOperations.erase(vecOperations.begin());
									bbi = 0;
									for (auto& c : vecOperations) bbi++;
									if (bbi <= 2)
										vecOperations.push_back({ a.pos.x, a.pos.y, 10.0f * sinf(angle4), -10.0f * cosf(angle4), 8, 0.0f, bbOpStr });
								}
							} else {
								bbi = 0;
								for (auto& c : vecSpaceAnchors) bbi++;
								float angle5 = ((float)rand() / (float)RAND_MAX) * 6.283185f;
								if (bbi <= 2)
									vecSpaceAnchors.push_back({ a.pos.x, a.pos.y, 10.0f * sinf(angle5), -10.0f * cosf(angle5),  8, 0.0f });
								else {
									vecSpaceAnchors.erase(vecSpaceAnchors.begin());
									bbi = 0;
									// I am sure this would work vecSpaceAnchors.size()
									for (auto& c : vecSpaceAnchors) bbi++;
									if (bbi <= 2)
										vecSpaceAnchors.push_back({ a.pos.x, a.pos.y, 10.0f * sinf(angle5), -10.0f * cosf(angle5), 8, 0.0f });
								}
							}
						}
						else
						{
							bbi = 0;
							for (auto& c : vecTicks) bbi++;
							if (bbi <= 6) {
								float angle6 = ((float)rand() / (float)RAND_MAX) * 6.283185f;
								vecTicks.push_back({ a.pos.x, a.pos.y, 10.0f * sinf(angle6), -10.0f * cosf(angle6), 8, 0.0f });
							}
						}
						a.pos.x = -100; //Move asteroid off screen
						nScore += 100;

					}
				}
			}

			// Append new asteroids to existing vector
			for (auto a : newAsteroids)
				vecAsteroids.push_back(a);

			//Remove off screen bullets
			if (vecBullets.size() > 0) {
				auto i = remove_if(vecBullets.begin(), vecBullets.end(),
					[&](sSpaceObject o) {return (o.pos.x <= 1 || o.pos.y <= 1 || o.pos.x >= ScreenWidth() - 1 || o.pos.y >= ScreenHeight() - 1);});
				if (i != vecBullets.end()) vecBullets.erase(i, vecBullets.end());
			}

			//Remove off screen asteroids
			if (vecAsteroids.size() > 0) {
				auto i = remove_if(vecAsteroids.begin(), vecAsteroids.end(),
					[&](sSpaceObject o) {return (o.pos.x < -50);});
				if (i != vecAsteroids.end()) vecAsteroids.erase(i, vecAsteroids.end());
			}

			//Remove off screen operations
			if (vecAsteroids.size() > 0) {
				auto i = remove_if(vecOperations.begin(), vecOperations.end(),
					[&](sSpaceObject o) {return (o.pos.x < -50);});
				if (i != vecOperations.end()) vecOperations.erase(i, vecOperations.end());
			}

			//Remove off screen space anchors
			if (vecSpaceAnchors.size() > 0) {
				auto i = remove_if(vecSpaceAnchors.begin(), vecSpaceAnchors.end(),
					[&](sSpaceObject o) {return (o.pos.x < -50);});
				if (i != vecSpaceAnchors.end()) vecSpaceAnchors.erase(i, vecSpaceAnchors.end());
			}

			//Remove off screen ticks
			if (vecTicks.size() > 0) {
				auto i = remove_if(vecTicks.begin(), vecTicks.end(),
					[&](sSpaceObject o) {return (o.pos.x < -50);});
				if (i != vecTicks.end()) vecTicks.erase(i, vecTicks.end());
			}

			//Remove off screen bombs - do this in two bites
			if (vecBombs.size() > 0) {
				auto i = remove_if(vecBombs.begin(), vecBombs.end(),
					[&](sSpaceObject o) {return (o.pos.y > (ScreenHeight() + 100));});
				if (i != vecBombs.end()) vecBombs.erase(i, vecBombs.end());
			}

			if (vecBombs.size() > 0) {
				auto i = remove_if(vecBombs.begin(), vecBombs.end(),
					[&](sSpaceObject o) {return (o.pos.y < (-100));});
				if (i != vecBombs.end()) vecBombs.erase(i, vecBombs.end());
			}


			if (vecBombs.size() > 0) {
				auto i = remove_if(vecBombs.begin(), vecBombs.end(),
					[&](sSpaceObject o) {return (o.pos.x > (ScreenWidth() + 100));});
				if (i != vecBombs.end()) vecBombs.erase(i, vecBombs.end());
			}

			if (vecBombs.size() > 0) {
				auto i = remove_if(vecBombs.begin(), vecBombs.end(),
					[&](sSpaceObject o) {return (o.pos.x < (-100));});
				if (i != vecBombs.end()) vecBombs.erase(i, vecBombs.end());
			}

			// Remove off screen bouncing messages
			if (vecBouncingMessages.size() > 0) {
				auto i = remove_if(vecBouncingMessages.begin(), vecBouncingMessages.end(),
					[&](sSpaceObject o) {return (o.pos.y < (-50)); });
				if (i != vecBouncingMessages.end()) vecBouncingMessages.erase(i, vecBouncingMessages.end());
			}

			//Handle running out of asteroids.
			if (vecAsteroids.empty()) {
				nScore += 1000;
				// Here we are going to have to work on a repopulate asteroids based on level.
				bbAsteroidLevel++;
				if (bbAsteroidLevel == 4) bbAsteroidLevel = 1;
				LoadAsteroidLevel(bbAsteroidLevel);
			
			}

			// Draw Ship
			DrawWireFrameModel(vecModelShip, vecPlayers[0].pos.x, vecPlayers[0].pos.y, vecPlayers[0].angle, 1.0f);

			std::string bbStr = "Score: ";

			// Draw Score
			//DrawString(5, 10, bbStr + std::to_string(nScore), olc::WHITE, 10);
			//DrawString(5, 10, "Level" + std::to_string(bbAsteroidLevel), olc::WHITE, 10);
			//DrawString(5, 10,  "vec.size " + std::to_string(vecOperations.size()), olc::WHITE, 10);
			//DrawString(5, 128, "vec.capacity " + std::to_string(vecOperations.capacity()), olc::WHITE, 10);
			//DrawString(5, 256, "vec.rend " + std::to_string(vecOperations.rend), olc::WHITE, 10);
			//DrawString(5, 10, PresentOperation + PresentOperationVariable, olc::WHITE, 10);
			//DrawString(5, 100, ScreenOutputFlag1, olc::WHITE, 5);
			//DrawString(5, 150, ScreenOutputFlag2, olc::WHITE, 5);
			//DrawString(5, 200, ScreenOutputFlag3, olc::WHITE, 5);
			//DrawString(5, 250, ScreenOutputFlag4, olc::WHITE, 5);
			//DrawString(5, 300, ScreenOutputFlag5, olc::WHITE, 5);
			//DrawString(5, 350, ScreenOutputFlag6, olc::WHITE, 5);

			//DrawString(5, 300, std::to_string(Complexity), olc::WHITE, 5);

			//DrawString(5, 10, std::to_string(BetweenEquationLevels), olc::WHITE, 10);


			//Draw Equation
			//DrawString((int)(ScreenWidth() / 2), (int)(ScreenHeight() / 2), L"Equation!");
			//DrawString(5, 20, L"Equation ");

			// Get mouse location this frame
			//olc::vf2d vMouse = { (float)GetMouseX(), (float)GetMouseY() };

			// Draw a string
			//DrawString(64, 64, "Equation", olc::WHITE, 10);

			//Draw Sprite
			//DrawSprite(vMouse.x, vMouse.y, sprDemo);
			//DrawSprite(vMouse.x, vMouse.y, sprDemo);

			//Draw Equation
			if (!DoingLogarithms) {
				if (BetweenEquationLevels) DrawString(5, 10, sCongratulations, olc::WHITE, 10);
				else {
					DrawString(5, 10, sLevelInstruction, olc::WHITE, 10);
					DrawString(3, 100, "Level " + std::to_string(bbEquationLevel), olc::WHITE, 5);
				}

				EquationBlaster::ScreenOutputFlag3 = "Cap'n";
				// Prepare LHS for Pagination
				PrepareEquationSide(bbEquationPlayer1.LHS);

				// PAGINATE BABY!

				PaginatorPlayer1LHS.Reset();

				PaginatorPlayer1LHS.Paginator1(bbEquationPlayer1.LHS.SideOfEquationStr, (float)ScreenWidth(), (float)ScreenHeight());
				PaginatorPlayer1LHS.PaginateExpression(PaginatorPlayer1LHS.vecBrackets[0], bbEquationPlayer1.LHS.SideOfEquationStr);
				PaginatorPlayer1LHS.SomethingToDisplay = true;

				for (sSpaceText PaginatedText : PaginatorPlayer1LHS.vecBrackets[0].PaginatedText)
				{
					PaginatorPlayer1LHS.vecPaginatedText.push_back(PaginatedText);
				}

				LHSxOffset = (float)(-1 * (PaginatorPlayer1LHS.vecBrackets[0].nTotalLength + 6));
				LHSyOffset = (float)(2 * ScreenHeight() / 6);


				for (sSpaceText& sText : PaginatorPlayer1LHS.vecPaginatedText)
				{
					//sText.x += sText.dx * fElapsedTime;
					if (sText.bImADivisionBar == true)
					{
						DrawRect((int32_t)(LHSxOffset + sText.x), (int32_t)(LHSyOffset + sText.y), (int32_t)sText.nWidth, (int32_t)sText.nHeight, olc::WHITE);
						//DrawString(sText.x, sText.y, sText.str, olc::WHITE, sText.nFontSize);
					}
					else
					{
						DrawString((int32_t)(LHSxOffset + sText.x), (int32_t)(LHSyOffset + sText.y), sText.str, olc::WHITE, sText.nFontSize);
					}

				}


				// Draw Equals sign... kinda the point...
				
				DrawString((ScreenWidth() / 2) - 12, 5 * ScreenHeight() / 6, "=", olc::WHITE, 3);

			

				// Prepare RHS for Pagination

				PrepareEquationSide(bbEquationPlayer1.RHS);

				//PAGINATE BABY!

				PaginatorPlayer1RHS.Reset();

				PaginatorPlayer1RHS.Paginator1(bbEquationPlayer1.RHS.SideOfEquationStr, (float)ScreenWidth(), (float)ScreenHeight());
				PaginatorPlayer1RHS.PaginateExpression(PaginatorPlayer1RHS.vecBrackets[0], bbEquationPlayer1.RHS.SideOfEquationStr);
				PaginatorPlayer1RHS.SomethingToDisplay = true;
			
				for (sSpaceText PaginatedText : PaginatorPlayer1RHS.vecBrackets[0].PaginatedText)
				{
					PaginatorPlayer1RHS.vecPaginatedText.push_back(PaginatedText);
				}

				RHSxOffset = 24;
				RHSyOffset = (float)(2 * ScreenHeight() / 6);

				for (sSpaceText& sText : PaginatorPlayer1RHS.vecPaginatedText)
				{
					//sText.x += sText.dx * fElapsedTime;
					if (sText.bImADivisionBar == true)
					{
						DrawRect((int32_t)(RHSxOffset + sText.x), (int32_t)(RHSyOffset + sText.y), (int32_t)sText.nWidth, (int32_t)sText.nHeight, olc::WHITE);
						//DrawString(sText.x, sText.y, sText.str, olc::WHITE, sText.nFontSize);
					}
					else
					{
						DrawString((int32_t)(RHSxOffset + sText.x), (int32_t)(RHSyOffset + sText.y), sText.str, olc::WHITE, sText.nFontSize);
					}

				}

				//DrawString(ScreenWidth() / 2, 200, bbEquation.RHS.SideOfEquationStr, olc::WHITE, 1);
			}
			else
			{
				int i = 0;
				int j = 0;
				for (Logarithm a : vecAppropriateLogarithms)
				{
					j++;
					i = i + 32;
					if (a.Achieved == true) 
					{
						DrawString(50, 64 + i, std::to_string(a.AntiLog), olc::BLUE, 4);
					}
					else
					{
						DrawString(50, 64 + i, std::to_string(a.AntiLog), olc::WHITE, 4);
					}

				}

				//DrawString(5, 10, "Doing Logarithms", olc::WHITE, 10);
				DrawWireFrameModel(vecModelLogarithmTriangle, (float)(ScreenWidth() / 2), (float)(ScreenHeight() / 2), 0.0f, 20.0f, olc::BLUE);

				//DrawWireFrameModel(vecModelBomb, ScreenWidth() / 4, ScreenHeight() / 2, 0.0f, 10.0f, olc::WHITE);


				//Locate triangle corners
				//Logarithm TargetLog = vecAppropriateLogarithms[j-1];

				bbStrLen = 0;
				for (char a : std::to_string(TriangleLogarithm.Log))
					bbStrLen++;

				if (std::to_string(TriangleLogarithm.Log) != "0")
					DrawString((ScreenWidth() / 2) - (16 * bbStrLen), (ScreenHeight() / 5) - 24, std::to_string(TriangleLogarithm.Log), olc::WHITE, 4);

				bbStrLen = 0;
				for (char a : std::to_string(TriangleLogarithm.Base))
					bbStrLen++;

				if (std::to_string(TriangleLogarithm.Base) != "0")
					DrawString((ScreenWidth() / 3) - (16 * bbStrLen), 4 * ScreenHeight() / 5, std::to_string(TriangleLogarithm.Base), olc::WHITE, 4);

				bbStrLen = 0;
				for (char a : std::to_string(TriangleLogarithm.AntiLog))
					bbStrLen++;

				if (std::to_string(TriangleLogarithm.AntiLog) != "0")
					DrawString((2 * ScreenWidth() / 3) - (16 * bbStrLen), 4 * ScreenHeight() / 5, std::to_string(TriangleLogarithm.AntiLog), olc::WHITE, 4);

			}
			//Investigate string parameters


			//Size 1 - 8 pixels
			//DrawString(0.0f, 1 * ScreenHeight() / 6, bbTestStr, olc::BLUE, 1);

			//Size 2 - 16 pixels
			//DrawString(0.0f, 2 * ScreenHeight() / 6, bbTestStr, olc::GREEN, 2);

			//Size 3 - 24 pixels
			//DrawString(0.0f, 1 * ScreenHeight() / 6, bbTestStr, olc::WHITE, 3);

			//Size 4 - 32 Pixels
			//DrawString(0.0f, 2 * ScreenHeight() / 6, bbTestStr, olc::WHITE, 4);

			//Size 5 - 40 Pixels
			//DrawString(0.0f, 3 * ScreenHeight() / 6, bbTestStr, olc::WHITE, 5);

			// Display Number of Space Anchors
			DrawString(1120, 80, std::to_string(AvailableSpaceAnchorsPlayer1), olc::WHITE, 5);
		}

		return true;
	
	}

	void DrawWireFrameModel(const std::vector<std::pair<float, float>>& vecModelCoordinates, float x, float y, float r = 0.0f, float s = 1.0f, olc::Pixel p = olc::WHITE,  uint32_t pattern = 0xFFFFFFFF)
	{
		// pair.first = x coordinate
		// pair.second = y coordinate

		// Create translated model vector of coordiante pairs
		std::vector<std::pair<float, float>> vecTransformedCoordinates;
		int verts = vecModelCoordinates.size();
		vecTransformedCoordinates.resize(verts);


		// Rotate
		for (int i = 0; i < verts; i++)
		{
			vecTransformedCoordinates[i].first = vecModelCoordinates[i].first * cosf(r) - vecModelCoordinates[i].second * sinf(r);
			vecTransformedCoordinates[i].second = vecModelCoordinates[i].first * sinf(r) + vecModelCoordinates[i].second * cosf(r);
		}

		// Scale
		for (int i = 0; i < verts; i++)
		{
			vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first * s;
			vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second * s;
		}

		// Translate
		for (int i = 0; i < verts; i++)
		{
			vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first + x;
			vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second + y;

		}

		// Draw Closed Polygon
		for (int i = 0; i < verts; i++)
		{
			int j = (i + 1);
			//DrawLine(vecTransformedCoordinates[i % verts].first, vecTransformedCoordinates[i % verts].second, vecTransformedCoordinates[j % verts].first, vecTransformedCoordinates[j % verts].second, olc::WHITE, pattern);
			DrawLine((int32_t)vecTransformedCoordinates[i % verts].first, (int32_t)vecTransformedCoordinates[i % verts].second, (int32_t)vecTransformedCoordinates[j % verts].first, (int32_t)vecTransformedCoordinates[j % verts].second, p, pattern);

		}


	}

	void ReturnTransformedCoordinates(polygon &polyref, const std::vector<std::pair<float, float>>& vecModelCoordinates, float x, float y, float r = 0.0f, float s = 1.0f)
	{

		polyref.pos.x = x;
		polyref.pos.y = y;
		polyref.angle = r;
		
		polyref.p.clear();
		//vec2d tempVec2d;

		
		// Create translated model vector of coordiante pairs
		std::vector<std::pair<float, float>> vecTransformedCoordinates;
		int verts = vecModelCoordinates.size();
		vecTransformedCoordinates.resize(verts);
		//ScreenOutputFlag1 = "In the damn function Cap'n";
		// Rotate
		for (int i = 0; i < verts; i++)
		{
			vecTransformedCoordinates[i].first = vecModelCoordinates[i].first * cosf(r) - vecModelCoordinates[i].second * sinf(r);
			vecTransformedCoordinates[i].second = vecModelCoordinates[i].first * sinf(r) + vecModelCoordinates[i].second * cosf(r);
		}

		// Scale
		for (int i = 0; i < verts; i++)
		{
			vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first * s;
			vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second * s;
		}

		// Translate
		for (int i = 0; i < verts; i++)
		{
			vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first + x;
			vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second + y;
		}

		//Copy to Polygon
		for (int i = 0; i < verts; i++)
		{

			vec2d tempVec2d;
			tempVec2d.x = vecTransformedCoordinates[i].first;
			tempVec2d.y = vecTransformedCoordinates[i].second;

			polyref.p.push_back(tempVec2d);
		}

		//ScreenOutputFlag1 = std::to_string(polyref.pos.x) + " pos " + std::to_string(polyref.pos.y);
		//ScreenOutputFlag2 = std::to_string(polyref.p[0].x) + " p[0] " + std::to_string(polyref.p[0].y);
		//ScreenOutputFlag3 = std::to_string(polyref.p[1].x) + " p[1] " + std::to_string(polyref.p[1].y);
		//ScreenOutputFlag4 = std::to_string(polyref.p[2].x) + " p[2] " + std::to_string(polyref.p[2].y);
		//ScreenOutputFlag5 = std::to_string(vecModelCoordinates[1].first) + " vmc[1] " + std::to_string(vecModelCoordinates[1].second);
		//ScreenOutputFlag6 = std::to_string(vecModelCoordinates[2].first) + " vmc[2] " + std::to_string(vecModelCoordinates[2].second);
		//ScreenOutputFlag6 = std::to_string(s) + " s : r" + std::to_string(r);

	}

	bool ShapeOverlap_SAT(polygon& r1, polygon& r2)
	{

		polygon* poly1 = &r1;
		polygon* poly2 = &r2;

		ScreenOutputFlag1 = std::to_string(poly1->p[2].x) + " " + std::to_string(poly2->p[2].x);

		for (int shape = 0; shape < 2; shape++)
		{
			if (shape == 1)
			{
				poly1 = &r2;
				poly2 = &r1;
			}

			for (size_t a = 0; a < poly1->p.size(); a++)
			{
				int b = (a + 1) % poly1->p.size();
				vec2d axisProj = { -(poly1->p[b].y - poly1->p[a].y), poly1->p[b].x - poly1->p[a].x };
				float d = sqrtf(axisProj.x * axisProj.x + axisProj.y * axisProj.y);
				axisProj = { axisProj.x / d, axisProj.y / d };

				// Work out min and max 1D points for r1
				//float min_r1 = INFINITY, max_r1 = -INFINITY;
				float min_r1 = 1000000000, max_r1 = -1000000000;
				for (size_t p = 0; p < poly1->p.size(); p++)
				{
					float q = (poly1->p[p].x * axisProj.x + poly1->p[p].y * axisProj.y);
					//min_r1 = std::min(min_r1, q);
					min_r1 = (min_r1 < q) ? min_r1 : q;
					//max_r1 = std::max(max_r1, q);
					max_r1 = (max_r1 > q) ? max_r1 : q;
				}

				// Work out min and max 1D points for r2
				float min_r2 = 1000000000, max_r2 = -1000000000;
				for (size_t p = 0; p < poly2->p.size(); p++)
				{
					float q = (poly2->p[p].x * axisProj.x + poly2->p[p].y * axisProj.y);
					//min_r2 = std::min(min_r2, q);
					min_r2 = (min_r2 < q) ? min_r2 : q;
					//max_r2 = std::max(max_r2, q);
					max_r2 = (max_r2 > q) ? max_r2 : q;
				}

				if (!(max_r2 >= min_r1 && max_r1 >= min_r2))
					return false;
			}
		}

		return true;
	}

	bool ShapeOverlap_DIAGS(polygon& r1, polygon& r2)
	{
		polygon* poly1 = &r1;
		polygon* poly2 = &r2;

		//ScreenOutputFlag1 = std::to_string(poly1->p[2].x) + " " + std::to_string(poly2->p[2].x);
		//ScreenOutputFlag1 = std::to_string(poly1->pos.x) + " " + std::to_string(poly2->pos.x);

		for (int shape = 0; shape < 2; shape++)
		{
			if (shape == 1)
			{
				poly1 = &r2;
				poly2 = &r1;
			}

			// Check diagonals of polygon...
			for (size_t p = 0; p < poly1->p.size(); p++)
			{
				vec2d line_r1s = poly1->pos;
				//line_r1s.x = poly1->pos.x;
				//line_r1s.y = poly1->pos.y;
				vec2d line_r1e = poly1->p[p];
				//line_r1e.x = poly1->p[p].x;
				//line_r1e.y = poly1->p[p].y;

				// ...against edges of the other
				for (size_t q = 0; q < poly2->p.size(); q++)
				{
					vec2d line_r2s = poly2->p[q];
					vec2d line_r2e = poly2->p[(q + 1) % poly2->p.size()];
					
					//ScreenOutputFlag3 = std::to_string(line_r1s.x) + " r1s " + std::to_string(line_r1s.y);
					//ScreenOutputFlag4 = std::to_string(line_r1e.x) + " r1e " + std::to_string(line_r1e.y);
					//ScreenOutputFlag5 = std::to_string(line_r2s.x) + " r2s " + std::to_string(line_r2s.y);
					//ScreenOutputFlag6 = std::to_string(line_r2e.x) + " r2e " + std::to_string(line_r2e.y);

					// Standard "off the shelf" line segment intersection
					float h = (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r1e.y) - (line_r1s.x - line_r1e.x) * (line_r2e.y - line_r2s.y);
					float t1 = ((line_r2s.y - line_r2e.y) * (line_r1s.x - line_r2s.x) + (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r2s.y)) / h;
					float t2 = ((line_r1s.y - line_r1e.y) * (line_r1s.x - line_r2s.x) + (line_r1e.x - line_r1s.x) * (line_r1s.y - line_r2s.y)) / h;

					//ScreenOutputFlag5 = std::to_string(t1) + " " + std::to_string(t2);
					//ScreenOutputFlag5 = std::to_string(poly1->p.size());
					//ScreenOutputFlag6 = std::to_string(poly2->p.size());

					//ScreenOutputFlag1 = std::to_string(poly1->pos.x) + " p1 pos " + std::to_string(poly1->pos.y);
					//ScreenOutputFlag2 = std::to_string(poly2->pos.x) + " p2 pos " + std::to_string(poly2->pos.y);

					if (t1 >= 0.0f && t1 < 1.0f && t2 >= 0.0f && t2 < 1.0f)
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	void WrapCoordinates(float ix, float iy, float& ox, float& oy)
	{
		ox = ix;
		oy = iy;
		if (ix < 0.0f) ox = ix + (float)ScreenWidth();
		if (ix > (float)ScreenWidth()) ox = ix - (float)ScreenWidth();
		if (iy < 0.0f) oy = iy + (float)ScreenHeight();
		if (iy > (float)ScreenHeight()) oy = iy - (float)ScreenHeight();
	}

	virtual void Draw(int x, int y, short c = 0x2588, short col = 0x000F)
	{
		float fx, fy;
		WrapCoordinates((float)x, (float)y, fx, fy);

		olc::vi2d bbPos;
		bbPos.x = int(fx);
		bbPos.y = int(fy);

		PixelGameEngine::Draw(bbPos, olc::WHITE);
		//olcConsoleGameEngine::Draw(fx, fy, c, col);
		//Draw(const olc::vi2d & pos, Pixel p = olc::WHITE)
	}
};

int main()
{
	EquationBlaster demo;
	//So we will work of a World Area of this plus 100 all round so -100-1300, -100-740.
	if (demo.Construct(1200, 640, 1, 1))
		demo.Start();
	return 0;
}
