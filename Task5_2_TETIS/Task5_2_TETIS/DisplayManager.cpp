#include "Tetromino.h"
#include "pch.h"
#include "DisplayManager.h"
#include <iostream>
using namespace std;

DisplayManager::DisplayManager()
 : clearedLine(0), score(0), PLAY_AREA_HEIGHT(24), PLAY_AREA_WIDTH(10), bIsRefreshNeeded(true)
{
	for (int i = 0; i < PLAY_AREA_HEIGHT; i++)
	{
		memset(playArea[i], 0, sizeof(playArea[i]));
	}

	currentTetromino = new Tetromino(Tetromino::GetRandomTetromino());
	nextSlot = Tetromino::GetRandomTetromino();
}

int DisplayManager::GetClearedLine() const
{
	return clearedLine;
}

int DisplayManager::GetScore() const
{
	return score;
}

int DisplayManager::GetScreen(const int r, const int c) const
{
	return playArea[r][c];
}

eTetromino DisplayManager::GetNextSlot() const
{
	return nextSlot;
}

// 블록 생성 함수
void DisplayManager::DrawCurrentTertromino()
{
	int x = 0;
	int y = 0;
	for (int i = currentTetromino->GetCoordinateX(); i < currentTetromino->GetCoordinateX() + 4; i++)
	{
		for (int j = currentTetromino->GetCoordinateY(); j < currentTetromino->GetCoordinateY() + 4; j++)
		{
			if (j >= 0)
			{
				if (currentTetromino->GetShape(x, y) == 2 && playArea[i][j] == 0)
				{
					playArea[i][j] = 2;
				}
			}
			y++;
		}
		y = 0;
		x++;
	}
}

// 블록 지우는 함수
void DisplayManager::ClearCurrentTetromino()
{
	int x = 0;
	int y = 0;
	for (int i = currentTetromino->GetCoordinateX(); i < currentTetromino->GetCoordinateX() + 4; i++)
	{
		for (int j = currentTetromino->GetCoordinateY(); j < currentTetromino->GetCoordinateY() + 4; j++)
		{
			if (j >= 0 && j < PLAY_AREA_WIDTH)
			{
				if (playArea[i][j] == 2)
				{
					playArea[i][j] = 0;
				}
			}
			y++;
		}
		y = 0;
		x++;
	}
}

void DisplayManager::ClearLine()
{
	for (int i = 4; i < PLAY_AREA_HEIGHT; i++)
	{
		bool bIsCleared = true;
		for (int j = 0; j < PLAY_AREA_WIDTH; j++)
		{
			if (playArea[i][j] != 1)
			{
				bIsCleared = false;
			}
		}
		if (bIsCleared)
		{
			for (int j = i; j > 0; j--)
			{
				for (int k = 0; k < PLAY_AREA_WIDTH; k++)
				{
					playArea[j][k] = playArea[j - 1][k];
				}
			}
			clearedLine++;
			score += 100;
		}
	}
}

// 블록 고정 함수
void DisplayManager::FixCurrentTetromino()
{
	int x = 0;
	int y = 0;
	for (int i = currentTetromino->GetCoordinateX(); i < currentTetromino->GetCoordinateX() + 4; i++)
	{
		for (int j = currentTetromino->GetCoordinateY(); j < currentTetromino->GetCoordinateY() + 4; j++)
		{
			if (currentTetromino->GetShape(x, y) == 2)
			{
				playArea[i][j] = 1;
			}
			y++;
		}
		y = 0;
		x++;
	}

	ClearLine();
	delete currentTetromino;						// 메모리 비우고,
	currentTetromino = new Tetromino(nextSlot);		// nextSlot data 넣는다.
	nextSlot = Tetromino::GetRandomTetromino();		// 새로운 nextSlot 생성.
}

