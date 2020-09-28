// File: Game.cpp
// Author: Adam May
// Project: CS215 Project 3 Fall 2016
// Description of file contents:

#include "Game.h"
#include <time.h>
#include <string>

static CBitmap images [NUMIMAGES];

using namespace std;

Game::GameSquare::GameSquare ()
{
	// This function will:

	what = WALL;
	imageid = WALLimg;
}

void Game::GameSquare::Display (CDC * deviceContextP)
{
	// This function will:

	CDC memDC;
	int res = memDC.CreateCompatibleDC(deviceContextP);
	memDC.SelectObject(&images[imageid]);
	deviceContextP->TransparentBlt (where.left, where.top, where.Width()+1,where.Height()+1,&memDC, 0, 0, 120, 90, RGB(255, 255, 255)); 
	DeleteDC (memDC);	
}

Game::Game ()
{
	// This function will load all of the images ready for use

	srand (time (NULL));
	numRows = numCols = 1;
	grid = new GameSquare * [numRows];
	for (int r = 0; r < numRows; r++)
		grid[r] = new GameSquare [numCols];
	int res = bgImage.LoadBitmap(CString("BACKGROUND_BMP"));
	res = images[PATHimg].LoadBitmap(CString("PATH_BMP"));		
	res = images[WALLimg].LoadBitmap(CString("WALL_BMP"));
	res = images[MEimg+0].LoadBitmap(CString("CHARACTER_BMP")); // CHARACTER DOWN
	res = images[MEimg+1].LoadBitmap(CString("CHARACTER_UP_BMP")); // CHARACTER UP
	


	res = images[GOALimg+0].LoadBitmap(CString("MEW_BMP")); // Mew
	res = images[GOALimg+1].LoadBitmap(CString("SNOZZLE_BMP")); // Snorlax
	res = images[GOALimg+2].LoadBitmap(CString("PIKA_BMP")); // Pikachu
	res = images[GOALimg+3].LoadBitmap(CString("SLOWP_BMP")); // Slowpoke
	res = images[GOALimg + 4].LoadBitmap(CString("PSYDUCK_BMP")); // Psyduck

	res = images[GOALimg+5].LoadBitmap(CString("CHARACTER_LT_BMP")); // CHARACTER LEFT
	res = images[GOALimg+6].LoadBitmap(CString("CHARACTER_RT_BMP")); // CHARACTER RIGHT

	res = images[GOALimg + 7].LoadBitmap(CString("PIDDY_BMP")); // Pidgey
	res = images[GOALimg + 8].LoadBitmap(CString("VOLT_BMP")); // Voltorb
	res = images[GOALimg + 9].LoadBitmap(CString("POLIWRATH_BMP")); // Poliwrath

	res = images[GOALimg + 10].LoadBitmap(CString("ZAPDOS_BMP")); // Zapdos
	res = images[GOALimg + 11].LoadBitmap(CString("MOLTRES_BMP")); // Moltres
	res = images[GOALimg + 12].LoadBitmap(CString("ARTICUNO_BMP")); // Articuno

}

Game::~Game ()
{
	// This function will deconstruct the grid/maze

	for (int r = 0; r < numRows; r++)
		delete [] grid[r];
	delete [] grid;
}

void Game::Init (int R, int C, CFrameWnd * windowP)
{
	// This function will construct the grid
	for (int r = 0; r < numRows; r++)
		delete [] grid[r];
	delete [] grid;
	numRows = R;
	numCols = C;
	currentRow = currentCol = 1;
	endRow = numRows - 2;
	endCol = numCols - 2;
	score = 0;
	timeLeft = MIN_TIME;
	grid = new GameSquare * [numRows];
	for (int r = 0; r < numRows; r++)
		grid[r] = new GameSquare [numCols];
	Carve (1,1);
	grid[currentRow][currentCol].what = ME;
	grid[currentRow][currentCol].imageid = MEimg+0;
	grid[endRow][endCol].what = GOAL;
	grid[endRow][endCol].imageid = GOALimg;
	
	int mons = 0;
	int r, c = 0;

	int i = 0;

	while (mons < 4) // Spaws in 4 random mons
	{
		r = rand() % endRow;
		c = rand() % endCol;
		
		i = rand() % 7;

		if (grid[r][c].what == PATH)
		{
			grid[r][c].what = GOAL;
			switch (i)
			{
			case 0:
				grid[r][c].imageid = GOALimg+1; // Snorlax
				break;
			case 1:
				grid[r][c].imageid = GOALimg+2; // Pikachu
				break;
			case 2:
				grid[r][c].imageid = GOALimg+3; // Slowpoke
				break;
			case 3:
				grid[r][c].imageid = GOALimg + 4; // Psyduck
				break;
			case 4:
				grid[r][c].imageid = GOALimg + 7; // Pidgey
				break;
			case 5:
				grid[r][c].imageid = GOALimg + 8; // Voltorb
				break;
			case 6:
				grid[r][c].imageid = GOALimg + 9; // Poliwrath
				break;
			}
			
			mons++;
		}
	}
	
}

