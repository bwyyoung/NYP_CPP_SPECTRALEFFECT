#ifndef FILEINITIALIZERTYPE_H
#define FILEINTIALIZERTYPE_H
#include <String>
#include "BryoEngine\\DirectX9Wrapper.h"
#include "Zip\\unzip.h"

using namespace std;

BryoSprite* LoadSprite(const char* Filename);

class FileInitializerType
{
private:
	LPDIRECT3DDEVICE9 d3dd;
	int mFiles;
	int mCurFile;
	bool mShutDown;
	string* mFileNames;
	BryoSprite* mSpriteDatabase;
public:
	//just a call to shutdown
	BryoSprite* GetSprite (const char* Filename) const;
	FileInitializerType()
	{
		mFileNames = NULL;
		mSpriteDatabase = NULL;
		mShutDown = false;
	}
	~FileInitializerType() { Shutdown(); }
	//begins a thread to load files
	void LoadFiles(char* FileName,LPDIRECT3DDEVICE9);
	void RunLoadFiles();
	//returns a percentage based on the current progress of file loading
	int GetProgress() const;
	//begins dereferencing memory
	void Shutdown()
	{
		if (!mShutDown)
		{
			mShutDown = true;
			if (mFileNames != NULL)
				delete [] mFileNames;
			if (mSpriteDatabase != NULL)
				delete [] mSpriteDatabase;
		}
	}
};
#endif