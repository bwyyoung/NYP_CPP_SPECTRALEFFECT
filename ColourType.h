#ifndef COLOURTYPE_H
#define COLOURTYPE_H

//Check out the ShieldType.h file for the tutorial on using the colours
//TODO: Are there enough colors? if there are alot of colours, we have to come up with many designs for each coloured projectile
enum Colours {	//the INTEGRAL values of colours, which are from 0 to 15
	cNULL = 0,
	Red = 1,
	Green = 2,
	Blue = 3,
	Cyan = 4,
	Magenta = 5,
	Yellow = 6,
	White = 13,
	Grey = 14,
	Black = 15
};
class ColourType {
public:
	unsigned char ColourSet[16][3];
	//ColourSet: Buffer to store predefined colours.
	//The first array stores the INTEGRAL value of 16 different colours
	//The second array stores the RGB values of the 16 colours

	//A player chooses 3 of these colours which are:
	//	- the 3 colours of his cannons and
	//	- the 3 colours of his opponent's shield

	ColourType();	//Initializes all the rgb values of the colours 0-15
};
#endif