eCollisionDirection DisplayManager::GetSideCollisionDirection()
{
	bool bIsCollidedLeft = false;
	bool bIsCollidedRight = false; 

	//벽과 충돌 체크
	int maxY = 6;
	for (int col = 3; col >= 0; col--)
	{
		for (int row = 0; row < 4; row++)
		{
			if (currentTetromino->GetShape(row, col) != 0)
			{
				goto quit_calculating_max_y;
			}
		}
		maxY++;
	}
quit_calculating_max_y:

	int minY = 0;
	for (int col = 0; col < 4; col++)
	{
		for (int row = 0; row < 4; row++)
		{
			if (currentTetromino->GetShape(row, col) != 0)
			{
				goto quit_calculating_min_y;
			}
		}
		minY--;
	}
quit_calculating_min_y:

	if (currentTetromino->GetCoordinateY() < minY)
	{
		bIsCollidedLeft = true;
	}

	if (currentTetromino->GetCoordinateY() > maxY)
	{
		bIsCollidedRight = true;
	}

	//다른 테트로미노와 충돌 체크
	int x = 0;
	int y = 0;
	for (int i = currentTetromino->GetCoordinateX(); i < currentTetromino->GetCoordinateX() + 4; i++)
	{
		for (int j = currentTetromino->GetCoordinateY(); j < currentTetromino->GetCoordinateY() + 4; j++)
		{
			if (j >= 0 && (currentTetromino->GetShape(x, y) == 2 && playArea[i][j] == 1))
			{
				if (y < 2)
				{
					bIsCollidedLeft = true;
				}
				else
				{
					bIsCollidedRight = true;
				}
			}
			y++;
		}
		y = 0;
		x++;
	}

	if (bIsCollidedLeft && bIsCollidedRight)
	{
		return eCollisionDirection::BOTH;
	}

	if (bIsCollidedLeft)
	{
		return eCollisionDirection::LEFT;
	}

	if (bIsCollidedRight)
	{
		return eCollisionDirection::RIGHT;
	}

	return eCollisionDirection::NOT_COLLIDED;
}

bool DisplayManager::CheckCollideWithFloor()
{
	int maxX = 20;
	for (int row = 3; row >= 0; row--)
	{
		for (int col = 0; col < 4; col++)
		{
			if (currentTetromino->GetShape(row, col) != 0)
			{
				goto quit_calculating_max_x;
			}
		}
		maxX++;
	}
quit_calculating_max_x:

	if (currentTetromino->GetCoordinateX() > maxX)
	{
		return true;
	}
	return false;
}

