#include <iostream>
#include <thread>
#include <chrono>

#include "Utils.h"

void loop();

int main()
{
    std::cout << "Hello World!\n";
	loop();
}

void loop() {
	Utils::clearConslole();
	while (true) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << static_cast<int>(Utils::getCurrentDirection()) << std::endl;
	}
}