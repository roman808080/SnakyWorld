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
    std::cout << "Hello World!\n";
	loop();

	return 0;
}

void loop() {
	Utils::clearConslole();
	Utils::showConsoleCursor(false);

	int bottom, left, right, top;
	std::tie(bottom, left, right, top) = Utils::getScreenCoordinates();

	std::cerr << cpp_sgr::b_green_bg << "This text has a bright green background";
}