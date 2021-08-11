#pragma once
#include <queue>

class MouseEvents
{
	friend class Window;

private:
	class Events
	{
	public:
		enum class Type
		{
			LeftPress,
			RightPress,
			LeftRelease,
			RightRelease,
			/*may change to include mouse move*/
			ScrollUp,
			ScrollDown,
			NOEVENT
		};

	public:
		Events(Type type, int XPos, int YPos)
			: type(type)
			, XPos(XPos)
			, YPos(YPos){};
		Type	  type;
		const int XPos;
		const int YPos;
	};

	/*need to do clang-format*/
	/*need to do think of a normalized wheel delta*/
	void OnMouseMove(int X, int Y) noexcept;
	void OnLeftPress(int X, int Y) noexcept;
	void OnRightPress(int X, int Y) noexcept;
	void OnLeftRelease(int X, int Y) noexcept;
	void OnRightRelease(int X, int Y) noexcept;
	void MouseScrollUp(int X, int Y) noexcept;
	void MouseScrollDown(int X, int Y) noexcept;
	void TrimBuffer(std::queue<Events>& Ref) noexcept;

public:
	MouseEvents() noexcept = default;
	bool   LeftDown		   = false;
	bool   RightDown	   = false;
	int	   XPos			   = 0;
	int	   YPos			   = 0;
	Events GetEvent() noexcept;

private:
	std::queue<Events>	 EventQueue;
	static constexpr int MAXQUEUESIZE = 16;
};
