#ifndef SHIELDTYPE_H
#define SHIELDTYPE_H
#include "BryoEngine\\DirectX9Wrapper.h"

/*		Spectral Effect Official Source Code
		ShieldType Revision 1						

ShieldType manages a player's shield, which is used to defend against coloured projectiles
by matching the projectile's colour

							##########################################
							## created by: Israel Chua				##
							## created on: 7/11/2006				##
							## spectraleffect.brankenonline.com		##
							##########################################

Tutorial: Using Colours

	INTEGRAL COLOUR VALUES & THE SHIELD
	- There are up to 16 default colours which players can choose for their cannons.
	- The colours have different RGB values but to make things easy, every colour is denoted by an integer.
	E.g. Red = 1, Green = 2, etc. (See ColourType.h)

	- Furthermore, every integral value of the colour is enumerated.
	If you type "Red", the compiler will recognize it as the integer 1.

	NOTE: NULL is enumerated as 0. Use NULL if you need to input a colour value without changing its colour.

	- A player may only choose 3.
	- A player's shield colours is the same as his opponent's cannon colours.
	- Thus, the player has 3 shield colours. The integral values of the shield colours are stored
	in an array, mShieldColors[3].

	- In the game, a shield can only display one colour. Thus, mShieldColor stores the shield's current
	colour, with 0 as its first colour, 1 as its second, and 2 as its third.
	- Thus, to access the shield's current INTEGRAL colour, do:
	mShieldColors[mShieldColor]

	Get it?

	ACCESSING THE RGB VALUES OF THE COLOURS
	- The class ColourType stores the default RGB values of the default 16 colours in a 2D unsigned char array,
	named ColourSet[16][3]. 
	- The first array stores the INTEGRAL value of 16 different colours
	- The second array stores the RGB values of the 16 colours. 0 for Red, 1 for Green, 2 for Blue.
	- Thus, to access the Red RGB value of the colour white, do:
	ColourSet[White][0]

	- In this case, we have created a ColourType object called MyColours.
	- To access Blue RGB value of the colour white, do:
	MyColours.ColourSet[White][1]

	If you can get all these, then you should be able to understand the next part.
	- If I want to access the Green RGB value of my shield colour, do:

	MyColours.ColourSet[mShieldColors[mShieldColor]][2]

	Any other questions on using the colours, please come and look for me.

*/
class ShieldType {
private:
	int x,y;						//X and Y Coordinates of the Shield
	byte mShieldLife;				//the shield's hit points
	byte mShieldColor;				//the shield's current colour number. The shield has 3 colours and their colour numbers are 0,1 and 2.
	byte mShieldColors[3];			//an array to store the 3 INTEGRAL values of the shield's 3 colours
	bool mShield_Is_On;				//if the shield is on, mShield_Is_On == true
	int mShieldThickness;			//The thickness of the shield, which would determine the area of...
									//...the shield which it can block projectiles

	//Sprites
	char * mShieldSpriteName;		//Name of the shield sprite file
	BryoSprite * mpShieldSprite;	//pointer to the Shield Sprite in the sprite database
	//TODO: Add AudioFile
public:
	ShieldType();					//Initialize all datatypes to defaults (which hasn't been added)

	//Set member datatypes
	void SetShieldCoords (int x, int y);	//Sets Shield Coordinates (x,y)
	void SetShieldLife(byte Life);			//Sets Shield Life (mShieldLife)
	void SetShieldColor(byte Color);		//Sets Shield Colour (mpShieldColor)
	void SetShield_Is_On(bool Is_On);		//Sets the functional state of the shield (mShield_Is_On)
	void SetShieldSpriteName(char * SpriteName);//Sets Shield Sprite's name
	void SetShieldThickness(int Thickness);	//Sets the thickness of the Shield (mShieldThickness)

	//Modify member datatypes
	void IncreaseShieldLife (byte value);	//Increase the shield life by x amount (mShieldLife)
	void SwitchShield_Is_On ();				//Changes the functional state of the shield (mShield_Is_On)

	//Get member datatypes
	int GetShieldCoords();					//Returns the shield's coordinates
	byte GetShieldLife();					//Returns the shield's life
	byte GetShieldColor();					//Returns the shield's current INTEGRAL colour
	bool GetShield_Is_On();					//Returns the functional state of the shield
	char * GetShieldSpriteName();			//Returns the shield sprite's name
	int GetShieldThickness();				//Returns the thickness of the shield

	//Render
	void RenderShield();				//Render. Duh.

	//Check Projectile Collision
	void CheckCollision();					//Checks if the y-coord of a projectile is the same as that of the shield
};

#endif