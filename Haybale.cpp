#include "Haybale.h"
#include <time.h>

int randomize(int min, int max)
{
	return rand() % max + min;
}

void Haybale::Initialize()
{
	int minX = 0;
	int minY = 0;
	int maxX = 600;
	int maxY = 600;
	int size = randomize(1, 3);
	mHayBalePos.x = randomize(minX, maxX);
	mHayBalePos.y = randomize(minY, maxY);
	mHayBaleDir.x = 0.0;
	mHayBaleDir.y = 0.0;
}

//this function is under construction and not currently called anywhere
bool Haybale::UpdatePosition()
{
	mHayBalePos.x += mHayBaleDir.x;
	mHayBalePos.y += mHayBaleDir.y;

	// Did the hay go off the screen in the x direction?
	if (mHayBalePos.x >= 1024 || mHayBalePos.x <= 0)
	{
		return false;
	}
	// in y direction?
	if (mHayBalePos.y >= 768 || mHayBalePos.y <= 0)
	{
		return true;
	}
	return true;
}

int Haybale::GetXPosition()
{
	return mHayBalePos.x;
}

int Haybale::GetYPosition()
{
	return mHayBalePos.y;
}


//Check if the current haybale is within ____ distance of the item asked about
bool Haybale::IsClose(Vector2 )
{
	int sizeofbale = 100;
	return true;
}


//100,	// Top left x-coordinate of window
//100,	// Top left y-coordinate of window
//1024,	// Width of window
//768,	// Height of window