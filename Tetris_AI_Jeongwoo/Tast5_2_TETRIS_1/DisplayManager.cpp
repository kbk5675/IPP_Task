#include "Tetromino.h"
#include "pch.h"
#include "DisplayManager.h"
#include <iostream>
using namespace std;

DisplayManager::DisplayManager()
	: clearedLine(0), score(0), PLAY_AREA_HEIGHT(24), PLAY_AREA_WIDTH(10), bIsRefreshNeeded(true), bestLot(100), cnt_Rotate(0), best_Rotate(0), cnt2_Rotate(0), width_control(0)
{
	for (int i = 0; i < PLAY_AREA_HEIGHT; i++)
	{
		memset(playArea[i], 0, sizeof(playArea[i]));
	}
	
	currentTetromino = new Tetromino(Tetromino::GetRandomTetromino());
	BestLotation();
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
	//블럭 생성
	int x = 0;
	int y = 0;
	for (int i = currentTetromino->GetCoordinateX(); i < currentTetromino->GetCoordinateX() + 4; i++)// x값 제어, current...==블럭 객체
	{
		for (int j = currentTetromino->GetCoordinateY(); j < currentTetromino->GetCoordinateY() + 4; j++)// y값 제어
		{
			if (j >= 0)//y축이 0이상일 때
			{
				if (currentTetromino->GetShape(x, y) == 2 && playArea[i][j] == 0)//블럭 모양에서 2이고, 맵이 0으로 비워져있으면
				{
					playArea[i][j] = 2;
				}
			}
			y++;//블럭 모양 검사
		}
		y = 0;//블럭 모양 검사
		x++;//블럭 모양 검사
	}
}


/*-------- 여기부터 인공지능 함수들입니다.--------*/

//시험 블럭 생성 함수
void DisplayManager::DrawTestTertromino()
{
	//블럭 생성
	int x = 0;
	int y = 0;
	for (int i = currentTetromino->GetCoordinateX(); i < currentTetromino->GetCoordinateX() + 4; i++)// x값 제어, current...==블럭 객체
	{
		for (int j = currentTetromino->GetCoordinateY(); j < currentTetromino->GetCoordinateY() + 4; j++)// y값 제어
		{
			if (j >= 0)//y축이 0이상일 때
			{
				if (currentTetromino->GetShape(x, y) == 2 && playArea[i][j] == 0)//블럭 모양에서 2이고, 맵이 0으로 비워져있으면
				{
					playArea[i][j] = 3;
				}
			}
			y++;//블럭 모양 검사
		}
		y = 0;//블럭 모양 검사
		x++;//블럭 모양 검사
	}
}

//가중치 검사 함수★★★★★★
void DisplayManager::BestLotation() {

	//블럭 생성 전에 처리 하는 곳
	for (int j = 0; j < 4; j++) {

		//시험 블럭 생성(그리기)
		DrawTestTertromino();

		//3번만 회전 하여 가중치 검사
		if (cnt_Rotate != 0) {
			for (int i = 0; i < cnt_Rotate; i++) {
				currentTetromino->Rotate(eRotate::CLOCKWISE);
			}
		}
		
		int height_count = 0;
		int width_count = 0;
		//가중치 검사 블럭 생성 좌표를 기준으로 검사
		for (int i = -5; i < 5; i++)//왼쪽부터 오른쪽으로 검사
		{	
			currentTetromino->SetCoordinate(currentTetromino->GetCoordinateX(), currentTetromino->GetCoordinateY() + i);
			width_count = i;

			//벽 또는 블럭과 충돌 했을 때 
			eCollisionDirection colDir;
			colDir = GetSideCollisionDirection();
			switch (colDir)
			{
			case eCollisionDirection::LEFT:	//좌측에서 충돌 시 우측으로 옮겨보기
				currentTetromino->SetCoordinate(currentTetromino->GetCoordinateX(), currentTetromino->GetCoordinateY() + i + 1);
				width_count++;
				if (GetSideCollisionDirection() != eCollisionDirection::NOT_COLLIDED) {
					currentTetromino->SetCoordinate(currentTetromino->GetCoordinateX(), currentTetromino->GetCoordinateY() + i + 1);
					width_count++;
				}
				break;
			case eCollisionDirection::RIGHT:	//우측에서 충돌 시 좌측으로 옮겨보기
				currentTetromino->SetCoordinate(currentTetromino->GetCoordinateX(), currentTetromino->GetCoordinateY() + i - 1);
				width_count--;
				if (GetSideCollisionDirection() != eCollisionDirection::NOT_COLLIDED) {
					currentTetromino->SetCoordinate(currentTetromino->GetCoordinateX(), currentTetromino->GetCoordinateY() + i - 1);
					width_count--;
				}
				break;
			default:
				break;
			}

			//1. 맨 밑으로 내리다가
			while (1)
			{
				currentTetromino->SetCoordinate(currentTetromino->GetCoordinateX() + 1, currentTetromino->GetCoordinateY());
				height_count++;
				//2. 바닥 부분에 닿게 되면
				if (CheckCollideWithFloor() == true || GetSideCollisionDirection() != eCollisionDirection::NOT_COLLIDED)
				{
					currentTetromino->SetCoordinate(currentTetromino->GetCoordinateX() - 1, currentTetromino->GetCoordinateY());
					height_count--;
					//3. 고정이 됩니다.
					for (int i = currentTetromino->GetCoordinateX() + height_count ; i < currentTetromino->GetCoordinateX() + height_count + 4; i++)
					{
						for (int j = currentTetromino->GetCoordinateY() + width_count; j < currentTetromino->GetCoordinateY() + width_count + 4; j++)
						{
							if (playArea[i][j] == 3)
							{
								playArea[i][j] = 4;
							}
						}
					}
					break;
				}
			}

			int cnt1 = 0;
			//고정이 되고 블럭 주변에 칸을 검사하여 가중치를 계산합니다.(낮을 수록 좋은 자리 입니다.) 
			for (int i = currentTetromino->GetCoordinateX() + height_count; i < currentTetromino->GetCoordinateX() + height_count + 4; i++)
			{
				for (int j = currentTetromino->GetCoordinateY() + width_count; j < currentTetromino->GetCoordinateY() + width_count + 4; j++)
				{
					if (playArea[i][j] == 1) {
						cnt1 += 1;
					}
				}
			}
			Set_BestLot(cnt1);//가중치 값을 저장합니다.

			//가장 좋은 자리인지 판단 해 줍니다.(가장 좋은 자리라면 위치값, 회전값이 저장 됩니다.)
			if (cnt1 < Get_BestLot()) {
				Set_Width_Control(width_count);
				SetBestRotate(cnt_Rotate);
			}

			//검사 초기 상태로 맵을 만듭니다.
			Remit_Map();

			height_count = 0;
			width_count = 0;

		}

		cnt_Rotate++;
		if (cnt_Rotate == 4) {
			cnt_Rotate = 0;
		}
	}
}

