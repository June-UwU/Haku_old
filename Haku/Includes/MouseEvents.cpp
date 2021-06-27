#include "MouseEvents.h"

void MouseEvents::OnMouseMove(int X,int Y,bool Left,bool Right) noexcept
{
	XPos = X;
	YPos = Y;
	LeftDown = Left;
	RightDown = Right;
}
