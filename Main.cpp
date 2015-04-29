#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "SpectralEffect.h"

//function prototypes
//api entry point
int APIENTRY WinMain ( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
//msg pump
LRESULT CALLBACK WindowProc ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain ( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//window handle
	SpectralEffect Main;
	HWND hWnd;
	WNDCLASSEX wcex = { sizeof(WNDCLASSEX), CS_OWNDC, WindowProc, 0L, 0L, hInstance, NULL, NULL, NULL, NULL, "SpecFX", NULL };;
	MSG Msg;
	
	//register the class :D (adds this to the processes in task manager i think)
	if (!RegisterClassEx(&wcex))
	{
		//could not register, show error message
		MessageBox (NULL, "Failed to initialize window", "Spectral Effect", MB_OK);
	}

	//show a window for the recently created class
	hWnd = CreateWindow("SpecFX", "Spectral Effect", WS_OVERLAPPEDWINDOW,0, 0, 1024, 768, NULL, NULL, hInstance, NULL);
	
	if (hWnd != NULL)
	{
		//window created successfully
		//start the message pump
		ShowWindow(hWnd, SW_SHOWNORMAL);
		UpdateWindow(hWnd);
		
		//initialize alura
		Main.InitSpectralEffect(hWnd);
		Main.RunGame ();
		
		ZeroMemory(&Msg, sizeof(Msg));
		
		while (Msg.message != WM_QUIT)
		{
			if (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage (&Msg);
				DispatchMessage (&Msg);
			}
		}
	}
	//synchronize threads so that the class is destroyed before the windows
	Main.ExitSpectralEffect();
	while (Main.GetShutDownStatus() == false)
		Sleep(100);
	
	UnregisterClass("SpecFX", hInstance);

	return 0;
}

//message handler
LRESULT CALLBACK WindowProc ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}