#include "GameManager.h"
#include<MMSystem.h>
#pragma comment(lib,"Winmm.lib")
using namespace std;

HANDLE g_hScreen[2];



GameManager::GameManager() 
	: t(0), getScore(0), level(1), difficulty(0.0f), speed(1.0f), displayManager(nullptr), 
	bIsPause(false),bPlayingBGM(false)
{

}

// Double Buffering
void GameManager::ScreenInit()
{
	CONSOLE_CURSOR_INFO cci;

	g_hScreen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	g_hScreen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	cci.dwSize = 1;
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(g_hScreen[0], &cci);
	SetConsoleCursorInfo(g_hScreen[1], &cci);
}
void GameManager::ScreenFlipping()
{
	SetConsoleActiveScreenBuffer(g_hScreen[index]);
	index = !index;
}
void GameManager::ScreenPrint(int x, int y, std::string s)
{
	DWORD dw;
	COORD CursorPosition = { x, y };
	SetConsoleCursorPosition(g_hScreen[index], CursorPosition);
	WriteFile(g_hScreen[index], s.c_str(), s.size(), &dw, NULL);
}
void GameManager::ScreenClear()
{
	for (int i = 0; i < 60; i++)
	{
		COORD Coor = { i, 0 };
		DWORD dw;
		FillConsoleOutputCharacter(g_hScreen[index], NULL, 60 * 30, Coor, &dw);
	}
}

void GameManager::Render()
{
	if (displayManager->CheckIsRefreshNeeded())
	{
#pragma region Fill_Data_to_String

		std::string s;

		if (bIsPause)
		{
			s += "CONSOLE TETRIS                                         PAUSE\n";
		}
		else
		{
			s += "CONSOLE TETRIS\n";
		}
		s += "L  E  V  E  L : ";
		s += to_string(level);
		s += '\n';
		s += "S  C  O  R  E : ";
		/*s += std::setfill('0');
		s += std::setw(8); */
		s += to_string(displayManager->GetScore());
		s += '\n';
		s += "C L E A R E D : ";
		s += to_string(displayManager->GetClearedLine());
		s += '\n';
		s += "S  P  E  E  D : ";
		s += to_string((int)(speed * 1000));
		s += "fps\n\n";
		s += "▩▩▩▩▩▩▩▩▩▩··········▩▩▩  NEXT  ▩▩▩\n";

		displayManager->DrawCurrentTertromino();

		for (int i = 4; i < displayManager->PLAY_AREA_HEIGHT; i++)
		{
			// 왼쪽
			if (i <= 7)
			{
				for (int j = 0; j < 10; j++)
				{
					screenBuffer[i - 4][j] = "▩";
				}
			}
			else
			{
				for (int j = 0; j < 10; j++)
				{
					screenBuffer[i - 4][j] = "▩";
				}
			}

			// 가운데(스크린)
			for (int j = 0; j < displayManager->PLAY_AREA_WIDTH; j++)
			{
				if (displayManager->GetScreen(i, j) == 1)
				{
					screenBuffer[i - 4][j + 10] = "▣";
				}
				else if (displayManager->GetScreen(i, j) == 2)
				{
					screenBuffer[i - 4][j + 10] = "■";
				}
				else
				{
					screenBuffer[i - 4][j + 10] = "  ";
				}
			}

			// 오른쪽
			if (i <= 7)
			{
				for (int j = 0; j < 3; j++)
				{
					screenBuffer[i - 4][j + 20] = "▩";
				}

				for (int j = 0; j < 4; j++)
				{
					int shape = Tetromino::GetSpecificShape(displayManager->GetNextSlot(), 0, i - 4, j);
					if (shape == 0)
					{
						screenBuffer[i - 4][j + 23] = "  ";
					}
					else
					{
						screenBuffer[i - 4][j + 23] = "■";
					}
				}

				for (int j = 0; j < 3; j++)
				{
					screenBuffer[i - 4][j + 27] = "▩";
				}

			}
			else
			{
				for (int j = 0; j < 10; j++)
				{
					screenBuffer[i - 4][j + 20] = "▩";
				}
			}
		}

		// 하단
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 30; j++)
			{
				screenBuffer[i + 20][j] = "▩";
			}
		}


		for (int i = 0; i < 22; i++)
		{
			for (int j = 0; j < 30; j++)
			{
				s += screenBuffer[i][j];
			}
			s += '\n';
		}
#pragma endregion

		ScreenPrint(0, 0, s);
		ScreenFlipping();
		
		displayManager->ClearCurrentTetromino();	// Original Coord 공백처리
	}
}
void GameManager::Update()
{
	// LEVEL
	int point = displayManager->GetScore();
	if (point > 1000 * level)
	{
		sndPlaySoundA("s_Level_Up.wav", SND_SYNC | SND_NODEFAULT);
		sndPlaySoundA("s_Loginska.wav", SND_ASYNC | SND_NODEFAULT | SND_LOOP);
		++level;

		difficulty = 0.1f * level;
		speed = 1.1f - difficulty;
	}

	if (point >= 9000)
	{
		bIsFinish = true;
	}

	displayManager->InputValidGameKey(key);		// 입력값에 따른 좌표계산
}
void GameManager::Input()
{
	key = GetInputKey(true);

	if (key == eInputKey::S)
	{
		ResetTimer();
	}

	if (key == eInputKey::SPACE)
	{
		bIsPause = !bIsPause;
	}
}

