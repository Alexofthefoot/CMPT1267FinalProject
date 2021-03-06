#include <stdio.h>
#include "Game.h"
#include "SDL/SDL_image.h"

//using namespace std;

enum PlayerFacing
{
	Player_Facing_Up ,		//0
	Player_Facing_Left,		//1
	Player_Facing_Down,		//2
	Player_Facing_Right,	//3
};

Game::Game()
{
	mWindow = nullptr;
	mRenderer = nullptr;
	mTexture = nullptr;
	mFontTexture1 = nullptr; //players score, updated every haybale
	mFontTexture2 = nullptr; // Ouch!
	mChomp = nullptr;
	mBackground = nullptr;
	mFont = nullptr;

	mIsRunning = true;
	mPlayerFacing = Player_Facing_Right; 
	mPlayerLives = 3;
	gamescore = 0;
	mCycle = 0;
	mMaxHayBales = 3;

	playerWidth = 130;
	playerHeight = 130;
	enemyWidth = 150;
	enemyHeight = 140;
	injury = false;

}

bool Game::Initialize()
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// Initialize IMG
	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
		return false;
	}

	// Initialize TTF 
	if (TTF_Init() < 0)
	{	
		SDL_Log("Unable to initialize TTF: %s", SDL_GetError());
	}
	// Load font
	mFont = TTF_OpenFont("Assets/Dubai-Regular.ttf", 28);
	if (!mFont)
	{
		return false;
	}

	// Create an SDL Window
	mWindow = SDL_CreateWindow( "CMPT 1267", 100, 100, 1024, 768, 0	);

	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	//// Create SDL renderer
	mRenderer = SDL_CreateRenderer(mWindow, -1,	SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );

	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	// Load the music and sound effects
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	mBackground = Mix_LoadMUS("Assets/jauntytune.wav");
	mChomp = Mix_LoadWAV("Assets/Cow2.wav");
	Mix_PlayMusic(mBackground, -1);

	// Load the words for Fonttexture2 (doesnt change throughout the game)
	std::string textureText = "You got eaten";

	//Put it in the texture
	SDL_Colour textColour{ 255, 10, 15 };
	SDL_Surface* textSurface = nullptr;
	textSurface = TTF_RenderText_Solid(mFont, textureText.c_str(), textColour);
	mFontTexture1 = SDL_CreateTextureFromSurface(mRenderer, textSurface);
	SDL_FreeSurface(textSurface);
	

	// Display the welcome screen & wait for further action
	Welcome();

	UpdateScoreText();

	// if there is any texture loaded, destroy it. Otherwise set it to the sprite sheet
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
	}
	mTexture = LoadFromFile("Assets/spritesheet.png");
	if (mTexture == NULL)
	{
		SDL_Log("Unable to load png file: %s", SDL_GetError());
		return false;
	}

	//Set players starting position
	mPlayerPos.x = 1024.0f / 2.0f; 
	mPlayerPos.y = 768.0f / 2.0f;
	mPlayerDir.x = 0.0f;
	mPlayerDir.y = 0.0;

	//Create bear
	Enemy newenemy;
	newenemy.Initialize();
	myEnemies.push_back(newenemy);

	return true;
}

void Game::Welcome()
{
	srand(time(0));

	//Display the instruction/welcome screen
	mTexture = IMG_LoadTexture(mRenderer, "Assets/WelcomeScreen.png");
	SDL_RenderCopy(mRenderer, mTexture, NULL, NULL);
	SDL_RenderPresent(mRenderer);

	//Wait for user to do something
	SDL_Event event;
	while (true)
	{
		if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
		{
			mIsRunning = false;
			return;
		}
		else if (event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.sym == SDLK_RETURN)
			{
				mTexture = nullptr;
				return;
			}
			if (event.key.keysym.sym == SDLK_p)
			{
				Togglemusic();
			}
		}
	}
}

void Game::Togglemusic()
{
	if (Mix_PlayingMusic() == 0)
	{
		Mix_PlayMusic(mBackground, -1);
	}
	else
	{
		if (Mix_PausedMusic() == 1)
		{
			Mix_ResumeMusic();
		}
		else
		{
			Mix_PauseMusic();
		}
	}
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
	void GameOverScreen();
}

