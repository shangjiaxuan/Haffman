#include "UI.h"

using namespace std;

int main(int argc, char* argv[]) {
	UI::prompt();
	do {
		try {
			UI::main();
			break;
		}
		catch (exception& e) {
			cerr << e.what() << '\n' << endl;
		}
	} while (UI::on);
	system("pause");
	return 0;
}
