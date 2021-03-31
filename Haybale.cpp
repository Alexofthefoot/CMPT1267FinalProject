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
	int size = 1;
	mHayBalePos.x = randomize(minX, maxX);
	mHayBalePos.y = randomize(minY, maxY);
}

int Haybale::GetXPosition()
{
	return mHayBalePos.x;
}

int Haybale::GetYPosition()
{
	return mHayBalePos.y;
}

//100,	// Top left x-coordinate of window
//100,	// Top left y-coordinate of window
//1024,	// Width of window
//768,	// Height of window