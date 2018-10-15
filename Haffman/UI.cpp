#include "UI.h"

using namespace std;

bool UI::on = true;
haffman::Haffman* UI::Instance = nullptr;

void UI::Init() {
	Instance = new haffman::Haffman;
}

void UI::main() {
	
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