void GameManager::StartGame(void)
{
	// Start BGM
	sndPlaySoundA("s_Loginska.wav", SND_ASYNC | SND_NODEFAULT | SND_LOOP);

	displayManager = new DisplayManager();
	ScreenInit();

	//Game Loop
	while (displayManager->CheckIsGameOver() == false)
	{
		ScreenClear();

		Render();

		Input();
		
		if (!bIsPause)
			Update();

		if (bIsFinish)
			break;
	}

	CloseHandle(g_hScreen[0]);
	CloseHandle(g_hScreen[1]);
	if (bIsFinish)
		ShowGameClear();
	else
		ShowGameover();

	delete displayManager;
}
void GameManager::ShowHelp(void)
{
	system("cls");
	cout << "가이드 1. 조작키\n\n";
	cout << "A: 테트로미노를 왼쪽으로 이동시킵니다.\n";
	cout << "D: 테트로미노를 오른쪽으로 이동시킵니다.\n";
	cout << "S: 테트로미노를 더 빠르게 아래로 내립니다.(소프트 드롭)\n";
	cout << "C: 테트로미노를 더 빠르게 아래로 내립니다.(하  드 드롭)\n";
	cout << "W: 테트로미노를 좌(반시계 방향)로 90도 회전합니다.\n";
	cout << "SPACE: 게임을 중지시킵니다.\n\n";
	system("pause");
	
	system("cls");
	cout << "가이드 2. 점수 합산\n\n";
	cout << "각 라인 클리어마다 100점이 추가되고,\n";
	cout << "소프트 드롭마다 1점이 추가됩니다.\n";
	cout << "점수 1000점 간격으로 스테이지 레벨이 증가합니다.\n";
	cout << "레벨이 증가하면 블록 하강 속도가 빨라집니다.\n";
	cout << "9000점을 달성 시 게임 클리어.\n\n";
	system("pause");
	system("cls");
}
void GameManager::ShowGameover()
{
	cout << "\n                        GAME OVER! \n\n";
	cout << "                      Score   : " << displayManager->GetScore() << "\n";
	cout << "                      Cleared : " << displayManager->GetClearedLine() << " line";
	if (displayManager->GetClearedLine() > 1)
	{
		cout << "s";
	}
	cout << "\n\n";

	bPlayingBGM = false;
	sndPlaySoundA("s_Game_Over.wav", SND_SYNC | SND_NODEFAULT);
}
void GameManager::ShowGameClear()
{
	cout << "\n                        GAME CLEAR! \n\n";
	cout << "                      Score   : " << displayManager->GetScore() << "\n";
	cout << "                      Cleared : " << displayManager->GetClearedLine() << " line";
	if (displayManager->GetClearedLine() > 1)
	{
		cout << "s";
	}
	cout << "\n\n";

	bPlayingBGM = false;
	bIsFinish	= false;

	sndPlaySoundA("s_Success.wav", SND_SYNC | SND_NODEFAULT);
	Sleep(1000);
}
eMainActions GameManager::ShowTitle(void)
{
	system("cls");

	if(!bPlayingBGM)
		sndPlaySoundA("s_Title_Screen.wav", SND_ASYNC | SND_NODEFAULT | SND_LOOP);
	
	int cursor = 0;
	bPlayingBGM = true;
	
	while (true)
	{
		cout << "\n\n    ######## ######## ######## ########  ####  ######  \n";
		cout << "       ##    ##          ##    ##     ##  ##  ##    ## \n";
		cout << "       ##    ##          ##    ##     ##  ##  ##       \n";
		cout << "       ##    ######      ##    ########   ##   ######  \n";
		cout << "       ##    ##          ##    ##   ##    ##        ## \n";
		cout << "       ##    ##          ##    ##    ##   ##  ##    ## \n";
		cout << "       ##    ########    ##    ##     ## ####  ######  \n\n";

		cout << "            ※ W,S 방향키로 이동, Space키로 선택\n\n";

		for (int i = 0; i < 3; i++)
		{
			if (cursor == i)
			{
				cout << "                   >> ";
			}
			else
			{
				cout << "                     ";
			}

			switch (i)
			{
			case 0:
				cout << "  게 임 시 작  ";
				break;
			case 1:
				cout << "  게임 가이드  ";
				break;
			case 2:
				cout << "  게 임 종 료  ";
				break;
			default:
				assert(false);
				break;
			}

			if (cursor == i)
			{
				cout << "<<";
			}
			cout << "\n";
		}

		try
		{
			eInputKey key = GetInputKey(false);
			switch (key)
			{
			case eInputKey::W:
				cursor--;
				if (cursor < 0) cursor = 0;
				break;
			case eInputKey::S:
				cursor++;
				if (cursor > 2) cursor = 2;
				break;
			case eInputKey::SPACE:
				if (cursor == 0) return eMainActions::START;
				else if (cursor == 1) return eMainActions::HELP;
				else return eMainActions::EXIT;
				break;
			default:
				break;
			}
			system("cls");
		}
		catch (const std::exception&)
		{
			system("cls");
		}
	}
}

void GameManager::ResetTimer(void)
{
	t = clock();
}
bool GameManager::CheckTimePassed()
{
	if ((clock() - (float)t) / CLOCKS_PER_SEC >= speed)
	{
		ResetTimer();
		return true;
	}

	return false;
}

eInputKey GameManager::GetInputKey(bool bIsPlaying)
{
	while (true)
	{
		if (bIsPlaying && CheckTimePassed())
		{
			return eInputKey::TIME_PASSED;
		}

		int input = 0;
		if (_kbhit())
		{
			input = _getch();
			
			if (input == 32)
			{
				return eInputKey::SPACE;
			}
			if (input == 'a' || input == 'A')
			{
				return eInputKey::A;
			}
			if (input == 'w' || input == 'W')
			{
				return eInputKey::W;
			}
			if (input == 's' || input == 'S')
			{
				return eInputKey::S;
			}
			if (input == 'd' || input == 'D')
			{
				return eInputKey::D;
			}
			if (input == 'c' || input == 'C')
			{
				return eInputKey::C;
			}
		}
	}
}

