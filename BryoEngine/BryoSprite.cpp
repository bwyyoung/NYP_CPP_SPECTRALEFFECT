#include <d3dx9.h>
#include <d3d9.h>
#include "DirectX9Wrapper.h"
#include "..\\Zip\\unzip.h"

//type 1 init from file
bool BryoSprite::Init(LPDIRECT3DDEVICE9 d3ddd, char* fileName)
{
	d3dd = d3ddd;
	d3dd->CreateVertexBuffer (sizeof(TLVERTEX) * 4, NULL, D3DFVF_TLVERTEX, D3DPOOL_MANAGED, &vertexBuffer, NULL);
	//load the texture into the texture holder: tex
	
	//use a green color key
	D3DCOLOR ColorKey = 0xFFFF00FF;
	
	x = y = velx = vely = height = width =  0;
	alpha = 255;

	/*
	HRESULT D3DXCreateTextureFromFileEx(          
		LPDIRECT3DDEVICE9 pDevice,
		LPCTSTR pSrcFile,
		UINT Width,
		UINT Height,
		UINT MipLevels,
		DWORD Usage,
		D3DFORMAT Format,
		D3DPOOL Pool,
		DWORD Filter,
		DWORD MipFilter,
		D3DCOLOR ColorKey,
		D3DXIMAGE_INFO *pSrcInfo,
		PALETTEENTRY *pPalette,
		LPDIRECT3DTEXTURE9 *ppTexture
	);
	*/
	
	
	if (FAILED(
		D3DXCreateTextureFromFileEx (d3dd, fileName, 0, 0, 1, 0, 
		D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, 
		D3DX_DEFAULT, ColorKey, NULL, NULL, &tex)))
	{
		//failed to load the specified texture.
		return false;
	}
	
	
	//D3DXCreateTextureFromFile(d3dd, fileName, &tex);

	//the texture loaded up fine, attempt to get the height and width now
	D3DSURFACE_DESC desc;
	
	if (FAILED(
		tex->GetLevelDesc(0, &desc)))
		return false;

	height = desc.Height;
	width = desc.Width;

	return true;
}


//type 2 load from zip
bool BryoSprite::Init(LPDIRECT3DDEVICE9 d3ddd, char* zipname, char* filename)
{
	d3dd = d3ddd;
	d3dd->CreateVertexBuffer (sizeof(TLVERTEX) * 4, NULL, D3DFVF_TLVERTEX, D3DPOOL_MANAGED, &vertexBuffer, NULL);
	//load the texture into the texture holder: tex

	//use a green color key
	D3DCOLOR ColorKey = 0xFFFF00FF;
	
	x = y = velx = vely = height = width =  0;
	alpha = 255;

	unzFile zip = unzOpen (zipname);
	
	//locate the file in the zip archive
	unzLocateFile (zip, filename, 1);
	

	if (unzOpenCurrentFile(zip) != UNZ_OK)
		return false;

	//opened sucessfully

	unz_file_info info;
	//load the info up
	unzGetCurrentFileInfo(zip, &info, NULL, 0, NULL, 0, NULL, 0);
	
	FileName = new char[info.size_filename];
	unzGetCurrentFileInfo(zip, NULL, FileName, info.size_filename, NULL, 0, NULL, 0);
	
	void *buffer = malloc (info.uncompressed_size);

	if (!buffer)
	{
		//out of memory
		unzCloseCurrentFile(zip);
		unzClose(zip);
		return false;
	}

	unzReadCurrentFile(zip, buffer, info.uncompressed_size );

	unzCloseCurrentFile (zip);
	unzClose(zip);


	if (FAILED(
		D3DXCreateTextureFromFileInMemoryEx (d3dd, buffer, info.uncompressed_size, 0, 0, 1, 0, 
		D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, 
		D3DX_DEFAULT, ColorKey, NULL, NULL, &tex)))
	{
		//failed to load the specified texture.
		free (buffer);
		return false;
	}
	
	free (buffer);
	//D3DXCreateTextureFromFile(d3dd, fileName, &tex);

	//the texture loaded up fine, attempt to get the height and width now
	D3DSURFACE_DESC desc;
	
	if (FAILED(
		tex->GetLevelDesc(0, &desc)))
		return false;

	height = desc.Height;
	width = desc.Width;

	return true;
}

