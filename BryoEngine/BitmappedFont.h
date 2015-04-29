#ifndef FONTLOADED
#define FONTLOADED

#include <String>
#include <fstream>
#include <sstream>
#include <d3d9.h>
#include <d3dx9.h>
#include "FileParser.h"

using namespace std;

struct ColourData
{
	int r, g, b;
};

struct ColBuffer
{
	ColourData Colour;
	int Offset;
};

ColourData MakeColour(int r, int g, int b);

class Font
{
private:
	//ubound for the colour buffer
	int colourub;
	void AddColor(int offset, ColourData color);
public:
	struct CharDescriptor
	{
		unsigned short x, y;
		unsigned short Width, Height;
		unsigned short XOffset, YOffset;
		unsigned short XAdvance;
		unsigned short Page;

		CharDescriptor() : x(0), y(0), Width(0), Height(0), XOffset(0), YOffset(0), XAdvance(0), Page(0)
		{ }
	};

	struct Charset
	{
		unsigned short LineHeight;
		unsigned short Base;
		unsigned short Width, Height;
		unsigned short Pages;
		CharDescriptor Chars[256];
	};

	LPDIRECT3DTEXTURE9 tex;
	LPDIRECT3DDEVICE9 d3dd;
	LPD3DXSPRITE TextSprite;
	string Text;
	Charset CharsetDesc;
	int x, y, size, width, height;
	ColBuffer * colours;

	float textHeight;
	
	Font()
	{
		//reset the color to null;
		colours = NULL;
		colourub = 0;
	}

	~Font()
	{
		//free memory used by the color buffer if it was initialized
		if (colours != NULL)
			delete [] colours;
	}

	bool Init(LPDIRECT3DDEVICE9 d3ddd, char* FileName);
	bool ReadFontDefinition (char* FileName);
	void FormatText();
	void Render();
};

//the manager for all UI in alura
class BryoUI
{

};

class BryoTextBox : protected BryoFileParser
{
private:
	Font font;
	bool Selected;
	LPDIRECT3DDEVICE9 d3dd;
	LPDIRECT3DTEXTURE9 tex;
	LPD3DXSPRITE sprite;
	RECT leftparam, midparam, rightparam;
public:
	RECT Position;
	int CaretPosition;
	string Text;
	
	//Function to load up the texture for skins
	void ReadTextureFile();
	void LoadTextureDefinition(LPDIRECT3DDEVICE9 d3dd, char* path, char* texpath);
	void Render();
};
#endif