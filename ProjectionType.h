#include "BryoEngine\\DirectX9Wrapper.h"

class ProjectileType
{
private:
	BryoSprite** mProjSprites; //array of sprites
	int mProjFrames;
	int mProjFramesInterval;
	BryoSprite** mExplosionSprites;
	int mExplosionFrames;
	int mExplosionFramesInterval;
	int mCurrentFrame;
	bool mExploding;
public:
	int x;
	int y;
	int yVelocity;
	int xVelocity;
	bool InitProjectile(int x, int y, const char* filename[], int frames, int time, const char* explosion[], int expframes, int exptime);
	void Move(int vx, int vy) { xVelocity = vx; yVelocity = vy; }
	void Render();
	void Explode();
};