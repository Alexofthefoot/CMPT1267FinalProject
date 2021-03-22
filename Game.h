
#pragma once
#include "SDL/SDL.h"
#include <string>
struct Vector2
{
	float x;
	float y;
};

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();
	bool LoadFromFile(std::string);
private:
	void Welcome();
	void ProcessInput();
	void UpdateGame();
	void IncrementRunCycle();
	void GenerateOutput();
	void DrawPlayer();
	void UnloadData();
	bool OffScreen();

	//SDL_Surface* mSurface;
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	SDL_Texture* mTexture;
	Uint32 mTicksCount;

	bool mIsRunning;
	int mPaddleDir;
	int mPaddleDir2;

	Vector2 mPaddlePos;
	Vector2 mPaddlePos2;
	Vector2 mPlayerPos;
	Vector2 mPlayerDir;
	int mPlayerFacing;
	int mPlayerLives;
	int mCycle;
	int mCycleCount;
};
