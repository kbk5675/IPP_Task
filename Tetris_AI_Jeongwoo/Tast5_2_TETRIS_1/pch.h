#pragma once
#define NOMINMAX	//Windows.h에서 min, max 함수 shadowing 방지
#include <iostream>	
#include <vector>	
#include <string>	
#include <cassert>	//Assert문에 필요
#include <conio.h>
#include <random>	
#include <Windows.h>	

enum class eMainActions
{
	START,
	HELP,
	EXIT
};

enum class eInputKey
{
	SPACE,
	A,
	W,
	S,
	D,
	C,
	TIME_PASSED,
	NONE
};

enum class eTetromino
{
	I,
	O,
	T,
	J,
	L,
	S,
	Z
};

enum class eRotate
{
	CLOCKWISE,
	COUNTERCLOCKWISE
};

enum class eCollisionDirection
{
	LEFT,
	RIGHT,
	BOTH,
	NOT_COLLIDED
};