void Game::IncrementRunCycle()
{
	mCycleCount++;
	if (mCycleCount > 12)
	{
		mCycleCount = mCycleCount % 12;
	}
	mCycle = mCycleCount / 6;
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	// Get state of keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);

	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	mPlayerDir.x = 0.0f;
	mPlayerDir.y = 0.0f;

	// Update player direction and movement based on W/A/S/D
	if (state[SDL_SCANCODE_W])
	{
		mPlayerDir.y -= 4.0f;
		mPlayerFacing = Player_Facing_Up;
	}
	if (state[SDL_SCANCODE_S])
	{
		mPlayerDir.y += 4.0f;
		mPlayerFacing = Player_Facing_Down;
	}
	if (state[SDL_SCANCODE_A])
	{
		mPlayerDir.x -= 4.0f;
		mPlayerFacing = Player_Facing_Left;
	}
	if (state[SDL_SCANCODE_D])
	{
		mPlayerDir.x += 4.0f;
		mPlayerFacing = Player_Facing_Right;
	}
	// Pause/play music
	if (state[SDL_SCANCODE_P])
	{
		Togglemusic();
	}
	IncrementRunCycle();
}

void Game::UpdateGame()
{
	UpdateCowPosition();

	for (int e=0; e<myEnemies.size(); e++)
	{
		myEnemies[e].UpdatePosition(mPlayerPos);
		if (myEnemies[e].CanAttack(mPlayerPos))
		{
			//Some attack sound effect
			
			//some injury occurs
			injury = true;
			mPlayerLives--;
			//reset the bear
			myEnemies[e].ResetPosition(mPlayerPos);
			if (mPlayerLives < 0)
			{
				mIsRunning = false;
			}
		}
	}

	UpdateEnvironment();

}

void Game::UpdateScoreText()
{
	//Update the text
	std::string scorestring = std::to_string(gamescore);
	std::string textureText = "Your Score: " + scorestring;

	//Put it in the texture
	SDL_Colour textColour {28, 62, 11};
	SDL_Surface* textSurface = nullptr;
	textSurface = TTF_RenderText_Solid(mFont, textureText.c_str(), textColour);
	mFontTexture1 = SDL_CreateTextureFromSurface(mRenderer, textSurface);
	SDL_FreeSurface(textSurface);
}

void Game::UpdateEnvironment()
{
	//Spawn haybales
	SpawnHayBales();

	//Check for collision between player/haybales
	for (int h=0; h<myHaybales.size(); h++)
	{
		if (myHaybales[h].IsCloseto(mPlayerPos))
		{
			//play the sound effect, update the score, and remove the haybale in question
			Mix_PlayChannel(-1, mChomp, 0);
			gamescore += 5;
			UpdateScoreText();
			myHaybales.erase(myHaybales.begin() + h);
		}
	}
}

void Game::UpdateCowPosition()
{
	// Update player position based on direction
	mPlayerPos.x += mPlayerDir.x;
	mPlayerPos.y += mPlayerDir.y;

	// Did the player go off the screen in the x direction?
	if (mPlayerPos.x >= 1024 - playerWidth * 1.5)
	{
		mPlayerPos.x = 1024 - playerWidth * 1.5;
	}
	else if (mPlayerPos.x <= 0 - playerWidth * 0.5)
	{
		mPlayerPos.x = -playerWidth * 0.5;
	}
	// in y direction?
	if (mPlayerPos.y >= 768 - playerHeight * 1.5)
	{
		mPlayerPos.y = 768 - playerHeight * 1.5;
	}
	else if (mPlayerPos.y <= 0 - playerHeight * 0.5)
	{
		mPlayerPos.y = -playerHeight * 0.5;
	}
}

void Game::SpawnHayBales()
{
	if (myHaybales.size() < mMaxHayBales)
	{
		Haybale newhaybale;
		newhaybale.Initialize();
		myHaybales.push_back(newhaybale);
	}
}

