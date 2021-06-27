#pragma once


class MouseEvents
{
	friend class Window;
public:
	MouseEvents() noexcept = default;
	void OnMouseMove(int X, int Y, bool Left, bool Right) noexcept;
	bool LeftDown = false;
	bool RightDown = false;
	int XPos = 0;
	int YPos = 0;
private:
};

