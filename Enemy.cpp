#include "Enemy.h"
#include <time.h>


//return the distance between 2 points
float Enemy::Getdistance(Vector2 pos1, Vector2 pos2)
{
	return 0.0;
}

void Enemy::Initialize()
{
	mEnemyPos.x = 800.0;
	mEnemyPos.y = 600.0;
	mEnemyDir.x = 2.0;
	mEnemyDir.y = 2.0;
}

//Used after a collision to give the player a chance to start over
void Enemy::ResetPosition(Vector2 cowposition)
{
	//if player is left of centre
	if (cowposition.x < 1024 / 2)
	{
		mEnemyPos.x = 600.0;
		mEnemyPos.y = 600.0;
	}
	//if player is right of centre
	if (cowposition.x >= 1024 / 2)
	{
		mEnemyPos.x = 50.0;
		mEnemyPos.y = 50.0;
	}
}

int Enemy::GetXPosition()
{
	return mEnemyPos.x;
}

int Enemy::GetYPosition()
{
	return mEnemyPos.y;
}

//I am responsible for updating position (based on pre-determined direction)
void Enemy::UpdatePosition(Vector2 cowposition)
{//for now just bouncing around like pong
	mEnemyPos.x += mEnemyDir.x;
	if (mEnemyPos.x <= 0 || mEnemyPos.x >= 1024 - 150)
		mEnemyDir.x = -mEnemyDir.x;

	mEnemyPos.y += mEnemyDir.y;
	if (mEnemyPos.y <= 0 || mEnemyPos.y >= 768 - 140)
		mEnemyDir.y = -mEnemyDir.y;
}


//I am responsible for changing direction
bool Enemy::CanAttack(Vector2 cowposition)
{
	int sizeofenemy = 145;
	int sizeofcow = 260;

	float x = (cowposition.x + sizeofcow / 2) - (mEnemyPos.x + sizeofenemy / 2);
	if (x < 0)
	{
		x = -x;
	}
	float y = (cowposition.y + sizeofcow / 2) - (mEnemyPos.y + sizeofenemy / 2);
	if (y < 0)
	{
		y = -y;
	}
	float dist = sqrtf((x * x) + (y * y));

	//printf("distance is %f\n", dist);
	if (dist < sizeofenemy / 2) //aka if distance
	{
		//for now just freeze so I know its happening
		SDL_Delay(50);
		return true;
	}
	return false;
}
