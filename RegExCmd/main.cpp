#include <iostream>

#include "parse.h"
#include "calculateHitMask.h"

#include "windows.h"
#include <string>
#include <cassert>

void displayMatches(std::string_view text, const regex::HitMask& hitMask) {
  assert(text.size() == hitMask.size());
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	for (size_t i = 0; i < text.size(); ++i) {
		int color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
		if (hitMask[i] > 0)
			color = FOREGROUND_GREEN;
		SetConsoleTextAttribute(hConsole, color);
		std::cout << text[i];
		}
	std::cout << std::endl;
}

int main(int argc, char* argv[]) {
	try {
		const std::string text = "abscdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::cout << text << std::endl;
		while (true) {
			std::string searchString;
			std::getline(std::cin, searchString);
			if (searchString == ":q")
				break;
			const auto parseResult = regex::parse(text, searchString);
			const regex::HitMask hitMask = regex::calculateHitMask(parseResult, text.size());
			std::cout << parseResult.size() << " matches found" << std::endl;
			if (!parseResult.empty()) {
				displayMatches(text, hitMask);
			}
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
		return 1;
	}
	catch (...) {
		std::cerr << "unknown exception" << std::endl;
		return 1;
	}


	return 0;
}