#include "GameManager.h"
#include "pch.h"
#include<MMSystem.h>				//SoundPlay
#pragma comment(lib,"Winmm.lib")	// SoundPlay


int main(void)
{
#pragma region unvisibleCursor
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.dwSize = 1; //커서 굵기 (1 ~ 100)
	cursorInfo.bVisible = FALSE; //커서 Visible TRUE(보임) FALSE(숨김)
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
#pragma endregion

	system("mode con cols=60 lines=30 | title Tetris");
	GameManager* gameManager = new GameManager();
	
	eMainActions selection;

	while (true)
	{
		selection = gameManager->ShowTitle();

		switch (selection)
		{
		case eMainActions::START:
			system("cls");
			gameManager->StartGame();
			break;
		case eMainActions::HELP:
			gameManager->ShowHelp();
			break;
		case eMainActions::EXIT:
			delete gameManager;
			return 0;
			break;
		default:
			assert(false);
			break;
		}
	}

	return 0;
}
