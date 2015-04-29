#include "TargetType.h"
#include "FileInitializerType.h"
#include "ColourType.h"
#include "Defaults.h"

ColourType MyColours2;
TargetType::TargetType() {
	x = dTargetX;
	y = dTargetY;
	mTargetnHits = dTargetnHits;
	mTargetMaxHits = dTargetMaxHits;

	//TODO: init TargetSpritesNames[]
	for (int i=0;i<3;i++) {	
		mTargetColors[i] = dTargetColors[i];
		mpTargetSprites[i] = LoadSprite(mpTargetSpritesNames[i]);
	}
}
void TargetType::SetCoords(int X, int Y) {
	x = X;
	y = Y;
}
void TargetType::SetTargetnHits(byte hits) {
	mTargetnHits = hits;
}
void TargetType::SetTargetMaxHits(byte maxhits) {
	mTargetMaxHits = maxhits;
}
void TargetType::SetTargetColors(byte col1, byte col2, byte col3) {
	int col[3]; col[0] = col1; col[1] = col2; col[2] = col3;

	for (int i=0;i<3;i++) {
		if (col[i] != cNULL) {
			mTargetColors[i] = col[i];
			mpTargetSprites[i]->SetColor( MyColours2.ColourSet[mTargetColors[i]][0],
										  MyColours2.ColourSet[mTargetColors[i]][1],
										  MyColours2.ColourSet[mTargetColors[i]][2]);
		}
	}
}
void TargetType::IncreaseTargetnHits(byte value) {
	mTargetnHits += value;
}
byte TargetType::GetTargetnHits() {
	return mTargetnHits;
}
void TargetType::RenderTarget() {
	for (int i=0;i<3;i++) {
		mpTargetSprites[i]->Render();
	}
}