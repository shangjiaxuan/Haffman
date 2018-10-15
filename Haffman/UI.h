#pragma once

#include "Header.h"
#include "Haffman.h"

class UI {
public:
	static void prompt() {
		std::cout << "Simple program for Haffman compression and decompression.\n";
		std::cout << "v0.0.1.0\n" << std::endl;
	}
	static void Init();
	static void parse_path(std::string& path);
	static bool on;
	static void main();
	static void select();
	static haffman::Haffman* Instance;
};

