#include "Enemy.h"
#include <time.h>



void Enemy::Initialize()
{
	mEnemyPos.x = 10.0;
	mEnemyPos.y = 10.0;
	mEnemyDir.x = 0.0;
	mEnemyDir.y = 0.0;
}
int Enemy::GetXPosition()
{
	return mEnemyPos.x;
}

int Enemy::GetYPosition()
{
	return mEnemyPos.y;
}

bool Enemy::UpdatePosition()
{
	return true;
}
