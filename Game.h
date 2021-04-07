
#pragma once
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_ttf.h"
#include "Haybale.h"
#include "Enemy.h"
#include <string>
#include <iostream>
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
	void UpdateCowPosition();
	void UpdateEnvironment();
	void SpawnHayBales();
	void UpdateScoreText();
	void IncrementRunCycle();
	void Togglemusic();
	void GenerateOutput();
	void UnloadData();
	//Currently not in use:
	void CowInjury();
	void GameOverScreen();

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	SDL_Texture* mTexture;
	SDL_Texture* mFontTexture1; 
	SDL_Texture* mFontTexture2;
	TTF_Font* mFont;
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
	bool injury;

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
