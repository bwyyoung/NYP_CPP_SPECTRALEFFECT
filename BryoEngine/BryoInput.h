#ifndef BRYOINPUT_H
#define BRYOINPUT_H
/*
BryoInput 
Created on 4th November 2006

Interface to the keyboard.              ######################################
										## Created by Bryan Yeo				##
										## SpectralEffect.BrankenOnline.com	##
										######################################
*/

//		Includes				
#include <dinput.h>

/*		BryoKeys{};
Class that initializes and detects input from the keyboard.
Triggers a virtual function when a key is pressed, and checks for key events on a separate thread.

Idea originally conceived by Bryan and Israel.

Guide: BryoKeys (double-click [...] to view): 
----------------------------------------------------
1)	Include BryoInput.h into the header				|
	file of the class you are working on.			|
													|
2)	The class you are working on should				|
	inherit the class BryoKeys.						|
													|
3)	In the Initialization function of the			|
	class, call the function: InitBryoKeys(),		|
	this will automatically start the thread		|
	that takes care of key detection.				|
													|
4)	Redefine the BryoKeyDown(), BryoKeyUp()			|
	and BryoKeyPress() functions.					|
													|
5)	You can use: GetKeyEvent() to check which		|
	keys are up/down when a BryoKey event occurs.	|
													|
6)	Call the function: ShutdownBryoKeys()			|
	In the shutdown function of your class.			|
----------------------------------------------------|
 For a full documentation of the functions, please	|
  refer to BryoKeys.cpp								|
-----------------------------------------------------

Code: */
struct B_Key
{
	unsigned char ID;
	bool KeyDown;
};

class BryoKeys
{
private:
	LPDIRECTINPUT8 din;
	LPDIRECTINPUTDEVICE8 dinkey;
	int mNumKeys;
	B_Key *mKeys;
	bool mLoaded;
public:
	~BryoKeys();

	bool InitBryoKeys(HWND hWnd, unsigned char Keys[], int numKeys);
	//thread that detects key events
	void RunBryoKeys();
	void ShutdownBryoKeys();
	bool GetKeyEvent(unsigned char key);

	void virtual BryoKeyDown();
	void virtual BryoKeyUp();
	void virtual BryoKeyPress();
};
#endif