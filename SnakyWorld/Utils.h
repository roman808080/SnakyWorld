#pragma once

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

	Directon getCurrentDirection();
	void clearConslole();
};

