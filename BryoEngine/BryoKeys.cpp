#include "BryoInput.h"

HANDLE hThread;
bool Running;
BryoKeys *bkeys;

DWORD WINAPI BeginKeys(LPVOID dummy);

/*
InitBryoKeys()
Initializes the keys and begins the key detection process.

Parameters
	HWND hWnd				- Handle to the main window.
	unsigned char Keys[]	- Array of keys to monitor.
	int numKeys				- Number of keys found in the array.

Returns 
	true if the initialization was sucessful and false if a problem was encountered.

Code: 
*/
bool BryoKeys::InitBryoKeys(HWND hWnd, unsigned char Keys[], int numKeys)
{
	//TODO: Create error messages for specific errors
	//direct input failed to initialize
	if (FAILED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void **) &din, NULL)))
		return false;
	
	//failed to create a direct input device
	if (FAILED(din->CreateDevice(GUID_SysKeyboard, &dinkey, NULL)))
		return false;

	//failed to set data format
	if (FAILED(dinkey->SetDataFormat(&c_dfDIKeyboard)))
		return false;
	
	//failed to set cooperative level
	if (FAILED(dinkey->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		return false;
	
	//failed to aquire keyboard
	if (FAILED(dinkey->Acquire()))
		return false;

	//copy keys into the key array
	mKeys = new B_Key[numKeys];
	mNumKeys = numKeys;

	for (int i = 0; i < numKeys; i ++)
	{
		mKeys[i].ID = Keys[i];
		mKeys[i].KeyDown = false;
	}

	//start the key detection thread
	bkeys = this;

	DWORD tid;
	Running = true;
	hThread = CreateThread(NULL, 0, BeginKeys, 0, 0, &tid);

	mLoaded = true;
	return true;
}

/* 
BeginKeys()
Thread function that begins the key detection process

Parameters
	LPVOID dummy	- input into the function. (by default, this should not affect the function so passing a 0 is fine)

Returns
	0 on exit.

Code: 
*/
DWORD WINAPI BeginKeys(LPVOID dummy)
{
	bkeys->RunBryoKeys();
	return 0;
}

/*
CreateKeys()
Helper function used to create an array of unsigned char keys

Parameters
	B_Key Keys		- Array containing keys used
	int bound		- upper bound of the array

Returns 
	an array containing the keystates of the keys.

Code: 
*/
unsigned char *CreateKeys(B_Key* Keys, int bound)
{
	unsigned char *mKeys;
	
	mKeys = (unsigned char *) malloc (sizeof(int) * bound);
	for (int i = 0; i < bound; i ++)
	{
		if (Keys[i].KeyDown)
			mKeys[i] = Keys[i].ID + 1;
		else
			mKeys[i] = 0;
	}

	return mKeys;
}

/*
GetKeyEvent()
Checks to see if a key is pressed. To be used during key events to check if a certain key is pressed.

Parameters
	unsigned char key - Key to monitor. (USE DIRECT INPUT'S DIK FORMAT)

Returns 
	true if the key is down and false if the key is up.

Code: 
*/
bool BryoKeys::GetKeyEvent(unsigned char key)
{
	if (Running)
	{
		for (int i = 0; i < mNumKeys; i ++)
			if (mKeys[i].ID == key)
				if (mKeys[i].KeyDown)
					return true;
				else
					return false;
	}
}

/*
RunBryoKeys()
Called by the thread. Monitors key events and triggers virtual functions.

Parameters
	none

Returns nothing.
Code: */
void BryoKeys::RunBryoKeys()
{
	unsigned char keystates[256];
	//unsigned char *tempkeys;
	while (Running)
	{
		//TODO: put an error message here.
		//couldnt get keystates o_O
		if (FAILED(dinkey->GetDeviceState(sizeof(unsigned char[256]), (LPVOID)keystates)))
			Running = false;
		
		for (int i = 0; i < mNumKeys; i ++)
		{
			if (keystates[mKeys[i].ID] & 0x80)
			{	
				if (mKeys[i].KeyDown != true)
				{
					mKeys[i].KeyDown = true;
					//tempkeys = CreateKeys(mKeys, mNumKeys);
					BryoKeyDown();
				}
				mKeys[i].KeyDown = true;
			}
			else
			{
				if (mKeys[i].KeyDown != false)
				{
					mKeys[i].KeyDown = false;
					//tempkeys = CreateKeys(mKeys, mNumKeys);
					BryoKeyUp();
					BryoKeyPress();
				}
				mKeys[i].KeyDown = false;
			}
		}
	}

	//free(tempkeys);
	ShutdownBryoKeys();
}

/*
Misc. virtual functions
(to be redefined when the class is used)
Code: */
void BryoKeys::BryoKeyDown()
{
	//put code here
}

void BryoKeys::BryoKeyUp()
{
	//put code here
}

void BryoKeys::BryoKeyPress()
{
	//put code here
}

/*
Destructor
Fail safe to terminate the keyboard device.
Code: */
BryoKeys::~BryoKeys()
{
	if (mLoaded)
		ShutdownBryoKeys();
}

/*
ShutdownBryoKeys()
Frees memory and releases the direct input device.

Code: 
*/
void BryoKeys::ShutdownBryoKeys()
{
	if (Running)
	{
		Running = false;
		return;
	}
	if (mLoaded)
	{
		if (mKeys)
			delete [] mKeys;

		dinkey->Release();
		din->Release();
	}
}