
#pragma once
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"
#include "Haybale.h"
#include <string>
#include <vector>
#include <time.h>


class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();
	SDL_Texture* LoadFromFile(std::string);
private:
	void Welcome();
	void ProcessInput();
	void UpdateGame();
	void IncrementRunCycle();
	void GenerateOutput();
	void UpdateEnvironment();
	void SpawnHayBales();
	void UnloadData();
	bool OffScreen();

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	SDL_Texture* mTexture;
	Uint32 mTicksCount;
	bool mIsRunning;
	
	Vector2 mPlayerPos;
	Vector2 mPlayerDir;
	int mPlayerFacing;
	int mPlayerLives;
	int mCycle;
	int mCycleCount;
	int mHayBales;
	int mMaxHayBales;
	std::vector<Haybale> myHaybales;
	Mix_Chunk* mChomp;
	Mix_Music* mBackground;
	int mMusicplaying;
};
