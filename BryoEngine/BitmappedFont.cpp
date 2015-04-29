#ifndef BitmappedFont_CPP
#define BitmappedFont_CPP

#include <fstream>
#include <sstream>
#include <String>
#include <d3d9.h>
#include <d3dx9.h>
#include "BitmappedFont.h"

using namespace std;

ColourData MakeColour(int r, int g, int b)
{
	ColourData temp;

	temp.r = r;
	temp.b = b;
	temp.g = g;

	return temp;
}

void Font::AddColor (int offset, ColourData color)
{
	ColBuffer *tempcols;
	if (colourub != 0)
	{
		//create a temporary buffer of colours
		tempcols = (ColBuffer *) malloc(colourub * sizeof(ColBuffer));
		for (int i = 0; i < colourub; i ++)
		{
			//start adding the colours from the original struct to this temporary one
			tempcols[i] = colours[i];
		}
		//after this is added change colours into a new array
		delete [] colours;
		colours = new ColBuffer[colourub + 1];
		
		//copy over the saved data
		for (int i = 0; i < colourub; i ++)
		{
			colours[i] = tempcols[i];
			//free as we save it over
		}
		//free the tempcols as there is no further use for it
		free(tempcols);
	}
	else
	{
		//its a fresh colour buffer, just redim with the size of 1
		colours = new ColBuffer[1];
	}

	//add in the info in the position of the new ub
	colours[colourub].Colour = color;
	colours[colourub].Offset = offset;

	colourub ++;
}

void Font::Render()
{
	RECT srcRect;
	D3DXVECTOR2 destPos, Scale, rotCenter;
	D3DXMATRIX trans;
	
	Scale.x = (float)(size)/10 * 0.6;
	Scale.y = (float)(size)/10 * 0.6;
	
	destPos.x = x;
	destPos.y = y;
	
	rotCenter.x = 0;
	rotCenter.y = 0;
	
	TextSprite->Begin(D3DXSPRITE_ALPHABLEND);
	
	float yoffset;
	yoffset = y;

	//calculate the actual y position of the starting text
	if (textHeight > height)
	{
		destPos.y = y - (textHeight - height);
		yoffset = destPos.y;
	}
	
	//start rendering the text here
	static int alpha = 1;
		static bool temp = false;
		 
		if (alpha >= 255)
			temp = true;
		else if (alpha <= 0)
			temp = false;

		if (temp == true)
		{
			//alpha -= 255/(30/2);
			if (alpha < 0)
				alpha = 0;
		}
		else
		{
			alpha += 255/(30/2);
			if (alpha > 255)
				alpha = 255;
		}
	ColourData currentcolor;
	//set the default colour to yellow
	currentcolor = MakeColour(255,255,0);
	int currentcolororder = 0;
	for (int i = 0; i < Text.length(); i ++)
	{
		srcRect.left = CharsetDesc.Chars[Text[i]].x;
		srcRect.top = CharsetDesc.Chars[Text[i]].y;
		srcRect.right = srcRect.left + CharsetDesc.Chars[Text[i]].Width;
		srcRect.bottom = srcRect.top + CharsetDesc.Chars[Text[i]].Height;
		destPos.y = yoffset + CharsetDesc.Chars[Text[i]].YOffset * Scale.y;
		if (colourub != 0)
		{
			if (colours[currentcolororder].Offset == i)
			{
				currentcolor = colours[currentcolororder].Colour;
				currentcolororder ++;
				if (currentcolororder >= colourub)
					currentcolororder = colourub - 1;
			}
		}
		if (y - destPos.y <= 0)
		{
			D3DXMatrixTransformation2D(&trans, &rotCenter, 0.0f, &Scale, &rotCenter, 0.0f, &destPos);
			TextSprite->SetTransform(&trans);
			TextSprite->Draw(tex, &srcRect, NULL, NULL, D3DCOLOR_ARGB(alpha, currentcolor.r, currentcolor.g, currentcolor.b));
		}

		if (Text[i] == ' ')
			destPos.x += 12 * Scale.x;
		else if (Text[i] == '\n')
		{
			yoffset += CharsetDesc.Base * Scale.y + 10 * Scale.y;
			destPos.x = x;
		}
		else
			destPos.x += CharsetDesc.Chars[Text[i]].Width*Scale.x + 1;
	}

	TextSprite->End();
}

