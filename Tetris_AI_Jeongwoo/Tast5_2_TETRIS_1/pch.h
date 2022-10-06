#pragma once
#define NOMINMAX	//Windows.h���� min, max �Լ� shadowing ����
#include <iostream>	
#include <vector>	
#include <string>	
#include <cassert>	//Assert���� �ʿ�
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