void Game::Instructions (CFrameWnd * windowP)
{
	// This function will displat the instrucitons

	CString message = "Use WASD to move around the maze. \n Catch 'em all before the timer runs out!";
	CString title = "Instructions";
	windowP->MessageBox (message, title);
}

// Replace this function with the function(s) used to generate your maze
void Game::Carve(int row, int col)
{
	// This function creates a new maze by using the depth first carve method                                                                                                          
	// It checks to see if possible new sections to carve are empty or                                                                                                                 
	// otherwise available to carve into.                                                                                                                                              
	grid[row][col].what = PATH;
	grid[row][col].imageid = PATHimg;
	int rd = (rand() % 4);
	for (int d = 0; d < 4; d++)
	{
		if (rd == 0 && col + 2 < numCols && grid[row][col + 2].what != PATH)
		{
			grid[row][col + 1].what = PATH;
			grid[row][col + 1].imageid = PATHimg;
			Carve(row, col + 2);
		}
		else if (rd == 1 && row - 2 > 0 && grid[row - 2][col].what != PATH)
		{
			grid[row - 1][col].what = PATH;
			grid[row - 1][col].imageid = PATHimg;
			Carve(row - 2, col);
		}
		else if (rd == 2 && col - 2 > 0 && grid[row][col - 2].what != PATH)
		{
			grid[row][col - 1].what = PATH;
			grid[row][col - 1].imageid = PATHimg;
			Carve(row, col - 2);
		}
		else if (rd == 3 && row + 2 < numRows && grid[row + 2][col].what != PATH)
		{
			grid[row + 1][col].what = PATH;
			grid[row + 1][col].imageid = PATHimg;
			Carve(row + 2, col);
		}
		rd = (rd + 1) % 4;
	}

	
}


void Game::Display (CFrameWnd * windowP)
{
	// This function will displat the window

	CPaintDC dc (windowP);
	CRect rect;
	windowP->GetClientRect (&rect);

	CDC memDC;
	int res = memDC.CreateCompatibleDC(&dc);
	memDC.SelectObject(&bgImage);
	dc.TransparentBlt (0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, 2000, 1000, SRCCOPY); 
	DeleteDC (memDC);

	SetUp (rect);
	DisplayGameRect (&dc);
	dc.SetBkMode(TRANSPARENT);	
	CFont font;
	CString fontName = "Comic Sans MS";
	font.CreatePointFont(rect.Height() / 4, fontName, &dc);
	CFont* def_font = dc.SelectObject(&font);
	COLORREF def_color = dc.SetTextColor (RGB (55, 65, 195));
	DisplayScoreRect (&dc);
	DisplaySpecialRect (&dc);
	dc.SelectObject(def_font);
	dc.SetTextColor(def_color);}

