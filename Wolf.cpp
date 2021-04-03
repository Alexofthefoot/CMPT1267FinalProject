#include "Wolf.h"
#include <time.h>



void Wolf::Initialize()
{
	mWolfPos.x = 10.0;
	mWolfPos.y = 10.0;
	mWolfDir.x = 0.0;
	mWolfDir.y = 0.0;
}
int Wolf::GetXPosition()
{
	return mWolfPos.x;
}

int Wolf::GetYPosition() 
{
	return mWolfPos.y;
}

bool Wolf::UpdatePosition()
{
	return true;
}
