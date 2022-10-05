#include <windows.h>//Ŀ��
#include <iostream>//�����
#include <cstdlib>//����
#include <conio.h>//Ű �Է�
#include <vector>
#include <algorithm>
using namespace std;

#pragma region DIR

#define left 75
#define right 77
#define down 80
#define up 72
#define space 32

#pragma endregion

struct block {
	int x = 5;
	int y = 2;
};

block block_shape[19][4] = {/*��*/
						{{-1,0}, {0,-1}, {0,0}, {-1,-1}} ,

						/*��,��*/
						{{-1,0},{2,0} ,{0,0},{1,0}}, {{0,0},{0,1}, {0,2},{0,-1}},
						/*S*/
						{{0,1},{1,0},{0,0},{1,-1}}, {{0,0},{1,0},{0,-1},{-1,-1}},
						/*Z*/
						{{-1,0},{0,0},{0,1},{-1,-1}}, {{-1,0},{1,-1}, {0,0},{0,-1}},

						/*�ݤ���*/
						{{0,0},{1,1},{0,1}, {0,-1}}, {{0,-1},{2,-1}, {0,0},{1,-1}}, {{-1,-1},{0,0},{0,1},{0,-1}}, {{-1,0},{1,0}, {0,0},{1,-1}},
						/*����*/
						{{-1,1},{0,0}, {0,1},{0,-1}}, {{-1,0},{1,0}, {0,0},{-1,-1}}, {{0,-1},{1,-1}, {0,0},{0,1},}, {{-2,-1} ,{0,-1}, {0,0}, {-1,-1}},
						/*��*/
						{ {0, 0},{ 1,0 },  { 0,1 },{ 0,-1 }}, { {-1,0},{1,0}, {0,1},{0,0}}, { {-1,0},{0,0}, {0,1},{0,-1} }, { {-1,0},{1,0}, {0,0},{0,-1}}
};

