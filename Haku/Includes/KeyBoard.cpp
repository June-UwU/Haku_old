#include "KeyBoard.h"

void KeyBoardEvents::KeyPress(int KeyCode) noexcept
{
	KeyState[KeyCode] = true;
}

void KeyBoardEvents::KeyRelease(int KeyCode) noexcept
{
	KeyState[KeyCode] = false;
}

bool KeyBoardEvents::CheckKeyDown(int KeyCode) noexcept
{
	return KeyState[KeyCode];
}
