#include "UI.h"

using namespace std;

int main(int argc, char* argv[]) {
	UI::prompt();
	UI::Init();
	do {
		try {
			UI::main();
		}
		catch (exception& e) {
			cerr << e.what() << '\n' << endl;
		}
	} while (UI::on);
	return 0;
}
