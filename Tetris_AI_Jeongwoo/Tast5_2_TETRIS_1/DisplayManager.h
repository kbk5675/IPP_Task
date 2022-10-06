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
	
	//---------------------------------------------------------------
	void DrawTestTertromino();

	void BestLotation();

	void SetBestRotate(int r);
	int GetBestRotate();

	void Setcnt2_Rotate(int r);
	int Getcnt2_Rotate();

	void Set_Width_Control(int r);
	int Get_Width_Control();

	void Set_BestLot(int r);
	int Get_BestLot();

	void Remit_Map();

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

	//---------------------------------------------------------------
	int bestLot;
	int cnt_Rotate;
	int best_Rotate;
	int cnt2_Rotate;
	int width_control;
};
