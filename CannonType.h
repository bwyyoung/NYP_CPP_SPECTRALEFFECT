#ifndef CANNONTYPE_H
#define CANNONTYPE_H
#include "PipeType.h"

struct Cannon
{
	int x, y;
	int mCannonWidth, mCannonHeight;
	int Loaded;
};

class CannonType
{
private:
	Cannon mCannons[3];
	BryoSprite* mCannonBG;
	PipeType *mPipe;
public:
	bool InitCannonType (PipeType* pipe, int startingx, int width, int height, int spacing, const char* CannonSprite);
	bool LoadCannon (int CannonNumber);
	void Render();

	friend class PipeType;
};
#endif