void DisplayManager::InputValidGameKey(eInputKey key)
{
	std::pair<int, int> curCoordinate = { currentTetromino->GetCoordinateX(), currentTetromino->GetCoordinateY() };		// 현재 좌표
	int curRotateLevel = currentTetromino->GetRotateLevel();	// 현재 회전상태
	bIsRefreshNeeded = true; 

	// 방향 이동
	switch (key)
	{
	case eInputKey::SPACE:	// PAUSE
		currentTetromino->SetCoordinate(currentTetromino->GetCoordinateX(), currentTetromino->GetCoordinateY());
		if (GetSideCollisionDirection() != eCollisionDirection::NOT_COLLIDED)
		{
			currentTetromino->SetCoordinate(currentTetromino->GetCoordinateX(), currentTetromino->GetCoordinateY());
			bIsRefreshNeeded = false;
		}
		break;
	case eInputKey::A:	// LEFT
		currentTetromino->SetCoordinate(currentTetromino->GetCoordinateX(), currentTetromino->GetCoordinateY() - 1);
		if (GetSideCollisionDirection() != eCollisionDirection::NOT_COLLIDED)
		{
			currentTetromino->SetCoordinate(currentTetromino->GetCoordinateX(), currentTetromino->GetCoordinateY() + 1);
			bIsRefreshNeeded = false;
		}
		break;
	case eInputKey::D:	// RIGHT
		currentTetromino->SetCoordinate(currentTetromino->GetCoordinateX(), currentTetromino->GetCoordinateY() + 1);
		if (GetSideCollisionDirection() != eCollisionDirection::NOT_COLLIDED)
		{
			currentTetromino->SetCoordinate(currentTetromino->GetCoordinateX(), currentTetromino->GetCoordinateY() - 1);
			bIsRefreshNeeded = false;
		}
		break;
	case eInputKey::S:	// SoftDrop
		score++;
	case eInputKey::TIME_PASSED: // 1 frame
		currentTetromino->SetCoordinate(currentTetromino->GetCoordinateX() + 1, currentTetromino->GetCoordinateY());
		if (CheckCollideWithFloor() == true || GetSideCollisionDirection() != eCollisionDirection::NOT_COLLIDED)
		{
			currentTetromino->SetCoordinate(currentTetromino->GetCoordinateX() - 1, currentTetromino->GetCoordinateY());
			FixCurrentTetromino();
		}
		break;
	case eInputKey::C: // HardDrop N frame
		while (1)
		{
			currentTetromino->SetCoordinate(currentTetromino->GetCoordinateX() + 1, currentTetromino->GetCoordinateY());
			if (CheckCollideWithFloor() == true || GetSideCollisionDirection() != eCollisionDirection::NOT_COLLIDED)
			{
				currentTetromino->SetCoordinate(currentTetromino->GetCoordinateX() - 1, currentTetromino->GetCoordinateY());
				FixCurrentTetromino();
				break;
			}
		}
	case eInputKey::W:	//ROTATE
	{
		int originalCoordinateX = currentTetromino->GetCoordinateX(); 
		int originalCoordinateY = currentTetromino->GetCoordinateY(); 
		bool bSuccess = false;

		// 회전방향 설정
		currentTetromino->Rotate(eRotate::CLOCKWISE);
		
		//회전 후 충돌 체크 및 충돌 방향 검출
		eCollisionDirection colDir;
		colDir = GetSideCollisionDirection();
		switch (colDir)
		{
		case eCollisionDirection::NOT_COLLIDED:	//충돌하지 않았다면 문제없이 회전처리
			bSuccess = true;
			break;
		case eCollisionDirection::BOTH:	//양쪽에서 충돌 시 무조건 실패
			bSuccess = false;
			break;
		case eCollisionDirection::LEFT:	//좌측에서 충돌 시 우측으로 옮겨보기
			for (int i = 1; i <= 3; i++)
			{
				currentTetromino->SetCoordinate(currentTetromino->GetCoordinateX(), currentTetromino->GetCoordinateY() + 1);
				if (GetSideCollisionDirection() == eCollisionDirection::NOT_COLLIDED)
				{
					bSuccess = true;
					break;
				}
			}
			break;
		case eCollisionDirection::RIGHT:	//우측에서 충돌 시 좌측으로 옮겨보기
			currentTetromino->SetCoordinate(originalCoordinateX, originalCoordinateY);
			for (int i = 1; i <= 3; i++)
			{
				currentTetromino->SetCoordinate(currentTetromino->GetCoordinateX(), currentTetromino->GetCoordinateY() - 1);
				if (GetSideCollisionDirection() == eCollisionDirection::NOT_COLLIDED)
				{
					bSuccess = true;
					break;
				}
			}
			break;
		default:
			assert(false);
			break;
		}
		//실패시 회전 복구
		if (!bSuccess)
		{
			currentTetromino->SetCoordinate(originalCoordinateX, originalCoordinateY);
			currentTetromino->Rotate(eRotate::CLOCKWISE);
			bIsRefreshNeeded = false;	
		}
		break;
	}
	default:
		bIsRefreshNeeded = false;
		break;
	}

	std::pair<int, int> nextCoordinate = { currentTetromino->GetCoordinateX(), currentTetromino->GetCoordinateY() };
	int nextRotateLevel = currentTetromino->GetRotateLevel();
	if (curCoordinate == nextCoordinate && curRotateLevel == nextRotateLevel) bIsRefreshNeeded = false;
}


bool DisplayManager::CheckIsRefreshNeeded()
{
	return bIsRefreshNeeded;
}

bool DisplayManager::CheckIsGameOver()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < PLAY_AREA_WIDTH; j++)
		{
			if (playArea[i][j] == 1)
			{
				return true;
			}
		}
	}
	return false;
}