void Game::CowInjury()
{
	//show "OUCH" text above the character
	//pause for a moment
	//reset bear
}

void Game::GenerateOutput()
{
	SDL_RenderClear(mRenderer);
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 0);
	// First the grassy background
	SDL_Rect src_Rect{ 985, 0, 610, 460 };
	SDL_RenderCopy(mRenderer, mTexture, &src_Rect, NULL);

	// Next the images of haybales
	src_Rect.x = 500;
	src_Rect.y = 0;
	src_Rect.w = 460;
	src_Rect.h = 450;

	SDL_Rect Hay_Rect { 0, 0, 100, 100 };

	for (Haybale h : myHaybales)
	{
		Hay_Rect.x = h.GetXPosition();
		Hay_Rect.y = h.GetYPosition();
		//SDL_RenderFillRect(mRenderer, &Hay_Rect);
		SDL_RenderCopy(mRenderer, mTexture, &src_Rect, &Hay_Rect);
	}

	// Next the cow image/main character
	src_Rect.x = 128*mCycle;
	src_Rect.y = 128*mPlayerFacing;
	src_Rect.w = 128;
	src_Rect.h = 128;

	SDL_Rect Player_Rect{ mPlayerPos.x, mPlayerPos.y, playerWidth*2, playerHeight*2 };	
	SDL_RenderCopy(mRenderer, mTexture, &src_Rect, &Player_Rect);


	// Next display the health bar / lives remaining
	SDL_Rect Healthbar_Rect{ 5, 5, 75, 75 };
	src_Rect.y = 128 * 3; //keep the animation but always facing right
	for (int i = 0; i < mPlayerLives; i++)
	{
		SDL_RenderCopy(mRenderer, mTexture, &src_Rect, &Healthbar_Rect);
		Healthbar_Rect.x += 60;
	}
	// Next the running score total (top right)
	SDL_Rect textrect{ 850, 10, 155, 48 };
	SDL_RenderCopyEx(mRenderer, mFontTexture1, NULL, &textrect, NULL, NULL, SDL_FLIP_NONE);

	// Finally dsplay the enemy/bear image
	SDL_Rect Enemy_Rect{ 0, 0, enemyWidth, enemyHeight };
	src_Rect.x = 1595;
	src_Rect.y = 0;
	src_Rect.w = 845;
	src_Rect.h = 700;

	for (Enemy e : myEnemies)
	{	//using a loop so I can add multiple enemies later if desired 
		Enemy_Rect.x = e.GetXPosition();
		Enemy_Rect.y = e.GetYPosition();
		SDL_RenderCopy(mRenderer, mTexture, &src_Rect, &Enemy_Rect);
	}

	//this is meant to print something over top the player's character when the bear eats them...still a work in progress
	if (injury)
	{ 
		SDL_Rect ouchrect{ mPlayerPos.x + playerWidth/2, mPlayerPos.y - 15};
		SDL_RenderCopyEx(mRenderer, mFontTexture2, NULL, &ouchrect, NULL, NULL, SDL_FLIP_NONE);
		//SDL_Delay(1000);
		injury = false;
	}

	SDL_RenderPresent(mRenderer);
}

SDL_Texture* Game::LoadFromFile(std::string filename)
{
	SDL_Surface* loadedSurface = IMG_Load(filename.c_str());
	SDL_Texture* mTexture = SDL_CreateTextureFromSurface(mRenderer, loadedSurface);

	if (loadedSurface == nullptr)
	{
		printf("Error! Cannot Load the Image!!");
		SDL_FreeSurface(loadedSurface);
		return false;
	}
	
	SDL_FreeSurface(loadedSurface);
	return mTexture;
	
}

void Game::UnloadData()
{
	//Free loaded images
	if (mTexture != NULL)
	{
		mTexture = NULL;
	}
}

void Game::GameOverScreen()
{
	int i_am_a_placeholder = 4;
}

void Game::Shutdown()
{
	UnloadData();
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	IMG_Quit();
	SDL_Quit();
}