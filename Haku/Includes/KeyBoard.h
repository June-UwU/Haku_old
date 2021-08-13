#pragma once
#include <bitset>
#include <queue>

class KeyBoardEvents
{
private:
	class Events
	{
	public:
		enum class Type
		{
			Keypress,
			Keyrelease,
			NOEVENT
		};
		Events(int KeyCode, Type type)
			: KeyCode(KeyCode)
			, type(type)
		{
		}
		Type type;
		int	 KeyCode;
	};
	void TrimBuffer(std::queue<Events>& Ref) noexcept;
	;
	static constexpr int MAXQUEUESIZE = 16;
	std::bitset<256u>	 KeyState{};
	std::queue<Events>	 EventQueue;

public:
	KeyBoardEvents() = default;
	void   KeyPress(int KeyCode) noexcept;
	void   KeyRelease(int KeyCode) noexcept;
	bool   CheckKeyDown(int KeyCode) noexcept;
	Events GetEvents() noexcept;
};
