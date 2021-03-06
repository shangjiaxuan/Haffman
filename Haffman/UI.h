#pragma once

#include "Header.h"
#include "Haffman.h"

class UI {
public:
	static void prompt() {
		std::cout << "Simple program for Haffman compression and decompression.\n";
		std::cout << "v0.0.1.0\n" << std::endl;
	}
	static bool on;
	static void main(int argc, char* argv[]);
	static haffman::Haffman Haffman;
};

