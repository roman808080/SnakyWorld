#pragma once

#include <tuple>

namespace Utils
{
	enum class Directon
	{
		STOP,
		LEFT,
		RIGHT,
		UP,
		DOWN,
		UNDEFINED 
	};

	void setCursorPositon(unsigned int x, unsigned int y);
	void showConsoleCursor(bool showFlag);

	Directon getCurrentDirection();
	void clearConslole();

	std::tuple<int, int, int, int> getScreenCoordinates();
};

