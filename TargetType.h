
#ifndef TARGETTYPE_H
#define TARGETTYPE_H
#include "BryoEngine\\DirectX9Wrapper.h"

/*		Spectral Effect Official Source Code
		TargetType Revision 1						

TargetType manages the Player's Target. The player defends his target using the shield.
If the player's shield is destroyed, his opponent may now attack (or colour) the player's target.
Every time the target gets hit, it will be partially coloured.
Once it has been hit 3 times, it will be completely coloured and the game will end,
pronouncing the opponent as the winner!

							##########################################
							## created by: Israel Chua				##
							## created on: 14/11/2006				##
							## spectraleffect.brankenonline.com		##
							##########################################


Check out the ShieldType.h file for the tutorial on using colours
*/
class TargetType {
private:
	int x,y;							//x and y coordinates of the target sprites
	byte mTargetnHits;
	byte mTargetMaxHits;
	byte mTargetColors[3];

	//Sprites
	bool mRenderTarget[3];
	char * mpTargetSpritesNames[3];
	BryoSprite *mpTargetSprites[3];
public:
	//Set member datatypes
	TargetType();
	void SetCoords (int X, int Y);
	void SetTargetnHits (byte hits);
	void SetTargetMaxHits (byte maxhits);
	void SetTargetColors (byte col1, byte col2, byte col3);

	//Modify member datatypes
	void IncreaseTargetnHits (byte value);
	//Get member datatypes
	byte GetTargetnHits ();

	//Render
	void RenderTarget();
};
#endif
