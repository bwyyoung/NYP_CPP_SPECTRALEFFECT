//refer to PipeType.h for more information
#include "PipeType.h"
#include "FileInitializerType.h"
#include "BryoEngine\\Random.h"
#include <sstream>
#include <stdlib.h>

using namespace std;

int BreakNumber (string&);

/*
//
+------------------------------------------------------------------------------+
| To use/view the code, double click the [...] boxes after the "Code: " label. |
+------------------------------------------------------------------------------+

SetPipeType
Initializes the Pipe where sparks pass through. Must be called before the pipe can function properly.

Parameters
	LPDIRECT3DDEVICE9 d3ddd			- Pointer to the direct3d 9 interface.
	int X							- Denotes the X position of the pipe.
	int Y							- Denotes the Y position of the pipe.
	int Width						- Width of the pipe.
	int MaxSparks					- Maximum number of sparks appearing at one time.
	int numTypeSparks				- Maximum different type of sparks.
	string SparkPaths[]				- Array of paths to the spark sprites found in the database.
	int Speed						- Velocity of the sprites.

Returns
	bool			- True if the process succeeded and false if it did not.

Code: 
*/
bool PipeType::SetPipeType(LPDIRECT3DDEVICE9 d3ddd, int X, int Y, int Width, int MaxSparks, int numTypeSparks, string SparkPaths[], int Speed, const char * pipesprite )
{
	d3dd			= d3ddd;
	x				= X;
	y				= Y;
	mWidth			= Width;
	mNumSparks		= MaxSparks;
	mSparkTypes		= numTypeSparks;
	mSpeed			= Speed;

	if (mNumSparks < 1 || mSparkTypes < 1 || mWidth < 1)
		return false;

	if ((PipeBG = LoadSprite (pipesprite))==NULL)
		return false;

	//reinitialise the sprites
	mSparkSprite = new BryoSprite*[mSparkTypes];
	
	//begin referencing the sprites from the database
	for (int i = 0; i < mSparkTypes; i ++)
	{
		mSparkSprite[i] = LoadSprite (SparkPaths[i].c_str());
		if (mSparkSprite[i] == NULL)
		{
			delete [] mSparkSprite;
			return false;
		}
	}
	
	mSparks = new sSpark[mNumSparks];
	//begin initializing spark structures
	for (int i = 0; i < mNumSparks; i ++)
	{
		mSparks[i].mVelocity = 0;
		mSparks[i].mPosition = -20;
		mSparks[i].type		 = RandomNumber(mSparkTypes);
		mSparks[i].mFalling	 = false;
		stringstream temp;
		temp << i;
		mQueue = mQueue + temp.str() + ' ';
	}

	//initialize falling sprites
	for (int i = 0; i < 9; i ++)
	{
		mCannonSparks[i].mFalling = false;
	}

	//create the clipping rectangle
	mBoundingBox.left		= x;
	mBoundingBox.top		= y;
	mBoundingBox.bottom		= y + 30;
	mBoundingBox.right		= x + mWidth;

	d3dd->SetScissorRect(&mBoundingBox);
	
	mPendingSpeed = 0;

	mLoaded					= true;
	return true;
}

