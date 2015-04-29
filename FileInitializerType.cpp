#include "FileInitializerType.h"
#include "Zip\\unzip.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <stdlib.h>

HANDLE thread;
DWORD WINAPI BeginLoadFiles(LPVOID dummy);
FileInitializerType* gpFileInitializer;
string gFilename;

BryoSprite* LoadSprite(const char *filename)
{
	return gpFileInitializer->GetSprite(filename);
}

BryoSprite* FileInitializerType::GetSprite(const char* filename) const
{
	if (mFileNames == NULL)
	{
		//error detected, return a null value
		return NULL;
	}
	string fname = filename;

	for (int i = 0; i < mFiles; i ++)
	{
		if (mFileNames[i] == fname)
		{
			return &mSpriteDatabase[i];
		}
	}
	
	//sprite with that name was never found =/
	return NULL;
}

int FileInitializerType::GetProgress() const
{
	return (mCurFile * 100)/(mFiles);
}

void FileInitializerType::LoadFiles(char* FileName, LPDIRECT3DDEVICE9 d3ddd)
{
	DWORD tid;

	gFilename = FileName;
	d3dd = d3ddd;
	gpFileInitializer = this;
	CreateThread(NULL, 0, BeginLoadFiles, 0, 0, &tid);
}

DWORD WINAPI BeginLoadFiles(LPVOID dummy)
{
	gpFileInitializer->RunLoadFiles();
	return 0;
}

void FileInitializerType::RunLoadFiles()
{
	unzFile zip = unzOpen (gFilename.c_str());
	unz_global_info info;

	unzGetGlobalInfo (zip, &info);
	mCurFile = 0;
	mFiles = info.number_entry;
	
	//assign memory to the sprite database and the file names
	mFileNames = new string[mFiles];
	mSpriteDatabase = new BryoSprite[mFiles];

	unzGoToFirstFile(zip);
	if (!unzOpenCurrentFile(zip))
	{
		//TODO: Load error display error message and quit
	}
	unz_file_info f_info;
	char * curFileName;
	void *buffer;

	unzGetCurrentFileInfo(zip, &f_info, NULL, 0, NULL, 0, NULL, 0);
	curFileName = (char *) malloc ((f_info.size_filename + 1) * sizeof(char));
	unzGetCurrentFileInfo(zip, NULL, curFileName, f_info.size_filename, NULL, 0, NULL, 0);
	curFileName[f_info.size_filename] = '\0';
	mFileNames[mCurFile] = curFileName;
	free (curFileName);
	buffer = malloc (f_info.uncompressed_size);
	//check if the buffer is out of memory
	if (!buffer)
	{
		//TODO: Add a message box and quit the game if this fails, stating that theres not enough memory to load all graphics.
		MessageBox (NULL, "Error!", "ERROR!!", MB_OK);
	}
	unzReadCurrentFile(zip, buffer, f_info.uncompressed_size);
	mSpriteDatabase[mCurFile].Init(d3dd, buffer, f_info.uncompressed_size);
	mCurFile ++;
	free(buffer);
	
	while (unzGoToNextFile(zip) == UNZ_OK)
	{
		if (!unzOpenCurrentFile(zip))
		{
			//TODO: Load error display error message and quit
		}
		unzGetCurrentFileInfo(zip, &f_info, NULL, 0, NULL, 0, NULL, 0);
		curFileName = (char *) malloc ((f_info.size_filename + 1) * sizeof(char));
		unzGetCurrentFileInfo(zip, NULL, curFileName, f_info.size_filename, NULL, 0, NULL, 0);
		curFileName[f_info.size_filename] = '\0';
		mFileNames[mCurFile] = curFileName;
		free (curFileName);
		buffer = malloc (f_info.uncompressed_size);
		//check if the buffer is out of memory
		if (!buffer)
		{
			//TODO: Add a message box and quit the game if this fails, stating that theres not enough memory to load all graphics.
		}
		unzReadCurrentFile(zip, buffer, f_info.uncompressed_size);
		mSpriteDatabase[mCurFile].Init(d3dd, buffer, f_info.uncompressed_size);
		mCurFile ++;
		free(buffer);
	}
}