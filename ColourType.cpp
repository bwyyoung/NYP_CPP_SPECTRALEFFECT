#include "ColourType.h"
ColourType::ColourType() {
	//ColourSet: Buffer to store predefined colours.
	//A player chooses 3 of these colours which are:
	//	- the 3 colours of his cannons and
	//	- the 3 colours of his opponent's shield
	
	ColourSet[Red][0]	= 255;
	ColourSet[Red][1]	= 0;
	ColourSet[Red][2]	= 0;
	ColourSet[Green][0]	= 0;
	ColourSet[Green][1] = 255;
	ColourSet[Green][2] = 0;
	ColourSet[Blue][0]	= 0;
	ColourSet[Blue][1]	= 0;
	ColourSet[Blue][2]	= 255;
	ColourSet[Cyan][0]	= 0;
	ColourSet[Cyan][1]	= 255;
	ColourSet[Cyan][2]	= 255;
	ColourSet[Magenta][0]	= 255;
	ColourSet[Magenta][1]	= 0;
	ColourSet[Magenta][2]	= 255;
	ColourSet[Yellow][0]= 255;
	ColourSet[Yellow][1]= 255;
	ColourSet[Yellow][2]= 0;
	ColourSet[White][0]	= 255;
	ColourSet[White][1]	= 255;
	ColourSet[White][2]	= 255;
	ColourSet[Grey][0]	= 127;
	ColourSet[Grey][1]	= 127;
	ColourSet[Grey][2]	= 127;
	ColourSet[Black][0]	= 0;
	ColourSet[Black][1]	= 0;
	ColourSet[Black][2]	= 0;
}
