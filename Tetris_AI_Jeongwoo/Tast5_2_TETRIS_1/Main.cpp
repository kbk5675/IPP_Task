#include "GameManager.h"
#include "pch.h"
#include<MMSystem.h>				//SoundPlay
#pragma comment(lib,"Winmm.lib")	// SoundPlay


int main(void)
{//커서 안 보이게 설정
#pragma region unvisibleCursor
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.dwSize = 1; //커서 굵기 (1 ~ 100)
	cursorInfo.bVisible = FALSE; //커서 Visible TRUE(보임) FALSE(숨김)
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
#pragma endregion
	//커맨드 창 크기 설정
	system("mode con cols=60 lines=30 | title Tetris");
	//게임 매니져 객체 생성
	GameManager* gameManager = new GameManager();
	//게임시작 선택하는 화면 객체
	eMainActions selection;

	while (true)//루프 돌아가는 곳
	{
		selection = gameManager->ShowTitle();//게임시작 선택하는 하면

		switch (selection)
		{
		case eMainActions::START:
			system("cls");
			gameManager->StartGame();//걍 게임 시작 해 버리기
			break;
		case eMainActions::HELP:
			gameManager->ShowHelp();//걍 게임 설명
			break;
		case eMainActions::EXIT:
			delete gameManager;//걍 나가버리기
			return 0;
			break;
		default:
			assert(false);//디버깅 모드에서 개발자가 오류가 생기면 치명적일 것이라는 곳에 심어 놓는 에러 검출용 코드입니다.
			break;
		}
	}

	return 0;
}
