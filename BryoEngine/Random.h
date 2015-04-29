#ifndef RANDOM_H
#define RANDOM_H

#include <cstdlib>
#include <ctime>

void InitRandomNumbers();		//initializes the number generator, make sure you only call this once
int RandomNumber(int);			//returns a random number from 0 to int
int RandomNumberEx(int, int);	//specify a minimum and maximum

#endif