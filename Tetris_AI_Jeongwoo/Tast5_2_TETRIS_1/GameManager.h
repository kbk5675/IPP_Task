#pragma once
#include "tetromino.h"
#include "DisplayManager.h"
#include "pch.h"

class GameManager
{
public:
	GameManager();

	eMainActions ShowTitle();
	eInputKey GetInputKey(bool bIsPlaying);

	void Render();
	void Update();
	void Input();

	void StartGame();
	void ShowHelp();
	void ShowGameover();
	void ShowGameClear();
	void ResetTimer();

	void ScreenInit();
	void ScreenFlipping();
	void ScreenPrint(int x, int y, std::string s);
	void ScreenClear();

	int GetLevel() { return level; }
	float GetSpeed() { return speed; }

	bool CheckTimePassed();

	//void FindBestPlace();//醤ちちちちちちちちちちちちちちちちちちち

	

private:
	clock_t t;

	int index = 0;
	int getScore;
	int level;

	float difficulty;
	float speed;

	bool bIsPause;
	bool bPlayingBGM;
	bool bIsFinish;

	const char* screenBuffer[22][30] = { "\n", };

	eInputKey key = eInputKey::NONE;
	DisplayManager* displayManager;



};