void Font::FormatText()
{
	//format the text so that it fits within the width and height

	//position in string of the last word
	int StartWordPos, LastWordPos;

	float curWidth;

	int LineBreaks = 0;
	
	StartWordPos = 0;
	LastWordPos = 0;
	//important! we should always replace colour tags before adding lines into the string! this prevents spacing bugs and other stupid problems
	for (int i = 0; i < Text.length(); i ++)
	{
		if (Text[i] == '[')
		{
			char Color;
			Color = Text[i + 1];

			switch (Color)
			{
				//check what color it was an add this to the color buffer
			case 'r':
				//red colour
				AddColor(i, MakeColour(255,0,0));
				//reformat the string
				Text = Text.substr(0, i) + Text.substr(i + 3);
				break;
			case 'b':
				//red colour
				AddColor(i, MakeColour(0, 0, 255));
				//reformat the string
				Text = Text.substr(0, i) + Text.substr(i + 3);
				break;
			case 'g':
				//red colour
				AddColor(i, MakeColour(0,200,0));
				//reformat the string
				Text = Text.substr(0, i) + Text.substr(i + 3);
				break;
			case '/':
				//the colour formatting is being reset, set the standard colour of yellow
				AddColor(i, MakeColour(255, 255, 0));
				Text = Text.substr(0, i) + Text.substr(i + 4);
			}
		}
	}

	for (int i = 0; i < Text.length(); i ++)
	{
		//check if a space or a new line was detected, also, check for other characters like colour tags
		//do check if its the last word also!
		if (Text[i] == ' ' || i == Text.length() - 1)
		{
			//space was found set this as the end of the last word
			curWidth = 0;
			for (int j = StartWordPos; j < i; j ++)
			{
				//calculate the current width
				if (Text[j] == ' ')
					curWidth += 12 * (float)(size)/10 * 0.6;
				else
					curWidth = curWidth + ((float)(CharsetDesc.Chars[Text[j]].Width) * (float)((float)(size)/10 * 0.6)) + 1;
			}

			//if the current position is more than the width of the text area
			if (curWidth > width)
			{
				if (StartWordPos != LastWordPos)
				{
					//add the new line character at the last known word break
					Text[LastWordPos] = '\n';
					LineBreaks ++;
					StartWordPos = LastWordPos + 1;
				}
				else
				{
					Text[i] = '\n';
					LineBreaks ++;
					StartWordPos = i + 1;
				}
				LastWordPos = StartWordPos;
			}
			else
			{
				LastWordPos = i;
			}
		}
		else if (Text[i] == '\n')
		{
			LineBreaks ++;
			StartWordPos = i + 1;
			LastWordPos = StartWordPos;
		}
	}

	textHeight = (float) (LineBreaks) * (CharsetDesc.Base * ((float)(size)/10 * 0.6) + 10 * ((float)(size)/10 * 0.6));
}

bool Font::Init (LPDIRECT3DDEVICE9 d3ddd, char *fname)
{
	d3dd = d3ddd;
	D3DCOLOR ColorKey = 0xFFFF00FF;

	if (FAILED(
		D3DXCreateTextureFromFileEx (d3dd, fname, 0, 0, 1, 0, 
		D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, 
		D3DX_DEFAULT, ColorKey, NULL, NULL, &tex)))
	{
		//failed to load the specified texture.
		return false;
	}
	
	D3DXCreateSprite(d3dd, &TextSprite);

	x = y = 100;
	size = 29;
	width = 500;
	height = 500;
	return true;
}

bool Font::ReadFontDefinition (char *Fname)
{
	ifstream file;
	string Read, Key, Value, Line;
	size_t i;

	file.open(Fname);

	//start parsing thru the definition
	while (!file.eof())
	{
		//load the line into the line reader
		stringstream LineReader;
		getline(file, Line);
		
		LineReader << Line;

		//read the header of the current line
		LineReader >> Read;

		if (Read == "common")
		{
			//loop til we reach the end of the string
			while (!LineReader.eof())
			{
				LineReader >> Read;
				i = Read.find('=');
				
				Key = Read.substr(0, i);
				Value = Read.substr(i + 1);
				stringstream Convertor;

				Convertor << Value;
				if			(Key == "lineheight")
					Convertor >> CharsetDesc.LineHeight;
				else if		(Key == "base")
					Convertor >> CharsetDesc.Base;
				else if		(Key == "scaleW")
					Convertor >> CharsetDesc.Width;
				else if		(Key == "scaleH")
					Convertor >> CharsetDesc.Height;
				else if		(Key == "pages")
					Convertor >> CharsetDesc.Pages;
			}
		}
		else if (Read == "char")
		{
			unsigned short CharID = 0;
			
			while (!LineReader.eof())
			{
				LineReader >> Read;
				i = Read.find('=');

				Key = Read.substr(0, i);
				Value = Read.substr(i + 1);

				stringstream Convertor;
				Convertor << Value;
				if			(Key == "id")
					Convertor >> CharID;
				else if		(Key == "x")
					Convertor >> CharsetDesc.Chars[CharID].x;
				else if		(Key == "y")
					Convertor >> CharsetDesc.Chars[CharID].y;
				else if		(Key == "width")
					Convertor >> CharsetDesc.Chars[CharID].Width;
				else if		(Key == "height")
					Convertor >> CharsetDesc.Chars[CharID].Height;
				else if		(Key == "xoffset")
					Convertor >> CharsetDesc.Chars[CharID].XOffset;
				else if		(Key == "yoffset")
					Convertor >> CharsetDesc.Chars[CharID].YOffset;
				else if		(Key == "xadvance")
					Convertor >> CharsetDesc.Chars[CharID].XAdvance;
				else if		(Key == "page")
					Convertor >> CharsetDesc.Chars[CharID].Page;
			}
		}
	}

	return true;
}

#endif