//type 3 initialization (from memory)
bool BryoSprite::Init(LPDIRECT3DDEVICE9 d3ddd, void* fileName, int size)
{
	d3dd = d3ddd;
	d3dd->CreateVertexBuffer (sizeof(TLVERTEX) * 4, NULL, D3DFVF_TLVERTEX, D3DPOOL_MANAGED, &vertexBuffer, NULL);
	//load the texture into the texture holder: tex
	
	//use a green color key
	D3DCOLOR ColorKey = 0xFFFF00FF;
	
	x = y = velx = vely = height = width =  0;
	alpha = 255;

	/*
	HRESULT D3DXCreateTextureFromFileEx(          
		LPDIRECT3DDEVICE9 pDevice,
		LPCTSTR pSrcFile,
		UINT Width,
		UINT Height,
		UINT MipLevels,
		DWORD Usage,
		D3DFORMAT Format,
		D3DPOOL Pool,
		DWORD Filter,
		DWORD MipFilter,
		D3DCOLOR ColorKey,
		D3DXIMAGE_INFO *pSrcInfo,
		PALETTEENTRY *pPalette,
		LPDIRECT3DTEXTURE9 *ppTexture
	);
	*/
	
		if (FAILED( D3DXCreateTextureFromFileInMemoryEx (d3dd, fileName, size, 0, 0, 1, 0, 
		D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, 
		D3DX_DEFAULT, ColorKey, NULL, NULL, &tex)))
		{
		//failed to load the specified texture.
			return false;
		}
	
	//D3DXCreateTextureFromFile(d3dd, fileName, &tex);

	//the texture loaded up fine, attempt to get the height and width now
	D3DSURFACE_DESC desc;
	
	if (FAILED(
		tex->GetLevelDesc(0, &desc)))
		return false;

	height = desc.Height;
	width = desc.Width;

	return true;
}

void BryoSprite::Render()
{
	//do necessary calculations
	x = x + velx;
	y = y + vely;

	//create the quad in the specified region
	TLVERTEX* vertices;

	vertexBuffer->Lock (0, 0, (void **) &vertices, NULL);
	//draw the vertices according to the width and height, by default, unloaded surfaces are textured as black squares
	
	//top left
	vertices[0].color = D3DCOLOR_ARGB(alpha, r, g, b);
	vertices[0].x = (float) x - 0.5f;
	vertices[0].y = (float) y - 0.5f;
	vertices[0].z = 0.0f;
	vertices[0].rhw = 1.0f;
	vertices[0].u = 0.0f;
	vertices[0].v = 0.0f;
	
	//top right
	vertices[1].color = D3DCOLOR_ARGB(alpha, r, g, b);
	vertices[1].x = (float) (x + width) - 0.5f;
	vertices[1].y = (float) y - 0.5f;
	vertices[1].z = 0.0f;
	vertices[1].rhw = 1.0f;
	vertices[1].u = 1.0f;
	vertices[1].v = 0.0f;

	//bottom right
	vertices[2].color = D3DCOLOR_ARGB(alpha, r, g, b);
	vertices[2].x = (float) (x + width) - 0.5f;
	vertices[2].y = (float) (y + height) - 0.5f;
	vertices[2].z = 0.0f;
	vertices[2].rhw = 1.0f;
	vertices[2].u = 1.0f;
	vertices[2].v = 1.0f;

	//bottom left
	vertices[3].color = D3DCOLOR_ARGB(alpha, r, g, b);
	vertices[3].x = (float) x - 0.5f;
	vertices[3].y = (float) (y + height) - 0.5f;
	vertices[3].z = 0.0f;
	vertices[3].rhw = 1.0f;
	vertices[3].u = 0.0f;
	vertices[3].v = 1.0f;

	//unlock the vertex buffer
	vertexBuffer->Unlock();
	d3dd->SetFVF(D3DFVF_TLVERTEX);
	d3dd->SetStreamSource(0, vertexBuffer, 0, sizeof(TLVERTEX));
	d3dd->SetTexture(0, tex);
	d3dd->DrawPrimitive (D3DPT_TRIANGLEFAN, 0, 2);
}

BryoSprite::BryoSprite()
{
	FileName = NULL;
	r = g = b = 255;
	tex = NULL;
}

BryoSprite::~BryoSprite()
{
	//deconstructor
	vertexBuffer->Release();
	if (tex != NULL)
		tex->Release();
	d3dd = NULL;

	if (FileName != NULL)
		delete [] FileName;
}