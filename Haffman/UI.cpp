#include "UI.h"

using namespace std;

bool UI::on = true;
haffman::Haffman UI::Haffman;

void UI::main() {
	auto start = std::chrono::high_resolution_clock::now();
	Haffman.encode("1.jpg", "1.jpg.hfm");
	auto duration = chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start);
	cout << "Encoding file took " << duration.count() << " microseconds" << endl;
	start = std::chrono::high_resolution_clock::now();
	Haffman.decode("1.jpg.hfm","2.jpg");
	duration = chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start);
	cout << "Decoding file took " << duration.count() << " microseconds" << endl;
}


void UI::select() {
	
}

void UI::parse_path(string& path) {
	string temp;
	istringstream iss(path);
	char current;
	iss.get(current);
	while (current == ' ') {
		iss.get(current);
	}
	iss.putback(current);
	while (iss) {
		iss.get(current);
		switch (current) {
		case '\"':
			iss.get(current);
			while (current != '\"') {
				temp.push_back(current);
				iss.get(current);
			}
			break;
//Currently do not support redirection of meaning in the current light weight code.
//To use more advanced features of command parsing, try look at my repo here and 
//file a issue to integrate this into that project.
//https://github.com/shangjiaxuan/Simple-shell
//		case '\\':
//
		default:
			temp.push_back(current);
//			iss.get(current);
		}
	}
	temp.pop_back();
	path = temp;
}

