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
	void UpdatePosition();
	bool CanAttack(Vector2);

	Vector2 mEnemyPos;
	Vector2 mEnemyDir;

};
