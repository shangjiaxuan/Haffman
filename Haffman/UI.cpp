#include "UI.h"

using namespace std;

bool UI::on = true;
haffman::Haffman UI::Haffman;

void UI::main(int argc, char* argv[]) {
	if(argc>3) {
		on = false;
		if(string("encode")==argv[1]) {
			Haffman.encode(argv[2], argv[3]);
			return;
		}
		if (string("decode") == argv[1]) {
			Haffman.decode(argv[2], argv[3]);
			return;
		}
	}
	if(argc>1){
		on = false;
		cout << "Syntax: hfm decode input output" << endl;
		cout << "\thfm encode input output" << endl;
		return;
	}
	cout << '>';
	string arg;
	cin >> arg;
	if(arg=="encode") {
		cin>>arg;
		if(arg[0]=='\"') {
			char c;
			cin.get(c);
			while(c!='\"'&&c!='\n') {
				arg.push_back(c);
				cin.get(c);
			}
		}
		string arg2;
		cin >> arg2;
		if (arg2[0] == '\"') {
			char c;
			cin.get(c);
			while (c != '\"'&&c != '\n') {
				arg2.push_back(c);
				cin.get(c);
			}
		}
		cout << endl;
		if(Haffman.encode(arg, arg2))
			cout << "Done encoding!\n" << endl;
		else cout << "Encoding failed!\n" << endl;
		return;
	}
	if(arg=="decode") {
		cin >> arg;
		if (arg[0] == '\"') {
			char c;
			cin.get(c);
			while (c != '\"'&&c != '\n') {
				arg.push_back(c);
				cin.get(c);
			}
		}
		string arg2;
		cin >> arg2;
		if (arg2[0] == '\"') {
			char c;
			cin.get(c);
			while (c != '\"'&&c != '\n') {
				arg2.push_back(c);
				cin.get(c);
			}
		}
		cout << endl;
		if(Haffman.decode(arg, arg2))
			cout << "Done decoding!\n" << endl;
		else cout << "Decoding failed!\n" << endl;
		return;
	}
	if(arg=="exit") {
		on = false;
		return;
	}
}


