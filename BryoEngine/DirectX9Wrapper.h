#ifndef DIRECTX9WRAPPER_H
#define DIRECTX9WRAPPER_H
#include <d3d9.h>
#include <d3dx9.h>
#include <windows.h>
#include <String>
#include <iostream>

using namespace std;

const DWORD D3DFVF_TLVERTEX = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
const DWORD D3DFVF_POINTVERTEX = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_PSIZE;

struct TLVERTEX
{
	float x;
	float y;
	float z;
	float rhw;
	D3DCOLOR color;
	float u;
	float v;
};

struct POINTVERTEX
{
	float x;
	float y;
	float z;
	float size;
	D3DCOLOR color;
};

class BryoDX9
{
public:
	int ScreenHeight, ScreenWidth;
	bool mShutDown;
	LPDIRECT3D9 d3d;
	LPDIRECT3DDEVICE9 d3dd;

	//bryan's dx9 wrapper
	bool InitDX9(HWND);
	bool GetShutDownStatus() { return mShutDown; }
	void BeginRender();
	void EndRender();

	void ShutDown();
};

class BryoSprite
{
private:
	int x, y, velx, vely;
	int r,g,b;
	int height, width;
	int alpha;
	char* FileName;
	LPDIRECT3DDEVICE9 d3dd;
	LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
	LPDIRECT3DTEXTURE9 tex;
public:
	//for ordinary file loading. test this first.
	BryoSprite();
	~BryoSprite();

	bool Init(LPDIRECT3DDEVICE9 d3dd, char* File_Name);
	bool Init(LPDIRECT3DDEVICE9 d3dd, void* buffer, int size);
	bool Init(LPDIRECT3DDEVICE9 d3dd, char* Zip_File_Name, char* File_Name);
	void Render();
	void SetPosition (int X_Coordinate, int Y_Coordinate)
	{
		x = X_Coordinate;
		y = Y_Coordinate;
	}
	void SetSize (int Width, int Height)
	{
		width = Width;
		height = Height;
	}
	void Move (int X_Velocity, int Y_Velocity)
	{
		velx = X_Velocity;
		vely = Y_Velocity;
	}
	void SetAlpha (int new_Alpha)
	{
		alpha = new_Alpha;
	}
	void SetColor (int red, int green, int blue)
	{
		r = red;
		g = green;
		b = blue;
	}
};

struct Particle
{
	char* filename;	//the filename of the loaded particle.
	int x;			//current x position
	int y;			//current y position
	int velx;		//moved in the x direction by this much every frame
	int vely;		//moved in the y direction by this much every frame
	int gravity;	//affects how fast vely is increased/decrease
	int lifetime;	//disappears after the lifetime, lifetime decreased on every frame
	int alpha;		//current alpha
	int fadetime;	//uses this to decide how fast the particle fades. only starts fading this amt of time before the lifetime.
	int r;			//rgb used for color
	int g;
	int b;
	int dr, dg, db; //decay for the 3 colours
};

class BryoParticles
{
private: 
	LPDIRECT3DDEVICE9 d3dd;
	int numParticles;
	Particle* Particles;	//create a dynamic array of particles based on numparticles.
	LPDIRECT3DTEXTURE9 tex;
	LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
	BryoSprite* points;

	int x, y, offset_X, offset_Y; //x and y position of the source
	int r, g, b, dr, dg, db; //for color settings
	bool flamefx; //set to true for flame like effects (BLEND_ONE shading)
	int m_gravity, m_fadetime, m_lifetime;

	int minx, maxx, miny, maxy;

	void InitParticle(int);
	void UpdateParticle(int);
public:
	BryoParticles();
	~BryoParticles();
	bool Init (LPDIRECT3DDEVICE9 Direct3D_Device_Reference, char* File_Name, int Number_of_Particles,
		int X, int Y, int X_Distance_From_Source, int Y_Distance_From_Source, int X_Min, int X_Max, 
		int Y_Min, int Y_Max, int Gravity, int FadeTime, int Life);
	void SetColor(int red, int green, int blue, int red_decay, int green_decay, int blue_decay)
	{
		r	= red;
		g	= green;
		b	= blue;
		dr	= red_decay;
		dg	= green_decay;
		db	= blue_decay;
	}
	void SetFlameFX(bool a)
	{
		flamefx = a;
	}
	void Render();
};

class BryoInput
{
	
};
#endif