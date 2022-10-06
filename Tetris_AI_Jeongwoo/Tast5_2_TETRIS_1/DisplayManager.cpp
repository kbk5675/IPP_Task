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

// ��� ���� �Լ�
void DisplayManager::DrawCurrentTertromino()
{
	//�� ����
	int x = 0;
	int y = 0;
	for (int i = currentTetromino->GetCoordinateX(); i < currentTetromino->GetCoordinateX() + 4; i++)// x�� ����, current...==�� ��ü
	{
		for (int j = currentTetromino->GetCoordinateY(); j < currentTetromino->GetCoordinateY() + 4; j++)// y�� ����
		{
			if (j >= 0)//y���� 0�̻��� ��
			{
				if (currentTetromino->GetShape(x, y) == 2 && playArea[i][j] == 0)//�� ��翡�� 2�̰�, ���� 0���� �����������
				{
					playArea[i][j] = 2;
				}
			}
			y++;//�� ��� �˻�
		}
		y = 0;//�� ��� �˻�
		x++;//�� ��� �˻�
	}
}


/*-------- ������� �ΰ����� �Լ����Դϴ�.--------*/

//���� �� ���� �Լ�
void DisplayManager::DrawTestTertromino()
{
	//�� ����
	int x = 0;
	int y = 0;
	for (int i = currentTetromino->GetCoordinateX(); i < currentTetromino->GetCoordinateX() + 4; i++)// x�� ����, current...==�� ��ü
	{
		for (int j = currentTetromino->GetCoordinateY(); j < currentTetromino->GetCoordinateY() + 4; j++)// y�� ����
		{
			if (j >= 0)//y���� 0�̻��� ��
			{
				if (currentTetromino->GetShape(x, y) == 2 && playArea[i][j] == 0)//�� ��翡�� 2�̰�, ���� 0���� �����������
				{
					playArea[i][j] = 3;
				}
			}
			y++;//�� ��� �˻�
		}
		y = 0;//�� ��� �˻�
		x++;//�� ��� �˻�
	}
}

//����ġ �˻� �Լ��ڡڡڡڡڡ�
void DisplayManager::BestLotation() {

	//�� ���� ���� ó�� �ϴ� ��
	for (int j = 0; j < 4; j++) {

		//���� �� ����(�׸���)
		DrawTestTertromino();

		//3���� ȸ�� �Ͽ� ����ġ �˻�
		if (cnt_Rotate != 0) {
			for (int i = 0; i < cnt_Rotate; i++) {
				currentTetromino->Rotate(eRotate::CLOCKWISE);
			}
		}
		
		int height_count = 0;
		int width_count = 0;
		//����ġ �˻� �� ���� ��ǥ�� �������� �˻�
		for (int i = -5; i < 5; i++)//���ʺ��� ���������� �˻�
		{	
			currentTetromino->SetCoordinate(currentTetromino->GetCoordinateX(), currentTetromino->GetCoordinateY() + i);
			width_count = i;

			//�� �Ǵ� ���� �浹 ���� �� 
			eCollisionDirection colDir;
			colDir = GetSideCollisionDirection();
			switch (colDir)
			{
			case eCollisionDirection::LEFT:	//�������� �浹 �� �������� �Űܺ���
				currentTetromino->SetCoordinate(currentTetromino->GetCoordinateX(), currentTetromino->GetCoordinateY() + i + 1);
				width_count++;
				if (GetSideCollisionDirection() != eCollisionDirection::NOT_COLLIDED) {
					currentTetromino->SetCoordinate(currentTetromino->GetCoordinateX(), currentTetromino->GetCoordinateY() + i + 1);
					width_count++;
				}
				break;
			case eCollisionDirection::RIGHT:	//�������� �浹 �� �������� �Űܺ���
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

			//1. �� ������ �����ٰ�
			while (1)
			{
				currentTetromino->SetCoordinate(currentTetromino->GetCoordinateX() + 1, currentTetromino->GetCoordinateY());
				height_count++;
				//2. �ٴ� �κп� ��� �Ǹ�
				if (CheckCollideWithFloor() == true || GetSideCollisionDirection() != eCollisionDirection::NOT_COLLIDED)
				{
					currentTetromino->SetCoordinate(currentTetromino->GetCoordinateX() - 1, currentTetromino->GetCoordinateY());
					height_count--;
					//3. ������ �˴ϴ�.
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
			//������ �ǰ� �� �ֺ��� ĭ�� �˻��Ͽ� ����ġ�� ����մϴ�.(���� ���� ���� �ڸ� �Դϴ�.) 
			for (int i = currentTetromino->GetCoordinateX() + height_count; i < currentTetromino->GetCoordinateX() + height_count + 4; i++)
			{
				for (int j = currentTetromino->GetCoordinateY() + width_count; j < currentTetromino->GetCoordinateY() + width_count + 4; j++)
				{
					if (playArea[i][j] == 1) {
						cnt1 += 1;
					}
				}
			}
			Set_BestLot(cnt1);//����ġ ���� �����մϴ�.

			//���� ���� �ڸ����� �Ǵ� �� �ݴϴ�.(���� ���� �ڸ���� ��ġ��, ȸ������ ���� �˴ϴ�.)
			if (cnt1 < Get_BestLot()) {
				Set_Width_Control(width_count);
				SetBestRotate(cnt_Rotate);
			}

			//�˻� �ʱ� ���·� ���� ����ϴ�.
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

//�˻� �ʱ� ���·� ���� �ǵ����� �Լ�
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

/*-------- ������� �ΰ����� �Լ����Դϴ�. --------*/


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
	delete currentTetromino;						// �޸� ����,
	currentTetromino = new Tetromino(nextSlot);		// nextSlot data �ִ´�.
	BestLotation();		//����ġ �˻� �Լ�!!
	nextSlot = Tetromino::GetRandomTetromino();		// ���ο� nextSlot ����.
}

eCollisionDirection DisplayManager::GetSideCollisionDirection()
{
	bool bIsCollidedLeft = false;
	bool bIsCollidedRight = false;

	//���� �浹 üũ
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

	//�ٸ� ��Ʈ�ι̳�� �浹 üũ
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

	// ���� �̵�
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

		/*----------------- ������� �ΰ����� �����Դϴ�. -----------------*/

		//1. ����ġ �˻縦 ���� ���� ȸ������ŭ ���� ȸ���մϴ�.
		if (Getcnt2_Rotate() < GetBestRotate()) {
			currentTetromino->Rotate(eRotate::CLOCKWISE);
			Setcnt2_Rotate(++cnt2_Rotate);
		}
		
		//2. ȸ���� ������ ���� �̵��մϴ�.
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

		/*------------------------------------------ ������� �ΰ������̾����ϴ�. ------------------------------------------*/
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

		// ȸ������ ����
		currentTetromino->Rotate(eRotate::CLOCKWISE);

		//ȸ�� �� �浹 üũ �� �浹 ���� ����
		eCollisionDirection colDir;
		colDir = GetSideCollisionDirection();
		switch (colDir)
		{
		case eCollisionDirection::NOT_COLLIDED:	//�浹���� �ʾҴٸ� �������� ȸ��ó��
			bSuccess = true;
			break;
		case eCollisionDirection::BOTH:	//���ʿ��� �浹 �� ������ ����
			bSuccess = false;
			break;
		case eCollisionDirection::LEFT:	//�������� �浹 �� �������� �Űܺ���
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
		case eCollisionDirection::RIGHT:	//�������� �浹 �� �������� �Űܺ���
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