void Game::SetUp (CRect rect)
{
	// This function will: Build all the different rects of the game, proportioning them with each other and the window size

	int thirdW = rect.Width() / 3;
	int twelfthW = rect.Width() / 12;
	int twelfthH = rect.Height() / 12;

	gameRect = CRect (10, 10, 2* thirdW - 10, rect.Height()-10);
	specialRect = CRect (2 * thirdW + 10, 2 * rect.Height() / 3, rect.Width() - 10, 10 * twelfthH);
	scoreRect = CRect (specialRect.left, (specialRect.top + specialRect.bottom) / 2, specialRect.right, rect.Height() - 10);

	int sqHeight = gameRect.Height() / numRows;
	int sqWidth = gameRect.Width() / numCols;
	int top = gameRect.top + (gameRect.Height() - sqHeight * numRows) / 2;
	int left = gameRect.left + (gameRect.Width() - sqWidth * numCols) / 2;
	for (int r = 0; r < numRows; r++)
		for (int c = 0; c < numCols; c++)
			grid[r][c].where = CRect(left + c*sqWidth, top + r*sqHeight, left + (c+1)*sqWidth, top + (r+1)*sqHeight);

	int thirdH = specialRect.Height() / 3;
	int halfW = specialRect.Width() / 2;
	timeRect = CRect (specialRect.left + halfW, specialRect.top, specialRect.right - halfW/2, (specialRect.top + specialRect.bottom) / 2);

	thirdH = scoreRect.Height() / 3;
	gamescoreRect = CRect (scoreRect.left, scoreRect.top, scoreRect.right, scoreRect.bottom - 10);
}

void Game::DisplayGameRect (CDC * deviceContextP)
{
	// This function will display the maze onto the window

	//deviceContextP->Rectangle(gameRect);
	for (int r = 0; r < numRows; r++)
		for (int c = 0; c < numCols; c++)
			grid [r][c].Display (deviceContextP);
}

