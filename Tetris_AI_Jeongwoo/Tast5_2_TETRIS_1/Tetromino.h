#pragma once
#include "pch.h"

class Tetromino
{
public:
	Tetromino(eTetromino t);

	eTetromino GetType() const;
	static int GetSpecificShape(eTetromino t, const int rot, const int r, const int c);
	static eTetromino GetRandomTetromino();
	static bool CheckIsBagFull();

	int GetShape(const int r, const int c) const;
	int GetCoordinateX() const;
	int GetCoordinateY() const;
	int GetRotateLevel();

	void SetCoordinate(const int r, const int c);
	void Rotate(const eRotate rot);

private:
	const static int tetrominoes[7][4][4][4];	//[��Ÿ��][ȸ��Ÿ��][������-��][������-��]
	std::pair<int, int> coordinate;	//4*4 �迭���� [0][0]�� ��ǥ
	eTetromino type;

	int shape[4][4];
	int currentRotateLevel;

	static bool bag[7];	//��Ʈ�ι̳� ���� ����Ŭ�� ���� �迭
};