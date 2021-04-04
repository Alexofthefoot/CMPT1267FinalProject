#pragma once
#include "SDL/SDL.h"
#include "Haybale.h"
#include <string>


class Enemy
{

public:
	void Initialize();
	int GetXPosition();
	int GetYPosition();
	bool UpdatePosition();


	Vector2 mEnemyPos;
	Vector2 mEnemyDir;
	int size;

};
