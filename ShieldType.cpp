#include "ShieldType.h"
#include "FileInitializerType.h" 
#include "ColourType.h"
#include "Defaults.h"

ColourType MyColours;
ShieldType::ShieldType() {
	x = dShieldX;
	y = dShieldY;
	mShieldLife = dShieldLife;
	mShieldColor = dShieldColor;
	for (int i=0;i<3;i++)
		mShieldColors[i] = dShieldColors[i];

	mShield_Is_On = dShield_Is_On;
	mShieldThickness = dShieldThickness;
	//TODO: init mShieldSpriteName
	mpShieldSprite = LoadSprite (mShieldSpriteName);
	mpShieldSprite->SetPosition(x,y);

}
void ShieldType::SetShieldCoords(int x, int y) {
	ShieldType::x = x;
	ShieldType::y = y;
}
void ShieldType::SetShieldLife(byte Life) {
	mShieldLife = Life;
}

void ShieldType::SetShieldColor(byte Color) {
	mShieldColor = Color;
	mpShieldSprite->SetColor(MyColours.ColourSet[mShieldColors[mShieldColor]][0],
							MyColours.ColourSet[mShieldColors[mShieldColor]][1],
							MyColours.ColourSet[mShieldColors[mShieldColor]][2]);
}
void ShieldType::SetShield_Is_On(bool Is_On) {
	mShield_Is_On = Is_On;
}
void ShieldType::SetShieldSpriteName (char *SpriteName) {
	mShieldSpriteName = SpriteName;
}
void ShieldType::SetShieldThickness(int Thickness) {
	mShieldThickness = Thickness;
}
void ShieldType::IncreaseShieldLife(byte value) {
	mShieldLife += value;
}
void ShieldType::SwitchShield_Is_On() {
	mShield_Is_On = !mShield_Is_On;
}
byte ShieldType::GetShieldLife() {
	return mShieldLife;
}
byte ShieldType::GetShieldColor() {
	return mShieldColor;
}
bool ShieldType::GetShield_Is_On() {
	return mShield_Is_On;
}
char * ShieldType::GetShieldSpriteName() {
	return mShieldSpriteName;
} 

//Render
void ShieldType::RenderShield() {	
	mpShieldSprite->Render();
}
void ShieldType::CheckCollision() {//TODO: Collsion Checking 
	//if (mProjectiles.y == y)
}
