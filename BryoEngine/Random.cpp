#include "Random.h"

void InitRandomNumbers()
{
	srand ((unsigned) time(0));
}

int RandomNumber(int max)
{
	//make sure max > 0
	if (max < 0)
		return 0;

	return (int) ((max * rand()) / (RAND_MAX + 1.0));
}

int RandomNumberEx(int min, int max)
{
	if (max < min)
	{
		return 0;
	}

	int range = max - min;

	return (int)( min + (int) ((range * rand()) / (RAND_MAX + 1.0)));
}