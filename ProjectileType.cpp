#include "ProjectionType.h"
#include "FileInitializerType.h"

bool ProjectileType::InitProjectile(int x, int y, const char *filename[], int frames, int time, const char* explosion[], int expframes, int exptime)
{
	ProjectileType::x = x;
	ProjectileType::y = y;

	mProjSprites = new BryoSprite * [frames];
	for (int i = 0; i < frames; i ++)
	{
		mProjSprites[i] = LoadSprite (filename[i]);
		if (mProjSprites[i] == NULL)
			return false;
	}
	mProjFrames = frames;

	//calculate the frames between each image assuming the framerate is 30.
	//TODO: Add code to customise framerate
	mProjFramesInterval = ((time * 30)/1000);


	mExplosionSprites = new BryoSprite * [expframes];
	for (int i = 0; i < expframes; i ++)
	{
		mExplosionSprites[i] = LoadSprite (explosion[i]);
		if (mExplosionSprites[i] == NULL)
			return false;
	}
	mExplosionFrames = expframes;

	//TODO: Add code to customise framerate
	mExplosionFramesInterval = ((exptime * 30)/1000);

	mCurrentFrame = 0;
	mExploding = false;

	return true;
}

void ProjectileType::Render()
{
	static int framespast;

	if (mExploding)
	{
		mExplosionSprites[mCurrentFrame]->SetPosition (x, y);
		mExplosionSprites[mCurrentFrame]->Render();

		framespast ++;
		if (framespast > mExplosionFramesInterval)
		{
			framespast = 0;
			mCurrentFrame ++;
		}
		if (mCurrentFrame > mExplosionFrames)
			mCurrentFrame = 0;
	}
	else
	{
		mProjSprites[mCurrentFrame]->SetPosition (x, y);
		mProjSprites[mCurrentFrame]->Render();

		framespast ++;
		if (framespast > mProjFramesInterval)
		{
			framespast = 0;
			mCurrentFrame ++;
		}
		if (mCurrentFrame > mProjFrames)
			mCurrentFrame = 0;
	}

	x = x + xVelocity;
	y = y + yVelocity;
}