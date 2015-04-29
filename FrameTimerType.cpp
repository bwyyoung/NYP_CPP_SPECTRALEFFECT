#include "FrameTimerType.h"
#include "Windows.h"

DWORD TimeStart, TimeEnd, tbf;

void SetFPS(int FPS)
{
	tbf =(DWORD) (1000/(DWORD) FPS);
}

void StartFrame()
{
	TimeStart = GetTickCount();
}

void EndFrame()
{	
	TimeEnd = GetTickCount();

	if (TimeEnd - TimeStart < tbf)
		Sleep(tbf - (TimeEnd - TimeStart));
}