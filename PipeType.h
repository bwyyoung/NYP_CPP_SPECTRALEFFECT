#ifndef PIPETYPE_H
#define PIPETYPE_H
/*		Spectral Effect Official Source Code
		PipeType Revision 1								
PipeType class header, makes up the pipe where sparks move through
uses the directX scissor test to properly blit the ball sprites

							##########################################
							## created by: Bryan Yeo				##
							## created on: 28/10/2006				##
							## spectraleffect.brankenonline.com		##
							##########################################
*/

//		Includes										
#include "BryoEngine\\DirectX9Wrapper.h"
#include <String>
#include <d3dx9.h>
#include <d3d9.h>

using namespace std;
//		sSpark											
struct sSpark
{
	int mPosition;			//x position relative to the pipe.
	int type;				//type
	int mVelocity;			//velocity
	bool mFalling;			//the spark is falling
	int mCurrentGrav;
	int mMaxGrav;
	int mYLimit;
	int Y;
	int XPos;
	//TODO: add particle systems so that the spark has trails attached to it
};
//		PipeType										
//class for the pipes where sparks fly in from left to right.
class PipeType
{
//		Private Data Members							
private:
	LPDIRECT3DDEVICE9 d3dd;			//pointer to the d3d interface
	int mNumSparks;					//number of sparks visible at once
	int x, y;						//controls the position of the pipe
	int mWidth;						//width of the pipe
	RECT mBoundingBox;				//boundingbox of the pipe, calculated with the x and y values and the width
	int mSparkTypes;				//number of different type of sparks
	BryoSprite** mSparkSprite;		//pointer to the sprite in the database
	BryoSprite* PipeBG;
	sSpark* mSparks;				//dynamic array of sparks based on mNumSparks
	int mSpeed;						//time taken for a particle to move from 1 side to another. (in frames)
	bool mLoaded;					//variable to check if the class has been loaded
	string mQueue;					//stores the sparks in queue
	int mPendingSpeed;
	sSpark mCannonSparks[9];

	void DropSpark(int TargetSpark, int xPos, int yPos, int maxGrav); //causes a sprite to fall vertically downwards... (as if falling into a cannon)

//		Public Data Members								
public:
	//constructor
	PipeType() { mLoaded = false; }
	//call shutdown as a fail safe.
	~PipeType() { Shutdown(); }
	void SetSpeed(int Speed) { 
		mPendingSpeed = Speed;
		for (int i = 0; i < mNumSparks; i ++)
			if (mSparks[i].mVelocity != 0)
				mSparks[i].mVelocity = Speed;
	}
	int GetSpeed() { return mSpeed; }
	//returns true if succeeded initialization, returns false if it messed up.
	bool SetPipeType ( LPDIRECT3DDEVICE9 d3dd, int X, int Y, int Width, int MaxSparks, int numTypeSparks, string SparkPaths[], int Speed, const char * pipesprite );
	//draws the sparks if visible
	void Render();
	//frees memory
	void Shutdown();	

	friend class CannonType;
};
#endif