#pragma once
#include "tetromino.h"
class DisplayManager
{
public:
	const int PLAY_AREA_HEIGHT;
	const int PLAY_AREA_WIDTH;

	DisplayManager();

	int GetClearedLine() const;
	int GetScore() const;
	int GetScreen(const int r, const int c) const;

	eTetromino GetNextSlot() const;

	void ClearCurrentTetromino();
	void DrawCurrentTertromino();
	void FixCurrentTetromino();
	void ClearLine();
	void InputValidGameKey(eInputKey key);

	bool CheckIsRefreshNeeded();
	bool CheckIsGameOver();

private:
	eCollisionDirection GetSideCollisionDirection();

	bool CheckCollideWithFloor();
	
private:
	Tetromino* currentTetromino;

	int clearedLine;
	int score;
	int playArea[24][10];

	bool bIsRefreshNeeded;



	eTetromino nextSlot;
};