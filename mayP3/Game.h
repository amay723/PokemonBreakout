// File: Game.h
// Author: 
// Project: CS215 Project 3 Fall 2016
// Description of file contents:

#ifndef GAME_H
#define GAME_H

#include <afxwin.h>

#define NUMIMAGES 18
#define MIN_TIME 30

enum square_types {PATH, WALL, ME, GOAL};
enum image_type {MEimg = 0, PATHimg= 2, WALLimg, GOALimg}; 

class Game
{
	public:
		Game ();
		Game (const Game & other);
		~Game ();
		Game & operator = (const Game & other);
		void Init (int R, int C, CFrameWnd * windowP);
		void Instructions (CFrameWnd * windowP);
		void Display (CFrameWnd * windowP);
		void Click (int x, int y, CFrameWnd * windowP);
		void Move (char dir, CFrameWnd * windowP);
		void Message (CFrameWnd * windowP);
		void UpdateTime (CFrameWnd * windowP);	
		void CheckGoal(int row, int col);
		void NewMon(CFrameWnd * windowP);
		bool Done (CFrameWnd * windowP);
	private:
		void Carve (int row, int col);
		void SetUp (CRect window);
		void DisplayGameRect (CDC * deviceContextP);
		void DisplayScoreRect (CDC * deviceContextP);
		void DisplaySpecialRect (CDC * deviceContextP);

		struct GameSquare
		{
			GameSquare ();
			void Display (CDC * deviceContextP);
			square_types what;
			int points;	// for goal
			int imageid;
			CRect where;
		};

		GameSquare ** grid;
		int numRows;
		int numCols;
		int score;
		int timeLeft;
		int currentRow, currentCol;
		int endRow, endCol;
		CRect gameRect;
		CRect specialRect;
		CRect scoreRect;
		CRect timeRect;
		CRect gamescoreRect;
		CBitmap bgImage;
};

#endif
