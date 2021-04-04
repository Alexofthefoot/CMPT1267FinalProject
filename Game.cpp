#include <stdio.h>
#include "Game.h"
#include "SDL/SDL_image.h"

using namespace std;

enum PlayerFacing
{
	Player_Facing_Up ,		//0
	Player_Facing_Left,		//1
	Player_Facing_Down,		//2
	Player_Facing_Right,	//3
};

//enum PlayerRunCycle
//{
//	cycle = 0
//};

Game::Game()
{
	mWindow = nullptr;
	mRenderer = nullptr;
	mTexture = nullptr;
	mTicksCount = 0;
	mIsRunning = true;
	mPlayerFacing = Player_Facing_Right; 
	mPlayerLives = 3;
	mCycle = 0;
	mHayBales = 0;
	mMaxHayBales = 3;
	mChomp = nullptr;
	mBackground = nullptr;
	playerWidth = 130;
	playerHeight = 130;
	gamescore = 0;
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

	// Create an SDL Window
	mWindow = SDL_CreateWindow(
		"CMPT 1267", // Window title
		100,	// Top left x-coordinate of window
		100,	// Top left y-coordinate of window
		1024,	// Width of window
		768,	// Height of window
		0		// Flags (0 for no flags set)
	);

	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	//// Create SDL renderer
	mRenderer = SDL_CreateRenderer(
		mWindow, // Window to create renderer for
		-1,		 // Usually -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	// Set up the music and sound effects
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	mBackground = Mix_LoadMUS("Assets/jauntytune.wav");
	mChomp = Mix_LoadWAV("Assets/Cow2.wav");
	Mix_PlayMusic(mBackground, -1);


	// Display the welcome screen & wait for further action
	Welcome();

	// if there is any texture loaded, destroy it. Otherwise set it to the sprite sheet
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
	}
	mTexture = LoadFromFile("Assets/spritesheet2.png");
	if (mTexture == NULL)
	{
		SDL_Log("Unable to load png file: %s", SDL_GetError());
		return false;
	}

	//Set players info
	mPlayerPos.x = 1024.0f / 2.0f; 
	mPlayerPos.y = 768.0f / 2.0f;
	mPlayerDir.x = 0.0f;
	mPlayerDir.y = 0.0;

	//Create wolf
	Wolf newwolf;
	newwolf.Initialize();
	myWolves.push_back(newwolf);

	return true;
}

void Game::Welcome()
{
	srand(time(0));

	//Display the instruction/welcome screen
	mTexture = IMG_LoadTexture(mRenderer, "Assets/WelcomeScreen.png");
	SDL_RenderCopy(mRenderer, mTexture, NULL, NULL);
	SDL_RenderPresent(mRenderer);

	//Wait for user
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
	// Update tick counts (for next frame)
	mTicksCount = SDL_GetTicks();


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
		mPlayerPos.y = - playerHeight * 0.5;
	}


	UpdateEnvironment();
}

void Game::UpdateEnvironment()
{
	//Spawn haybales
	SpawnHayBales();

	for (int h=0; h<myHaybales.size(); h++)
	//for (auto iter = myHaybales.begin(); iter != myHaybales.end();)
	{
		//printf("now checking haybale %d\n", h);
		if (myHaybales[h].IsCloseto(mPlayerPos))
		{
			printf("Collision Happened! with bale %d\n", h);
			Mix_PlayChannel(-1, mChomp, 0);
			myHaybales.erase(myHaybales.begin() + h);
			//swap
			//put to the end
			//myHaybales.pop_back();
		}
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


void Game::GenerateOutput()
{
	// Set background draw color to green
	SDL_SetRenderDrawColor(mRenderer,0,	230, 0, 255);
	// Clear back buffer
	SDL_RenderClear(mRenderer);
	
	//First set the grassy background
	SDL_Rect srcRect{985,0,610,460};
	SDL_RenderCopy(mRenderer, mTexture, &srcRect, NULL);

	//then hay
	srcRect.x = 500;
	srcRect.y = 0;
	srcRect.w = 460;
	srcRect.h = 450;

	SDL_Rect Hay_Rect{0,0,100, 100};

	for (Haybale h : myHaybales)
	{
		Hay_Rect.x = h.GetXPosition();
		Hay_Rect.y = h.GetYPosition();
		//SDL_RenderFillRect(mRenderer, &Hay_Rect);
		SDL_RenderCopy(mRenderer, mTexture, &srcRect, &Hay_Rect);
	}


	//then character
	srcRect.x = 128*mCycle;
	srcRect.y = 128*mPlayerFacing;
	srcRect.w = 128;
	srcRect.h = 128;

	SDL_Rect Player_Rect{
		mPlayerPos.x,			// Top left x
		mPlayerPos.y,			// Top left y
		playerWidth*2,			// Width
		playerHeight*2			// Height
	};	
	//SDL_RenderFillRect(mRenderer, &Player_Rect);												 // FOR THE THE SQUARE AROUND THE IMAGE

	SDL_RenderCopy(mRenderer, mTexture, &srcRect, &Player_Rect);


	// Display remaining lives/health in top corner (for now using the same haybale image)
	SDL_Rect Healthbar_Rect{ 5,5,50,50 };
	srcRect.y = 128 * 3; //have it always facing right
	for (int i = 0; i < mPlayerLives; i++)
	{
		//SDL_RenderFillRect(mRenderer, &Healthbar_Rect);										// FOR THE SQUARE AROUND THE IMAGE
		SDL_RenderCopy(mRenderer, mTexture, &srcRect, &Healthbar_Rect);
		Healthbar_Rect.x += 60;
	}
	//finally the predator image 
	SDL_Rect Wolf_Rect{

	};


	// Swap front buffer and back buffer (making it all visible)
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
	int this_is_nothing_can_delete = 7;
}

void Game::Shutdown()
{
	UnloadData();
	SDL_DestroyRenderer(mRenderer);
	//SDL_FreeSurface(mSurface);
	SDL_DestroyWindow(mWindow);
	IMG_Quit();
	SDL_Quit();
}