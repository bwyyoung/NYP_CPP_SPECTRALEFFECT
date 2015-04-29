#include <d3d9.h>
#include <d3dx9.h>
#include "DirectX9Wrapper.h"
#include "Random.h"

BryoParticles::BryoParticles()
{
	Particles = NULL;
	points = NULL;
	r = g = b = 255;
	dr = dg = db = 0;
	flamefx = false;
}

BryoParticles::~BryoParticles()
{
	if (Particles != NULL)
		delete [] Particles;
	if (points != NULL)
		delete [] points;
}

bool BryoParticles::Init (LPDIRECT3DDEVICE9 d3ddd, char* fileName, int numpart, 
						  int srcX, int srcY, int offX, int offY, int velxmin, 
						  int velxmax, int velymin, int velymax, int gravity, 
						  int fadetime, int lifetime)
{
	d3dd = d3ddd;

	//create the vertex buffer;

	//create the particles

	Particles = new Particle[numpart];

	points = new BryoSprite[numpart];

	numParticles = numpart;
	
	//set local variables
	x			= srcX;
	y			= srcY;
	offset_X	= offX;
	offset_Y	= offY;
	minx		= velxmin;
	maxx		= velxmax;
	miny		= velymin;
	maxy		= velymax;
	m_gravity	= gravity;
	m_fadetime	= fadetime;
	m_lifetime	= lifetime;

	//initialize texture
	if (FAILED(D3DXCreateTextureFromFile(d3dd, fileName, &tex)))
	{
		delete [] Particles;
		Particles = NULL;
		return false;
	}

	for (int i = 0; i < numpart; i ++)
	{
		InitParticle(i);
		points[i].Init(d3dd, fileName);
	}
	//send the particles through 1 cycle

	for (int i = 0; i < numpart; i ++)
	{
		for (int j = 0; j < m_lifetime; j++)
			UpdateParticle(i);
	}
	return true;
}

void BryoParticles::InitParticle(int index)
{
	//initialize the particle
	Particles[index].alpha = 255;
	Particles[index].fadetime = m_fadetime;
	Particles[index].gravity = m_gravity;
	Particles[index].lifetime = m_lifetime/2 + RandomNumber(m_lifetime/2);
	Particles[index].velx = RandomNumberEx(minx, maxx);
	Particles[index].vely = RandomNumberEx(miny, maxy);
	Particles[index].x = x - (offset_X / 2) + RandomNumber(offset_X);
	Particles[index].y = y - (offset_Y / 2) + RandomNumber(offset_Y);
	Particles[index].r = r;
	Particles[index].g = g;
	Particles[index].b = b;
	Particles[index].dr = dr;
	Particles[index].dg = dg;
	Particles[index].db = db;
}

void BryoParticles::UpdateParticle(int index)
{
	//move the particle
	//update the y velocity
	Particles[index].vely = Particles[index].vely + m_gravity;

	//update the particle's x and y positions
	Particles[index].x += Particles[index].velx;
	Particles[index].y += Particles[index].vely;

	Particles[index].lifetime --;
	
	//change the color based on the dr, dg, and db
	if (Particles[index].r - Particles[index].dr >= 0 && Particles[index].r - Particles[index].dr <= 255)
		Particles[index].r -= Particles[index].dr;

	if (Particles[index].g - Particles[index].dg >= 0 && Particles[index].g - Particles[index].dg <= 255)
		Particles[index].g -= Particles[index].dg;

	if (Particles[index].b - Particles[index].db >= 0 && Particles[index].b - Particles[index].db <= 255)
		Particles[index].b -= Particles[index].db;

	if (Particles[index].lifetime == 0)
	{
		//reset the particle
		InitParticle(index);
	}

	else if (Particles[index].lifetime <= Particles[index].fadetime)
	{
		Particles[index].alpha = Particles[index].alpha - 255 / Particles[index].fadetime;
	}
}

void BryoParticles::Render ()
{
	//update all the particles
	if (flamefx)
		d3dd->SetRenderState(D3DRS_DESTBLEND, ::D3DBLEND_ONE);
	for (int i = 0; i < numParticles; i ++)
	{
		UpdateParticle(i);
		points[i].SetPosition (Particles[i].x, Particles[i].y);
		points[i].SetAlpha (Particles[i].alpha );
		points[i].SetColor (Particles[i].r, Particles[i].g, Particles[i].b);
		points[i].Render();
	}
	if (flamefx)
		d3dd->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//start rendering the particles
}