/*
Render()
Moves and draws the sparks. Decides what to render or reset.

Parameters
	This function does not take in any parameters.

Returns
	This function does not return any values.

Code: 
TODO (SOLVED): it may lag, and it that case we might want to use threading to fix the lag (when searching for a new spark). Solution: used a string queue to store unused sparks.
*/
void PipeType::Render()
{
	//problem 5 sparks, get time between sparks if the sparks move at 5 pixels a frame across a width of 50...
	//10 frames to move completely
	//3 sparks must always be out
	//release a spark every 10/3 frames... about 3 frames
	//frames before a new spark is released = [(width of pipe)/(speed of sparks)]/[(number of sparks) - 2]
	static int timepassed = 0;
	int nextSpark;

	int ReleaseTime;
	ReleaseTime = (mWidth/mSpeed)/(mNumSparks - 2);

	if (timepassed >= ReleaseTime)
	{
		if (mPendingSpeed != 0)
		{
			mSpeed = mPendingSpeed;
			mPendingSpeed = 0;
		}
		//its time to make a new spark check if a new spark is available
		nextSpark = BreakNumber (mQueue);
		if (nextSpark != -1)
		{
			//a spark was found!
			//get a new random type
			mSparks[nextSpark].type = RandomNumber(mSparkTypes);
			mSparks[nextSpark].mVelocity = mSpeed;
			timepassed = 0;
		}	
	}

	//render the pipe bg
	PipeBG->SetPosition(x, y);
	PipeBG->Render();

	//move the falling cannon balls
	for (int i = 0; i < 9; i ++)
	{
		if (mCannonSparks[i].mFalling)
		{
			//move it on the x axis...
			if (mCannonSparks[i].mPosition > mCannonSparks[i].XPos)
				mCannonSparks[i].mPosition --;
			else if (mCannonSparks[i].mPosition < mCannonSparks[i].XPos)
				mCannonSparks[i].mPosition ++;

			//now move it on the y axis (check if it can move the total y)
			if (mCannonSparks[i].Y + mCannonSparks[i].mCurrentGrav > mCannonSparks[i].mYLimit)
			{
				mCannonSparks[i].Y = mCannonSparks[i].mYLimit;
			}
			else
				mCannonSparks[i].Y += mCannonSparks[i].mCurrentGrav;

			//add to the gravity
			if (mCannonSparks[i].mCurrentGrav + 1 <= mCannonSparks[i].mMaxGrav)
				mCannonSparks[i].mCurrentGrav ++;

			mSparkSprite[mCannonSparks[i].type]->SetPosition(x + mCannonSparks[i].mPosition, y + 5 + mCannonSparks[i].Y);
			mSparkSprite[mCannonSparks[i].type]->Render();
		}
	}

	//move the balls
	for (int i = 0; i < mNumSparks; i ++)
	{
		//check if any of the balls have moved out of the bounding box
		if (mSparks[i].mPosition >= mWidth)
		{
			//reset the spark
			mSparks[i].mPosition = -20;
			mSparks[i].mVelocity = 0;
			stringstream temp;
			temp << i;
			mQueue = mQueue + temp.str() + " ";
			continue;
		}
		//draw the moving spark
		d3dd->SetRenderState(D3DRS_SCISSORTESTENABLE, true);
		
		mSparkSprite[mSparks[i].type]->SetPosition(x + mSparks[i].mPosition, y + 5);
		mSparkSprite[mSparks[i].type]->Render();

		d3dd->SetRenderState(D3DRS_SCISSORTESTENABLE, false);

		mSparks[i].mPosition += mSparks[i].mVelocity;
	}
	
	timepassed ++;
}

int BreakNumber(string& input)
{
	if (input == "")
		return -1;
	stringstream temp;
	temp << input;

	int num;
	temp >> num;

	input = input.substr(input.find(" ") + 1);

	return num;
}

/*
Shutdown()
Frees memory used by this class.

Parameters
	This function does not take in any parameters.

Returns
	This function does not return any values.

Code: 
*/
void PipeType::Shutdown()
{
	//only shutdown if the class was set up in the first place
	if (mLoaded)
	{
		mLoaded = false;
		if (mSparkSprite)
			delete [] mSparkSprite;
		if (mSparks)
			delete [] mSparks;
		d3dd = NULL;
	}
}

void PipeType::DropSpark(int t, int xPos, int yPos, int maxGrav)
{
	//function to make the spark fall...
	static int currentGrav = 1;

	mSparks[t].mVelocity = 0;
	//search for an empty cannonspark
	for (int i = 0; i < 9; i ++)
	{
		if (mCannonSparks[i].mFalling == false)
		{
			mCannonSparks[i] = mSparks[t];
			mCannonSparks[i].mMaxGrav = maxGrav;
			mCannonSparks[i].mYLimit = yPos;
			mCannonSparks[i].Y = 0;
			mCannonSparks[i].mCurrentGrav = 1;
			mCannonSparks[i].XPos = xPos;
			mCannonSparks[i].mFalling = true;
			break;
		}
	}
	//reset the spark
	mSparks[t].mPosition = -20;

	stringstream temp;
	temp << t;
	mQueue = mQueue + temp.str() + " ";
}