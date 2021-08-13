#include "KeyBoard.h"

void KeyBoardEvents::KeyPress(int KeyCode) noexcept
{
	TrimBuffer(EventQueue);
	EventQueue.push(Events(KeyCode, KeyBoardEvents::Events::Type::Keypress));
	KeyState.set(KeyCode);
}

void KeyBoardEvents::KeyRelease(int KeyCode) noexcept
{
	TrimBuffer(EventQueue);
	EventQueue.push(Events(KeyCode, KeyBoardEvents::Events::Type::Keyrelease));
	KeyState.reset(KeyCode);
}

bool KeyBoardEvents::CheckKeyDown(int KeyCode) noexcept
{
	return KeyState[KeyCode];
}

KeyBoardEvents::Events KeyBoardEvents::GetEvents() noexcept
{
	if (EventQueue.size() > 0)
	{
		Events ret = EventQueue.front();
		EventQueue.pop();
		return ret;
	}
	else
	{
		return Events(0, KeyBoardEvents::Events::Type::NOEVENT);
	}
}

void KeyBoardEvents::TrimBuffer(std::queue<Events>& Ref) noexcept
{
	while (Ref.size() > MAXQUEUESIZE)
	{
		Ref.pop();
	}
}
