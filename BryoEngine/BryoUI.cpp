#ifndef BryoUI_CPP
#define BryoUI_CPP

#include "BitmappedFont.h"
#include <String>
#include <d3d9.h>
#include <d3dx9.h>

void BryoTextBox::LoadTextureDefinition (LPDIRECT3DDEVICE9 d3ddd, char* path, char* texpath)
{
	d3dd = d3ddd;
	string temp;

	D3DXCreateSprite(d3dd, &sprite);
	
	temp = path;
	TargetFile(temp);

	//start loading coordinates
	leftparam.left		= GetInt("textboxleft", "x1");
	leftparam.top		= GetInt("textboxleft", "y1");
	leftparam.right		= GetInt("textboxleft", "x2");
	leftparam.bottom	= GetInt("textboxleft", "y2");

	midparam.left		= GetInt("textboxmid", "x1");
	midparam.top		= GetInt("textboxmid", "y1");
	midparam.right		= GetInt("textboxmid", "x2");
	midparam.bottom		= GetInt("textboxmid", "y2");

	rightparam.left 	= GetInt("textboxright", "x1");
	rightparam.top		= GetInt("textboxright", "y1");
	rightparam.right	= GetInt("textboxright", "x2");
	rightparam.bottom	= GetInt("textboxright", "y2");

	//now load the texture for textboxes
	D3DCOLOR ColorKey = 0xFFFF00FF;
	D3DXCreateTextureFromFileEx (d3dd, texpath , 0, 0, 1, 0, 
		D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, 
		D3DX_DEFAULT, ColorKey, NULL, NULL, &tex);

	CaretPosition = 0;
	Text = "";
}

void BryoTextBox::Render()
{
	D3DXVECTOR2 scale, mid, rotcen, pos;
	D3DXMATRIX trans;

	scale.y = (float) (1/(float)(leftparam.bottom - leftparam.top)) * (float) (Position.bottom - Position.top);
	scale.x = 1;
	
	rotcen.x = rotcen.y = 0;

	mid.y = (float) (1/(float)(leftparam.bottom - leftparam.top)) * (float) (Position.bottom - Position.top);
	mid.x = (float) (1/(float)(midparam.right - midparam.left)) * (float) (Position.right - Position.left - (leftparam.right - leftparam.left) - (rightparam.right - rightparam.left));

	sprite->Begin(D3DXSPRITE_ALPHABLEND);
	//draw the 3 boxes
	pos.x = Position.left;
	pos.y = Position.top;
	D3DXMatrixTransformation2D(&trans, &rotcen, 0.0f, &scale, &rotcen, 0.0f, &pos);
	sprite->SetTransform(&trans);
	sprite->Draw(tex, &leftparam, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));
	pos.x = Position.left + 10;
	pos.y = Position.top;
	D3DXMatrixTransformation2D(&trans, &rotcen, 0.0f, &mid, &rotcen, 0.0f, &pos);
	sprite->SetTransform(&trans);
	sprite->Draw(tex, &midparam, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));
	pos.x = Position.right - 10;
	pos.y = Position.top;
	D3DXMatrixTransformation2D(&trans, &rotcen, 0.0f, &scale, &rotcen, 0.0f, &pos);
	sprite->SetTransform(&trans);
	sprite->Draw(tex, &rightparam, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));
	sprite->End();
}
#endif BryoUI_CPP