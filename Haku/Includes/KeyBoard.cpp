#include "KeyBoard.h"

void KeyBoardEvents::KeyPress(int KeyCode) noexcept
{
	KeyState.set(KeyCode);
}

void KeyBoardEvents::KeyRelease(int KeyCode) noexcept
{
	KeyState.reset(KeyCode);
}

bool KeyBoardEvents::CheckKeyDown(int KeyCode) noexcept
{
	return KeyState[KeyCode];
}
