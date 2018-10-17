#include "UI.h"

using namespace std;

int main(int argc, char* argv[]) {
	UI::prompt();
	do {
		try {
			UI::main(argc, argv);
		}
		catch (exception& e) {
			cerr << e.what() << '\n' << endl;
		}
	} while (UI::on);
	return 0;
}