void Game::DisplayScoreRect (CDC * deviceContextP)
{
	// This function will display the score rect onto the window

	deviceContextP->Rectangle(scoreRect);
	deviceContextP->Rectangle(gamescoreRect);
	char buffer [50];
	sprintf_s (buffer, "Score: %d", score);
	deviceContextP->DrawText(CString(buffer), gamescoreRect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
}

void Game::DisplaySpecialRect (CDC * deviceContextP)
{
	// This function will display the time rect onto the window

	deviceContextP->Rectangle(specialRect);
	deviceContextP->Rectangle(timeRect);
	char buffer [20];
	sprintf_s (buffer, "%d:%02d", timeLeft/60, (timeLeft < 0 ? 0 : timeLeft%60));
	deviceContextP->DrawText("Time:", CRect(specialRect.left, specialRect.top, timeRect.left, timeRect.bottom), DT_SINGLELINE | DT_VCENTER | DT_RIGHT);
	deviceContextP->DrawText(CString (buffer), timeRect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
}

void Game::Click (int y, int x, CFrameWnd * windowP)
{
	// This function will: not do anything, has not been used, but would have been used for buttons

}

void Game::Move (char direction, CFrameWnd * windowP)
{
	// This function will move your character based off keyboard inputs

	int z = 0;

	CRect oldWhere = grid[currentRow][currentCol].where | grid[currentRow-1][currentCol].where;
	switch (direction)
	{
		case 'a': // moving left

			if (grid[currentRow][currentCol - 1].what == WALL) // Do nothing if Player tries to walk into a WALL
				break;
			
			if (grid[currentRow][currentCol].imageid != GOALimg + 5) // If the Player image is not facing left, change
				grid[currentRow][currentCol].imageid = GOALimg + 5;  // the Player image to face left

				
			if (grid[currentRow][currentCol - 1].what == GOAL) // If the Player moves onto a GOAL, adds points depending on
			{												   // which goal was achieved.
				z = 1;
				CheckGoal(currentRow, currentCol - 1);
			}


			grid[currentRow][currentCol - 1].what = grid[currentRow][currentCol].what; // Value at new block is set to current block
			grid[currentRow][currentCol].what = PATH; // Old block is set to a PATH
			grid[currentRow][currentCol - 1].imageid = grid[currentRow][currentCol].imageid; // New block image is set to old block image
			grid[currentRow][currentCol].imageid = PATHimg; // Old block image is set to PATHimg
			
			if (z)
				NewMon(windowP); // Redraws window if GOAL has been achieved (used to spawn another GOAL)

			currentCol--; // Adjusts currentCol to new position
			break;

		case 'w': // moving up
			if (grid[currentRow - 1][currentCol].what == WALL) // Do nothing if Player tries to walk into a WALL
				break;
			
			if (grid[currentRow][currentCol].imageid != MEimg+1) // If the Player image is not facing UP, change
				grid[currentRow][currentCol].imageid = MEimg+1;  // the Player image to face UP


			if (grid[currentRow - 1][currentCol].what == GOAL) // If the Player moves onto a GOAL, adds points depending on
			{												   // which GOAL was achieved.
				z = 1;
				CheckGoal(currentRow - 1, currentCol);
			}

			grid[currentRow - 1][currentCol].what = grid[currentRow][currentCol].what; // Value at new block is set to current block
			grid[currentRow][currentCol].what = PATH; // Old block is set to a PATH
			grid[currentRow - 1][currentCol].imageid = grid[currentRow][currentCol].imageid; // New block image is set to old block image
			grid[currentRow][currentCol].imageid = PATHimg; // Old block image is set to PATHimg
			
			if (z)
				NewMon(windowP); // Redraws window if GOAL has been achieved (used to spawn another GOAL)

			currentRow--; // Adjusts currentRow to new position
			break;

		case 'd': // moving right
			if (grid[currentRow][currentCol + 1].what == WALL) // Do nothing if Player tries to walk into a WALL
				break;

			if (grid[currentRow][currentCol].imageid != GOALimg + 6) // If the Player image is not facing right, change
				grid[currentRow][currentCol].imageid = GOALimg + 6;  // the Player image to face right
				

			if (grid[currentRow][currentCol + 1].what == GOAL) // If the Player moves onto a GOAL, adds points depending on
			{												   // which GOAL was achieved.
				z = 1;
				CheckGoal(currentRow, currentCol + 1);
			}
			
			grid[currentRow][currentCol + 1].what = grid[currentRow][currentCol].what; // Value at new block is set to current block
			grid[currentRow][currentCol].what = PATH; // Old block is set to a PATH
			grid[currentRow][currentCol + 1].imageid = grid[currentRow][currentCol].imageid; // New block image is set to old block image
			grid[currentRow][currentCol].imageid = PATHimg; // Old block image is set to PATHimg
			
			if (z)
				NewMon(windowP);  // Redraws window if GOAL has been achieved (used to spawn another GOAL)

			currentCol++; // Adjusts currentCol to new position
			break;

		case 's': // moving down

			if (grid[currentRow + 1][currentCol].what == WALL) // Do nothing if Player tries to walk into a WALL
				break;

			if (grid[currentRow][currentCol].imageid != MEimg + 0) // If the Player image is not facing down, change
				grid[currentRow][currentCol].imageid = MEimg + 0;  // the Player image to face down
				

			if (grid[currentRow + 1][currentCol].what == GOAL) // If the Player moves onto a GOAL, adds points depending on
			{												   // which GOAL was achieved.
				z = 1;
				CheckGoal(currentRow + 1, currentCol);
			}

			
			grid[currentRow + 1][currentCol].what = grid[currentRow][currentCol].what; // Value at new block is set to current block
			grid[currentRow][currentCol].what = PATH; // Old block is set to a PATH
			grid[currentRow + 1][currentCol].imageid = grid[currentRow][currentCol].imageid; // New block image is set to old block image
			grid[currentRow][currentCol].imageid = PATHimg; // Old block image is set to PATHimg
			
			if (z)
				NewMon(windowP); // Redraws window if GOAL has been achieved (used to spawn another GOAL)

			currentRow++; // Adjusts currentRow to new position
			break;
	}
	CRect newWhere = grid[currentRow][currentCol].where | grid[currentRow-1][currentCol].where;
	windowP->InvalidateRect (oldWhere | newWhere); // Redraws the character from the old to new path
	windowP->InvalidateRect(scoreRect); // Redraws score rect (used to update the score value on the window)
	windowP->InvalidateRect(timeRect);
}

void Game::Message(CFrameWnd * windowP)
{
	// This function will show the end of game window, showing player score.

	CString message, title;


	message = "Nice collecting, now get ready for the next round!";
	title = "Congratulations!";

	windowP->KillTimer(1);
	windowP->MessageBox(message, title);

}
bool Game::Done (CFrameWnd * windowP)
{
	// This function will check to see if the game time is over (reached 0)

	if (timeLeft < 1)
	{
		return true;
	}
	return false;
}

void Game::UpdateTime (CFrameWnd * windowP)
{
	// This function will: Update the elapsed time each time it is called by OnTimer.

	timeLeft--;
	if (timeLeft <= 0)
		windowP->KillTimer(1);
	windowP->InvalidateRect (timeRect);
}

void Game::CheckGoal(int row, int col)
{
	int r, c = 0;
	int z = 0;

	int j = 1;
	
	
	if (grid[row][col].imageid == GOALimg + 0) // Mew
	{
		score += 100;
		timeLeft += 4;
	}
	else if (grid[row][col].imageid == GOALimg + 1) // Snorlax
	{
		score += 40;
		timeLeft += 1;
	}
	else if (grid[row][col].imageid == GOALimg + 2) // Pikachu
	{
		score += 30;
		timeLeft += 3;
	}
	else if (grid[row][col].imageid == GOALimg + 3) // Slowpoke
	{
		score += 15;

		z = rand() % 10;
		if (z >= 6) // 30% chance
		{
			while (j)
			{
				r = rand() % endRow;
				c = rand() % endCol;

				if (grid[r][c].what == PATH)
				{
					grid[r][c].what = GOAL;
					grid[r][c].imageid = GOALimg + 12; // Spawns Articuno
					j = 0;
				}

			}
		}
	}
	else if (grid[row][col].imageid == GOALimg + 4) // Psyduck
	{
		score += 15;

		z = rand() % 10;
		if (z >= 6) // 30% chance
		{
			while (j)
			{
				r = rand() % endRow;
				c = rand() % endCol;

				if (grid[r][c].what == PATH)
				{
					grid[r][c].what = GOAL;
					grid[r][c].imageid = GOALimg + 0; // Spawns Mew
					j = 0;
				}
			}
		}
	}
	else if (grid[row][col].imageid == GOALimg + 7) // Pidgey
	{
		score += 10;

		z = rand() % 10;
		if (z >= 6) // 30% chance
		{
			while (j)
			{
				r = rand() % endRow;
				c = rand() % endCol;

				if (grid[r][c].what == PATH)
				{
					grid[r][c].what = GOAL;
					grid[r][c].imageid = GOALimg + 11; // Spawns Moltres
					j = 0;
				}
			}
		}
	}
	else if (grid[row][col].imageid == GOALimg + 8) // Voltorb
	{
		score += 15;

		z = rand() % 10;
		if (z >= 6) // 30% chance
		{
			while (j)
			{
				r = rand() % endRow;
				c = rand() % endCol;

				if (grid[r][c].what == PATH)
				{
					grid[r][c].what = GOAL;
					grid[r][c].imageid = GOALimg + 10; // Spawns Zapdos
					j = 0;
				}

			}
		}
	}
	else if (grid[row][col].imageid == GOALimg + 9) // Poliwrath
	{
		score += 25;
	}
	else if (grid[row][col].imageid == GOALimg + 10) // Zapdos
	{
		score += 70;
		timeLeft += 4;
	}
	else if (grid[row][col].imageid == GOALimg + 11) // Moltres
	{
		score += 70;
		timeLeft += 4;
	}
	else if (grid[row][col].imageid == GOALimg + 12) // Articuno
	{
		score += 70;
		timeLeft += 4;
	}
}

void Game::NewMon(CFrameWnd * windowP) // This function randomly spawns in a new monster and is called only when a previous monster
{									   // has been captured.


	int r, c = 0;

	int i = 0;
	int z = 1;

	while (z)
	{
		r = rand() % endRow;
		c = rand() % endCol;


		i = rand() % 7; // Allows the new monster to be randomly chosen

		if (grid[r][c].what == PATH)
		{
			grid[r][c].what = GOAL;
			switch (i)
			{
			case 0:
				grid[r][c].imageid = GOALimg + 1; // Snorlax
				z = 0;
				break;
			case 1:
				grid[r][c].imageid = GOALimg + 2; // Pikachu
				z = 0;
				break;
			case 2:
				grid[r][c].imageid = GOALimg + 3; // Slowpoke
				z = 0;
				break;
			case 3:
				grid[r][c].imageid = GOALimg + 4; // Psyduck
				z = 0;
				break;
			case 4:
				grid[r][c].imageid = GOALimg + 7; // Pidgey
				z = 0;
				break;
			case 5:
				grid[r][c].imageid = GOALimg + 8; // Voltorb
				z = 0;
				break;
			case 6:
				grid[r][c].imageid = GOALimg + 9; // Poliwrath
				z = 0;
				break;
			}
		}
	}

	windowP->InvalidateRect(grid[r][c].where);
}