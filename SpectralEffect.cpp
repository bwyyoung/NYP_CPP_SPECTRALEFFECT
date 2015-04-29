#include "SpectralEffect.h"

//pointer to run engine as a thread
SpectralEffect *Engine;
//handle to the main game thread
HANDLE gameThread;
//set this to true when the game is running, setting it to false will cause the game to break out of the game loop
bool gameRunning;

//function prototypes
DWORD WINAPI beginGame(LPVOID dummy);

void SpectralEffect::ExitSpectralEffect()
{
	if (mShutDown != true)
	{
		gameRunning = false;
		while(gameRunning == false)
			Sleep(100);
		testpipe.Shutdown();
		ShutDown();
	}
}

bool SpectralEffect::InitSpectralEffect(HWND hWnd)
{
	mShutDown = false;
	SpectralEffect::hWnd = hWnd;
	if (!InitDX9(hWnd))
	{
		//initialization of directx has failed
		MessageBox (NULL, "DirectX has failed to initialize!", "Spectral Effect", MB_OK);
		return false;
	}

	unsigned char keys[] = { DIK_A , DIK_S, DIK_D };

	InitBryoKeys(hWnd, keys, 3);

	return true;
}

void SpectralEffect::RunGame()
{
	Engine = this;
	DWORD tid;
	gameRunning = true;
	gameThread = CreateThread(NULL, 0, beginGame, 0, 0, &tid);
}

DWORD WINAPI beginGame(LPVOID dummy)
{
	Engine->InitInGame();
	return 0;
}

void SpectralEffect::InitInGame()
{
	test.LoadFiles("Data.zip", d3dd);

	while (test.GetProgress() != 100)
	{
		Sleep (100);
	}
	
	string temp[1];
	temp[0] = "YellowBall.png";
	testpipe.SetPipeType(d3dd, 100, 100, 280, 5, 1, temp, 1, "Pipe.png");
	testcannons.InitCannonType(&testpipe, 30, 20, 100, 15, "Cannon.png");
	int time = 0, speed = 1;
	SetFPS(30);
	while (gameRunning)
	{
		StartFrame();
		BeginRender();
		testcannons.Render();
		testpipe.Render();
		EndRender();
		if (time > 500)
		{
			speed ++;
			testpipe.SetSpeed(speed);
			time = 0;
		}
		time ++;
		EndFrame();
	}

	gameRunning = true;
}

void SpectralEffect::BryoKeyDown()
{
	if (GetKeyEvent(DIK_A))
		testcannons.LoadCannon(0);
	if (GetKeyEvent(DIK_S))
		testcannons.LoadCannon(1);
	if (GetKeyEvent(DIK_D))
		testcannons.LoadCannon(2);
}