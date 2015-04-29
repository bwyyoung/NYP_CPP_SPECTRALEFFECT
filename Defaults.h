#ifndef DEFAULTS_H
#define DEFAULTS_H
//All default datatypes are prefixed with a "d"
//TODO: Add all the defaults

////Target Constants////
	const int dTargetX = 0;
	const int dTargetY = 0;
	const byte dTargetnHits = 0;
	const byte dTargetMaxHits = 3;
	const byte dTargetColors[3] = {0,0,0};
	//static const char * dTargetSpritesNames[3] = ???

	const bool dRenderTarget[3] = {1,1,1};

////Shield Constants////
	const int dShieldX = 0;
	const int dShieldY = 0;
	const byte dShieldLife = 3;
	const byte dShieldColor = 0;
	const byte dShieldColors[3] = {0,0,0};
	const bool dShield_Is_On = true;
	const int dShieldThickness = 1;
	//static const char * dShieldSpriteName = ???


#endif