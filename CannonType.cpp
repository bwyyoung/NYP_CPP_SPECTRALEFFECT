#include "CannonType.h"
#include "FileInitializerType.h"

void CannonType::Render()
{
	for (int i = 0; i < 3; i ++)
	{
		mCannonBG->SetPosition(mCannons[i].x + mPipe->x, mPipe->y + 30);
		mCannonBG->Render();
	}
}

bool CannonType::LoadCannon (int CannonNumber)
{
	//checks if any spark is in the cannon's position.
	for (int i = 0; i < mPipe->mNumSparks; i ++)
	{
		if (mPipe->mSparks[i].mPosition + 20 > mCannons[CannonNumber].x && mPipe->mSparks[i].mPosition + 20 < mCannons[CannonNumber].x + mCannons[CannonNumber].mCannonWidth && mCannons[CannonNumber].Loaded < 3)
		{
			//its within the boundaries of the cannon
			mPipe->DropSpark(i, mCannons[CannonNumber].x, (mCannons[CannonNumber].mCannonHeight/3) * (3 - mCannons[CannonNumber].Loaded), 3);
			mCannons[CannonNumber].Loaded ++;
			return true;
		}
		else if (mPipe->mSparks[i].mPosition < mCannons[CannonNumber].x + mCannons[CannonNumber].mCannonWidth && mPipe->mSparks[i].mPosition > mCannons[CannonNumber].x && mCannons[CannonNumber].Loaded < 3)
		{
			//also within the bounds
			mPipe->DropSpark(i, mCannons[CannonNumber].x, (mCannons[CannonNumber].mCannonHeight/3) * (3 - mCannons[CannonNumber].Loaded), 3);
			mCannons[CannonNumber].Loaded ++;
			return true;
		}
	}

	//no spark within the bounds
	return false;
}


bool CannonType::InitCannonType (PipeType* pipe, int startingx, int width, int height, int spacing, const char * CannonSprite)
{
	mPipe = pipe;

	mCannonBG = LoadSprite(CannonSprite);

	//set the cannons up
	//start with the first cannon
	mCannons[0].x = startingx;
	mCannons[0].y = pipe->y + 30;
	mCannons[0].mCannonWidth = width;
	mCannons[0].mCannonHeight = height;
	mCannons[0].Loaded = 0;

	//setup the second cannon;
	mCannons[1].x = startingx + width + spacing;
	mCannons[1].y = pipe->y + 30;
	mCannons[1].mCannonWidth = width;
	mCannons[1].mCannonHeight = height;
	mCannons[1].Loaded = 0;

	//setup the last cannon;
	mCannons[2].x = startingx + width + spacing + width + spacing;
	mCannons[2].y = pipe->y + 30;
	mCannons[2].mCannonWidth = width;
	mCannons[2].mCannonHeight = height;
	mCannons[2].Loaded = 0;

	return true;
}