#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <Windows.h>
#include <sgr.hpp>

#include "Utils.h"

void loop();

int main()
{
	loop();
	return 0;
}

void loop() {
	Utils::clearConslole();
	Utils::showConsoleCursor(false);

	int bottom, left, right, top;
	std::tie(bottom, left, right, top) = Utils::getScreenCoordinates();

	Utils::setCursorPositon(1, 1);
	std::cerr << cpp_sgr::b_green_bg << "  ";
}