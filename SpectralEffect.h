//engine includes
#include "BryoEngine\\DirectX9Wrapper.h"
#include "BryoEngine\\BryoInput.h"

//lib includes
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <String>

//SE includes
#include "FileInitializerType.h"
#include "PipeType.h"
#include "CannonType.h"
#include "FrameTimerType.h"

//main game class
class SpectralEffect : public BryoDX9, public BryoKeys
{
private:
	HWND hWnd;
	FileInitializerType test;
	PipeType testpipe;
	CannonType testcannons;
public:
	bool InitSpectralEffect(HWND hWnd);
	void RunGame();
	//initialization screen, initialize and render the progress
	void InitInGame();
	
	void ExitSpectralEffect();
	void BryoKeyDown();
	//deconstructor - removes the instances of directx
	~SpectralEffect() { ExitSpectralEffect(); }
};