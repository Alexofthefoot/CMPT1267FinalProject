
#pragma once
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"
#include "Haybale.h"
#include "Enemy.h"
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
	void Togglemusic();
	void GenerateOutput();
	void UpdateEnvironment();
	void SpawnHayBales();
	void UnloadData();
	void GameOverScreen();

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	SDL_Texture* mTexture;
	Uint32 mTicksCount;
	bool mIsRunning;
	
	Vector2 mPlayerPos;
	Vector2 mPlayerDir;
	int mPlayerFacing;
	int mPlayerLives;
	int playerWidth;
	int playerHeight;
	int enemyWidth;
	int enemyHeight;
	int mCycle;
	int mCycleCount;
	int mHayBales;
	int mMaxHayBales;
	std::vector<Haybale> myHaybales;
	std::vector<Enemy> myEnemies;
	Mix_Chunk* mChomp;
	Mix_Music* mBackground;
	int mMusicplaying;
	int gamescore;
	
};
