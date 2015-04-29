#define DEBUG

#include "DirectX9Wrapper.h"
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "..\\Zip\\unzip.h"

void BryoDX9::ShutDown()
{
	d3dd->Release();
	d3d->Release();
	mShutDown = true;
}

bool BryoDX9::InitDX9(HWND hWnd)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (d3d == NULL)
	{
		//directx not found
		MessageBox(NULL, "DirectX 9 not installed. Please install the latest version of DirectX.", "Alura X Client", MB_OK);
		return false;
	}

	D3DPRESENT_PARAMETERS d3dpp;

	//clear out the d3dpp struct
	ZeroMemory (&d3dpp, sizeof(d3dpp));
	//not windowed. alura X client runs at 1024 by 768. *may put configurations
#ifdef DEBUG
	d3dpp.Windowed = true;
#else
	d3dpp.Windowed = false;
#endif
	//apply double buffering only, no need for triple buffering
	d3dpp.BackBufferCount = 1;

	ScreenWidth = 1024;
	ScreenHeight = 768;
	
	d3dpp.BackBufferHeight = ScreenHeight;
	d3dpp.BackBufferWidth = ScreenWidth;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
#ifdef DEBUG
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
#else
	d3dpp.BackBufferFormat = D3DFMT_R5G6B5;
#endif
	d3dpp.EnableAutoDepthStencil = false;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	d3dpp.Flags = 0;
	
	if (FAILED(d3d->CreateDevice(	D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,
									hWnd,
									D3DCREATE_SOFTWARE_VERTEXPROCESSING,
									&d3dpp,
									&d3dd
								)))
	{
		MessageBox (NULL, "Cannot initialize Direct3d, please check if your computer is DirectX compliant.", "Alura X Client", MB_OK);
		return false;
	}
	
	//set up the d3d device for 2d rendering
	d3dd->SetRenderState(D3DRS_LIGHTING, false);
	//d3dd->SetRenderState(D3DRS_AMBIENT, 255);
	//d3dd->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//d3dd->SetRenderState(D3DRS_ZENABLE, false);
	d3dd->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	d3dd->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	d3dd->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	d3dd->SetRenderState(D3DRS_POINTSPRITEENABLE,true);
	d3dd->SetRenderState(D3DRS_POINTSCALEENABLE,false);

	d3dd->SetTextureStageState (0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	//set up the camera so that it gives us a "2d" view
	D3DXMATRIX Ortho2D;
	D3DXMATRIX Identity;
	
	D3DXMatrixOrthoLH ( &Ortho2D, (float) ScreenWidth, (float) ScreenHeight, 0.0f, 1.0f );
	D3DXMatrixIdentity ( &Identity );
	
	d3dd->SetTransform (D3DTS_PROJECTION, &Ortho2D);
	d3dd->SetTransform (D3DTS_WORLD, &Identity);
	d3dd->SetTransform (D3DTS_VIEW, &Identity);
	
	//set up the device for vertex shading
	d3dd->SetVertexShader (NULL);

	return true;
}

void BryoDX9::BeginRender ()
{
	d3dd->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	d3dd->BeginScene ();
}

void BryoDX9::EndRender ()
{
	d3dd->EndScene();
	d3dd->Present(NULL, NULL, NULL, NULL);
}