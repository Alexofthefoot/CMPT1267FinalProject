#pragma once
#include "SDL/SDL.h"
#include "Haybale.h"
#include <string>


class Wolf
{

public:
	void Initialize();
	int GetXPosition();
	int GetYPosition();
	bool UpdatePosition();


	Vector2 mWolfPos;
	Vector2 mWolfDir;
	int size;

};
