#pragma once
#include <bitset>
class KeyBoardEvents
{
public:
	KeyBoardEvents() = default;
	void KeyPress(int KeyCode) noexcept;
	void KeyRelease(int KeyCode) noexcept;
	bool CheckKeyDown(int KeyCode) noexcept;

private:
	std::bitset<256u> KeyState{};
};
