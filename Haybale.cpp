#include "Haybale.h"
#include <time.h>

int randomize(int min, int max)
{
	return rand() % max + min;
}

void Haybale::Initialize()
{
	int minX = 20;
	int minY = 20;
	int maxX = 1024 - 100 - 20;
	int maxY = 768 - 100 - 20;
	int size = randomize(1, 3);
	bool isedible = false;
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


//Check if the current haybale is within eating distance of the cow
bool Haybale::IsCloseto(Vector2 position)
{
	int sizeofbale = 100;
	int sizeofcow = 260;

	float x = (position.x + sizeofcow / 2) - (mHayBalePos.x + sizeofbale / 2);
	if (x < 0)
	{
		x = -x;
	}
	float y = (position.y + sizeofcow / 2) - (mHayBalePos.y + sizeofbale / 2);
	if (y < 0)
	{
		y = -y;
	}
	float dist = sqrtf((x*x) + (y*y));
	
	//printf("distance is %f\n", dist);
	if (dist < sizeofbale / 2) //aka if distance
	{
		//printf("Collision!!\n");
		return true;
	}
	return false;
}


void Haybale::Remove()
{

}

//100,	// Top left x-coordinate of window
//100,	// Top left y-coordinate of window
//1024,	// Width of window
//768,	// Height of window