#pragma region CursorFunctions
void gotoXY(int x, int y) {
	COORD Pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
void Cursor(bool flag)
{
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.dwSize = 1;
	cursorInfo.bVisible = flag;//true : ������, false : �� ������
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}
#pragma endregion

int		Map_Origin[12][22] = { 0, };
int		Map_Weighted[12][22] = { 0, };
int		Random_value();

void	Create_Map();
void	Redering_Map();

bool	Check_Block(int block_select, block blockxy, int Map[12][22]);
void	GameOver(int block_select, block blockxy, int Map[12][22]);
void	fill_Block(int block_select, block blockxy, int Map[12][22]);
void	Empty_Block(int block_select, block blockxy, int Map[12][22]);
void	Stop_Block(int block_select, block blockxy, int Map[12][22]);
void	Move_Left(int block_select, block blockxy, int Map[12][22]);
void	Move_Right(int block_select, block blockxy, int Map[12][22]);
void	Move_Down(int block_select, block blockxy, int Map[12][22]);
void	Rotate_Block(int* block_select, block blockxy, int Map[12][22]);
void	Clear_BlockLine(int Map[12][22], int& score);

void	SetStartPos(block& blockxy, int x, int y)
{
	blockxy.x = x;
	blockxy.y = y;
}
pair<int, int>	SearchBestWay(int block_select);

int main() 
{
	Cursor(false);

	block	blockxy;
	int		block_select = 0;
	int		input = 0;
	bool	check = 0;
	int		level = 5;
	int		score = 0;
	int		rotateCnt = 0;
	int*	_block_select = &block_select;//block_select�� �ּҰ��� ���� ������ ����(�� ȸ���� �̿�)
	
	Create_Map();

	unsigned __int64 Timer = GetTickCount64();
	while (true)
	{
		
		#pragma region INPUT ACTION
				if (_kbhit())
					input = _getch();
				else
					input = 0;

				switch (input)
				{
					//���� Ű
				case left:
					blockxy.x--;
					if (Check_Block(block_select, blockxy, Map_Origin) == false)//2��
						blockxy.x++;
					else
						Move_Left(block_select, blockxy, Map_Origin);//9��
					break;

					//������ Ű
				case right:
					blockxy.x++;
					if (Check_Block(block_select, blockxy, Map_Origin) == false)//2��
						blockxy.x--;
					else
						Move_Right(block_select, blockxy, Map_Origin);//9��
					break;

					//�Ʒ��� Ű
				case down:
					blockxy.y++;
					if (Check_Block(block_select, blockxy, Map_Origin) == false)//2��
						blockxy.y--;
					else
						Move_Down(block_select, blockxy, Map_Origin);//9��
					break;

					//���� Ű (���� ����)
				case up:
					Rotate_Block(_block_select, blockxy, Map_Origin);//10��
					break;

					//�����̽��� (�ϵ� ���)
				case space:
					while (true) 
					{
						blockxy.y++;
						if (!Check_Block(block_select, blockxy, Map_Origin)) {
							blockxy.y--;
							break;
						}
						else {
							Move_Down(block_select, blockxy, Map_Origin);//9��
						}
					}
					break;
				}
		#pragma endregion

		if (GetTickCount64() >= Timer + 300/ level)
		{
			Empty_Block(block_select, blockxy, Map_Origin);

			if (rotateCnt > 0)
			{
				while (rotateCnt--)
				{
					Rotate_Block(_block_select, blockxy, Map_Origin);
				}
			}
			
			blockxy.y++;
			check = Check_Block(block_select, blockxy, Map_Origin);

			if (check == false) {
				blockxy.y--;
				Stop_Block(block_select, blockxy, Map_Origin);
				Clear_BlockLine(Map_Origin, score);

				block_select = Random_value();
				auto value = SearchBestWay(block_select);
				SetStartPos(blockxy, value.first, 2);
				rotateCnt = value.second;
			
				GameOver(block_select, blockxy, Map_Origin);
			}
			else 
			{
				Move_Down(block_select, blockxy, Map_Origin);
			}

			gotoXY(30, 1);
			cout << "score : " << score;

			Timer = GetTickCount64();
		}
		Redering_Map();
	}
}

void Create_Map() {
	for (int x = 1; x < 11; x++) {//���� : �� ��,�� �� 8��(����)  
		gotoXY(x * 2 + 2, 0);
		cout << "��";
		Map_Origin[x][0] = 1;

		gotoXY(x * 2 + 2, 21);
		cout << "��";
		Map_Origin[x][21] = 1;
	}
	for (int y = 0; y < 22; y++) {//���� : 22�� ���� 
		gotoXY(2, y);
		cout << "��";
		Map_Origin[0][y] = 1;

		gotoXY(24, y);
		cout << "��";
		Map_Origin[11][y] = 1;
	}

	// COMPUTER
	for (int x = 25; x < 35; x++) {//���� : �� ��,�� �� 8��(����)  
		/*gotoXY(x * 2 + 2, 0);
		cout << "��";*/
		Map_Weighted[x - 24][0] = 1;

		/*gotoXY(x * 2 + 2, 21);
		cout << "��";*/
		Map_Weighted[x - 24][21] = 1;
	}

	for (int y = 0; y < 22; y++) {//���� : 22�� ���� 
		/*gotoXY(50, y);
		cout << Map_Weighted[0][y];*/
		Map_Weighted[0][y] = 1;
		
		/*gotoXY(72, y);
		cout <<  Map_Weighted[11][y];*/
		Map_Weighted[11][y] = 1;
	}
}
void Redering_Map() {
	for (int i = 20; i > 0; i--) {
		for (int j = 1; j < 11; j++) {
			if (Map_Origin[j][i] == 1) {//������ �� ��
				gotoXY(j * 2 + 2, i);
				cout << "��";
			}
			else if (Map_Origin[j][i] == 2) {//�����̴� ��
				gotoXY(j * 2 + 2, i);
				cout << "��";
			}
			else {
				gotoXY(j * 2 + 2, i);
				cout << "  ";
			}
		}
	}

	//for (int i = 20; i > 0; i--) {
	//	for (int j = 1; j < 11; j++) {
	//		if (Map_Weighted[j][i] == 1) {//������ �� ��
	//			gotoXY(j * 2 + 50, i);
	//			cout << "��";
	//		}
	//		else if (Map_Weighted[j][i] == 2) {//�����̴� ��
	//			gotoXY(j * 2 + 50, i);
	//			cout << "��";
	//		}
	//		else {
	//			gotoXY(j * 2 + 50, i);
	//			cout << "  ";
	//		}
	//	}
	//}
}
bool Check_Block	(int block_select, block blockxy, int Map[12][22]) {

	block temp;

	for (int i = 0; i < 4; i++) 
	{
		temp.x = block_shape[block_select][i].x + blockxy.x;
		temp.y = block_shape[block_select][i].y + blockxy.y;

		if (temp.y > 0) 
		{
			if (Map[temp.x][temp.y] == 1)
			{
				return false;
			}
		}
	}
	return true;
}
void fill_Block		(int block_select, block blockxy, int Map[12][22])
{
	block temp;
	vector<int> tempX;
	vector<int> tempY;

	for (int i = 0; i < 4; i++)
	{
		temp.x = block_shape[block_select][i].x + blockxy.x;
		tempX.push_back(temp.x);
	}

	for (int i = 0; i < 4; i++)
	{
		temp.y = block_shape[block_select][i].y + blockxy.y;
		tempY.push_back(temp.y);
	}

	for (int i = 0; i < 4; i++)
	{
		if (temp.y >= 1) 
		{
			Map[tempX[i]][tempY[i]] = 2;
		}
	}
}
void Stop_Block		(int block_select, block blockxy, int Map[12][22]) {

	block temp;

	for (int i = 0; i < 4; i++) {
		temp.x = block_shape[block_select][i].x + blockxy.x;
		temp.y = block_shape[block_select][i].y + blockxy.y;
	
		Map[temp.x][temp.y] = 1;			
	}
}
void Clear_BlockLine(int Map[12][22], int &score) {

	for (int i = 20; i > 0; i--)
	{
		int cnt = 0;
		for (int j = 1; j < 11; j++)
		{
			if (Map[j][i] == 1)
				++cnt;

			if (cnt == 10)
			{
				score += 100;
				for (int n = 1; n < 11; n++)
				{
					for (int m = i; m > 1; m--)
					{
						Map[n][m] = Map[n][m - 1];//�� ���̶� ü����
					}
				}
				i++;//�������� �ѹ��� ä������ �� �˻� �� ó��
			}
		}
	}
}
void Empty_Block	(int block_select, block blockxy, int Map[12][22]) {

	block temp;

	for (int i = 0; i < 4; i++) {
		temp.x = block_shape[block_select][i].x + blockxy.x;
		temp.y = block_shape[block_select][i].y + blockxy.y;
		Map[temp.x][temp.y] = 0;
	}

}
void Move_Left		(int block_select, block blockxy, int Map[12][22]) {

	blockxy.x++;
	Empty_Block(block_select, blockxy, Map);
	blockxy.x--;
	fill_Block(block_select, blockxy, Map);

}
void Move_Right		(int block_select, block blockxy, int Map[12][22]) {

	blockxy.x--;
	Empty_Block(block_select, blockxy, Map);
	blockxy.x++;
	fill_Block(block_select, blockxy, Map);

}
void Move_Down		(int block_select, block blockxy, int Map[12][22]) {

	blockxy.y--;
	Empty_Block(block_select, blockxy, Map);
	blockxy.y++;
	fill_Block(block_select, blockxy, Map);

}
void Rotate_Block	(int* block_select, block blockxy, int Map[12][22]) {

	bool check_rotate = false;//���� ���� ����

	//2�� ���ϴ� ���� ��
	if (*block_select >= 1 && *block_select <= 6) {
		if (*block_select % 2 == 0) {
			*block_select -= 1;
			check_rotate = Check_Block(*block_select, blockxy, Map);
			if (check_rotate == 0) {
				*block_select += 1;
			}
			else {
				*block_select += 1;
				Empty_Block(*block_select, blockxy, Map);
				*block_select -= 1;
				fill_Block(*block_select, blockxy, Map);
			}
		}
		else {
			*block_select += 1;
			check_rotate = Check_Block(*block_select, blockxy, Map);
			if (check_rotate == 0) {
				*block_select -= 1;
			}
			else {
				*block_select -= 1;
				Empty_Block(*block_select, blockxy, Map);
				*block_select += 1;
				fill_Block(*block_select, blockxy, Map);
			}
		}
	}
	//4�� ���ϴ� ���� ��
	else if (*block_select >= 7 && *block_select <= 18) {
		if ((*block_select - 7) % 4 == 3) {//�迭 ���� �ִ� ������ ó�� ������ ��ȯ 
			*block_select -= 3;
			check_rotate = Check_Block(*block_select, blockxy, Map);//2��
			if (check_rotate == 0) {
				*block_select += 3;
			}
			else {
				*block_select += 3;
				Empty_Block(*block_select, blockxy, Map);//5��
				*block_select -= 3;
				fill_Block(*block_select, blockxy, Map);//3��
			}
		}
		else {
			*block_select += 1;
			check_rotate = Check_Block(*block_select, blockxy, Map);//2��
			if (check_rotate == 0) {
				*block_select -= 1;
			}
			else {
				*block_select -= 1;
				Empty_Block(*block_select, blockxy, Map);//5��
				*block_select += 1;
				fill_Block(*block_select, blockxy, Map);//3��
			}
		}
	}
}
void GameOver		(int block_select, block blockxy, int Map[12][22])
{
	bool check = Check_Block(block_select, blockxy, Map);//2��

   	if (check == false)  //���� ����
	{
      		system("cls");
		cout << "\n\n    Game Over!!    \n\n\n\n";
		Sleep(1000);
		exit(0);
	}
}
pair<int, int>	SearchBestWay(int block_select)
{
	vector<int> vScore;
	vector<int> vPosX;
	vector<int> vRotate;

	block blockxy_cpu;
	blockxy_cpu.x = 2;
	blockxy_cpu.y = 2;
	int score_cpu = 0;
	int rotateCnt = 0;
	int block_select_cpu = block_select;
	int* _block_select = &block_select_cpu;
	int ROTATECOUNT = 4;

	// ��� ����� �� ���ϱ�
	while (ROTATECOUNT--)
	{
		for (int i = 1; i < 10; i++)
		{
			SetStartPos(blockxy_cpu, i, 2);

			if (rotateCnt > 0)
			{
				for (int i = 0; i < rotateCnt; i++)
				{
					Rotate_Block(_block_select, blockxy_cpu, Map_Weighted);
				}
			}

			while (1)
			{
				Empty_Block(block_select_cpu, blockxy_cpu, Map_Weighted);

				blockxy_cpu.y++;
				int check = Check_Block(block_select_cpu, blockxy_cpu, Map_Weighted);

				if (check == false)
				{
					blockxy_cpu.y--;
					blockxy_cpu.y--;
					break;
				}
				else
					Move_Down(block_select_cpu, blockxy_cpu, Map_Weighted);
			}

			// �� �������
			// ����
			blockxy_cpu.x--;
			if (Check_Block(block_select_cpu, blockxy_cpu, Map_Weighted) == false)
			{
				block temp;
				temp.x = block_shape[block_select][i].x + blockxy_cpu.x;
				temp.y = block_shape[block_select][i].y + blockxy_cpu.y;

				for (int i = 0; i < 4; i++)
				{
					if (Map_Weighted[temp.x - 1][temp.y] == 2)
						continue;

					if (Map_Weighted[temp.x - 1][temp.y] == 1)
						score_cpu += 5;
				}
			}
			blockxy_cpu.x++;

			// ������
			blockxy_cpu.x++;
			if (!Check_Block(block_select_cpu, blockxy_cpu, Map_Weighted) == false)
			{
				block temp;
				temp.x = block_shape[block_select][i].x + blockxy_cpu.x;
				temp.y = block_shape[block_select][i].y + blockxy_cpu.y;

				for (int i = 0; i < 4; i++)
				{
					if (Map_Weighted[temp.x + 1][temp.y] == 2)
						continue;

					if (Map_Weighted[temp.x + 1][temp.y] == 1)
						score_cpu += 5;
				}
			}
			blockxy_cpu.x--;

			// �ٴ�
			blockxy_cpu.y++;
			if (!Check_Block(block_select_cpu, blockxy_cpu, Map_Weighted) == false)
			{
				block temp;
				temp.x = block_shape[block_select][i].x + blockxy_cpu.x;
				temp.y = block_shape[block_select][i].y + blockxy_cpu.y;

				for (int i = 0; i < 4; i++)
				{
					if (Map_Weighted[temp.x][temp.y + 1] == 2)
						continue;

					if (Map_Weighted[temp.x][temp.y + 1] == 1)
						score_cpu += 10;
				}
			}
			blockxy_cpu.x--;

			Stop_Block(block_select_cpu, blockxy_cpu, Map_Weighted);
			Clear_BlockLine(Map_Weighted, score_cpu);
			vScore.push_back(score_cpu);

			// ���� �ʱ�ȭ
			score_cpu = 0;
			vPosX.push_back(i);
			vRotate.push_back(rotateCnt);

			// �� �ʱ�ȭ
			for (int y = 0; y < 22; y++)
			{
				for (int x = 0; x < 12; x++)
				{
					Map_Weighted[x][y] = Map_Origin[x][y];
				}
			}
		}
		rotateCnt++;
	}

	// �ְ����� ��󳻱�
	int bestScore = *max_element(vScore.begin(), vScore.end());
	int bestScoreIndex = 0;

	for (int i = 0; i < vScore.size(); i++)
	{
		if (vScore[i] == bestScore)
		{
			bestScoreIndex = i;
			break;
		}
	}

	int bestX = vPosX[bestScoreIndex];
	int bestRotate = vRotate[bestScoreIndex];

	return { bestX, bestRotate };
}
int Random_value() {

	srand((static_cast<unsigned int>(time(NULL))));
	int value = rand() % 19;//0���� 18����

	return value;
}