//int best_Rotate (set, get)
void DisplayManager::SetBestRotate(int r) {
	best_Rotate = r;
}
int DisplayManager::GetBestRotate() {
	return best_Rotate;
}

//int cnt2_Rotate (set, get)
void DisplayManager::Setcnt2_Rotate(int r) {
	cnt2_Rotate = r;
}
int DisplayManager::Getcnt2_Rotate() {
	return cnt2_Rotate;
}

//int width_control (set, get)
void DisplayManager::Set_Width_Control(int r) {
	width_control = r;
}
int DisplayManager::Get_Width_Control() {
	return width_control;
}

//int bestLot (set, get)
void DisplayManager::Set_BestLot(int r) {
	bestLot = r;
}
int DisplayManager::Get_BestLot() {
	return bestLot;
}

//검사 초기 상태로 맵을 되돌리는 함수
void DisplayManager::Remit_Map() {
	for (int i = 0; i < 24; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (j >= 0 && j < PLAY_AREA_WIDTH)
			{
				if (playArea[i][j] == 4 || playArea[i][j] == 3)
				{
					playArea[i][j] = 0;
				}
			}
		}
	}
}

/*-------- 여기까지 인공지능 함수들입니다. --------*/


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
	BestLotation();		//가중치 검사 함수!!
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
	std::pair<int, int> curCoordinate = { currentTetromino->GetCoordinateX(), currentTetromino->GetCoordinateY() };		
	int curRotateLevel = currentTetromino->GetRotateLevel();	

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

		/*----------------- 여기부터 인공지능 시작입니다. -----------------*/

		//1. 가중치 검사를 통해 얻어온 회전값만큼 블럭을 회전합니다.
		if (Getcnt2_Rotate() < GetBestRotate()) {
			currentTetromino->Rotate(eRotate::CLOCKWISE);
			Setcnt2_Rotate(++cnt2_Rotate);
		}
		
		//2. 회전이 끝나면 블럭이 이동합니다.
		else {
			if (currentTetromino->GetCoordinateY() > Get_Width_Control()) {
				for (int i = 0; i < Get_Width_Control(); i++) {
					currentTetromino->SetCoordinate(currentTetromino->GetCoordinateX(), currentTetromino->GetCoordinateY() - 1);
				}
			}
			else if (currentTetromino->GetCoordinateY() < Get_Width_Control()) {
				for (int i = 0; i < Get_Width_Control(); i++) {
					currentTetromino->SetCoordinate(currentTetromino->GetCoordinateX(), currentTetromino->GetCoordinateY() + 1);
				}
			}
			else {
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
			}
		}

		/*------------------------------------------ 여기까지 인공지능이었습니다. ------------------------------------------*/
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

