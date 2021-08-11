#include "MouseEvents.h"

void MouseEvents::OnMouseMove(int X, int Y) noexcept
{
	if (EventQueue.size() > MAXQUEUESIZE)
	{
		TrimBuffer(EventQueue);
	}
	XPos = X;
	YPos = Y;
}

void MouseEvents::OnLeftPress(int X, int Y) noexcept
{
	if (EventQueue.size() > MAXQUEUESIZE)
	{
		TrimBuffer(EventQueue);
	}
	XPos = X;
	YPos = Y;
	EventQueue.push(Events(Events::Type::LeftPress, X, Y));
}

void MouseEvents::OnRightPress(int X, int Y) noexcept
{
	if (EventQueue.size() > MAXQUEUESIZE)
	{
		TrimBuffer(EventQueue);
	}
	XPos = X;
	YPos = Y;
	EventQueue.push(Events(Events::Type::RightPress, X, Y));
}

void MouseEvents::OnLeftRelease(int X, int Y) noexcept
{
	if (EventQueue.size() > MAXQUEUESIZE)
	{
		TrimBuffer(EventQueue);
	}
	XPos = X;
	YPos = Y;
	EventQueue.push(Events(Events::Type::LeftRelease, X, Y));
}

void MouseEvents::OnRightRelease(int X, int Y) noexcept
{
	if (EventQueue.size() > MAXQUEUESIZE)
	{
		TrimBuffer(EventQueue);
	}
	XPos = X;
	YPos = Y;
	EventQueue.push(Events(Events::Type::RightRelease, X, Y));
}

void MouseEvents::MouseScrollUp(int X, int Y) noexcept
{
	if (EventQueue.size() > MAXQUEUESIZE)
	{
		TrimBuffer(EventQueue);
	}
	XPos = X;
	YPos = Y;
	EventQueue.push(Events(Events::Type::ScrollUp, X, Y));
}

void MouseEvents::MouseScrollDown(int X, int Y) noexcept
{
	if (EventQueue.size() > MAXQUEUESIZE)
	{
		TrimBuffer(EventQueue);
	}
	XPos = X;
	YPos = Y;
	EventQueue.push(Events(Events::Type::ScrollDown, X, Y));
}

void MouseEvents::TrimBuffer(std::queue<Events>& Ref) noexcept
{
	while (Ref.size() > MAXQUEUESIZE)
	{
		Ref.pop();
	}
}

MouseEvents::Events MouseEvents::GetEvent() noexcept
{
	if (EventQueue.size() > 0)
	{
		Events ret = EventQueue.front();
		EventQueue.pop();
		return ret;
	}
	else
	{
		Events ret(MouseEvents::Events::Type::NOEVENT, XPos, YPos);
		EventQueue.pop();
		return ret;
	}
}
