// File: GameWin.cpp
// Author: 
// Project: CS215 Project 3 Fall 2016
// Description of file contents:

#include <afxwin.h>
#include "GameWin.h"

GameWin::GameWin ()
{
// This function will display the window and initialize the grid

		CString WindowTitle = "Pokemon Breakout!";
		Create (NULL, WindowTitle);
		numRows = 15;
		numCols = 19;
		myGame.Init(numRows, numCols, this);
		firstTime = true;
}

afx_msg void GameWin::OnPaint ()
{
// This function will display and intialize the timer

		myGame.Display (this);
		if (firstTime)
		{
			myGame.Instructions(this);
			firstTime = false;
			SetTimer (1, 1000, NULL);
		}
}

afx_msg void GameWin::OnLButtonDown( UINT nFlags, CPoint point )
{
// This function will begin the game when the instruction message is clicked on

	myGame.Click (point.y, point.x, this);
	if (myGame.Done(this))
	{
		myGame.Message (this);
		myGame.Init(numRows+=2, numCols+=2, this);
		Invalidate (TRUE);
		SetTimer (1, 1000, NULL);
	}
}

afx_msg void GameWin::OnKeyDown (UINT achar, UINT repeat, UINT flags) 
{	// Sets the values for the controls
	switch(achar)
    {
	case VK_LEFT: case 'A':
			myGame.Move ('a', this);
			break;
		case VK_RIGHT: case 'D':
			myGame.Move ('d', this);
			break;
		case VK_UP: case 'W':
			myGame.Move ('w', this);
			break;
		case VK_DOWN: case 'S':
			myGame.Move ('s', this);
			break;
	}
	if (myGame.Done(this))
	{
		myGame.Message (this);
		myGame.Init(numRows+=2, numCols+=2, this);
		Invalidate (TRUE);
		SetTimer (1, 1000, NULL);
	}
}

afx_msg void GameWin::OnTimer (UINT nIDEvent)
{	
	// This function will: Handle timer events

	if (nIDEvent == 1)
		myGame.UpdateTime (this);  
}

BEGIN_MESSAGE_MAP (GameWin, CFrameWnd)
	ON_WM_PAINT ()
	ON_WM_LBUTTONDOWN( )
	ON_WM_KEYDOWN ()
	ON_WM_TIMER ()
END_MESSAGE_MAP ()
