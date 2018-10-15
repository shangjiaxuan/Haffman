#include "Haffman.h"
#include "UI.h"

using namespace std;

//buffer for storing the intermediate code
Byte buffer{};
//offset of the current read in the byte
int char_offset{0};

namespace haffman {
	void Haffman::init(std::ifstream& ifs) {
		for(auto& i : tree) {
			i = node();
		}
		cout << "Reading file to initialize frequency list...\n";
		char ch;
		ifs.get(ch);
		while(ifs.good()) {
			const unsigned char temp = ch;
			tree[temp].count++;
			ifs.get(ch);
		}
		cout << endl;
		cout << "Done initializing list, initializing Haffman tree...\n";
		init_tree();
		cout << endl;
		cout << "Done!\n";
	}

	void Haffman::init_tree() {
		for(unsigned short added_node = MAX_CHAR; added_node < NUM_NODE; added_node++) {
			unsigned short found = find_smallest(added_node);
			tree[added_node].rchild = found;
			tree[found].parent = added_node;
			found = find_smallest(added_node);
			tree[added_node].lchild = found;
			tree[found].parent = added_node;
			tree[added_node].count = tree[tree[added_node].lchild].count + tree[tree[added_node].rchild].count;
			tree[added_node].weight = tree[tree[added_node].lchild].weight + tree[tree[added_node].rchild].weight
			+ tree[tree[added_node].lchild].count + tree[tree[added_node].rchild].count;
		}
	}

	unsigned short Haffman::find_smallest(short range) const {
		int j = 0;
		while(tree[j].parent >= 0) {
			j++;
		}
		unsigned short rtn=j;
		for (int i = j + 1; i < range; i++) {
			if(tree[i].parent < 0) {
				if(tree[i].weight < tree[rtn].weight) rtn = i;
				else if(!(tree[rtn].weight)) {
					if (tree[i].count < tree[rtn].count) 
						rtn = i;
				}
			}
		}
		return rtn;
	}

	void Haffman::encode(std::istream& input, std::ostream& output) const {
		char c;
		input.get(c);
		char_offset = 0;
		buffer.reset();
		while(input.good()) {
			const unsigned char temp = c;
			encode_char(temp, output);
			input.get(c);
		}
		//write the last few bits from buffer
		output << buffer;
		char_offset = 0;
	}

	void Haffman::decode(std::istream& input, std::ostream& output) const {
		char_offset = 0;
		buffer.reset();
		short current = root;
		input.get(buffer.data);
		//loop for one decoded char or encoded char
		//(one loop when either one happens)
		do {
			//loop for one encoded char
			while(input.good() && char_offset < 8) {
				if(!buffer[char_offset]) {
					char_offset++;
					if(tree[current].lchild < 0) {
						output << char(current);
						current = tree[root].lchild;
						break;
					}
					current = tree[current].lchild;
				} else if(buffer[char_offset]) {
					char_offset++;
					if(tree[current].rchild < 0) {
						output << char(current);
						current = tree[root].rchild;
						break;
					}
					current = tree[current].rchild;
				} else {
					cout << "Haffman::decode(std::istream&, std::ostream&): Unknown signal state!" << endl;
					return;
				}
			}
			//re-get the buffer when full.
			//sets eof bit if gotten the eof bit.
			if(char_offset == 8) {
				input.get(buffer.data);
				char_offset = 0;
			}
		} while(input.good());
	}

	void Haffman::encode_char(unsigned char ch, std::ostream& output) const {
		short current = ch;
		short parent = tree[ch].parent;
		//encode a char into a series of bits.
		bool stack[256]{};
		int filled = 0;
		while(parent >= 0 && filled < 256) {
			if(tree[parent].lchild == current) {
				stack[filled] = false;
				filled++;
				current = parent;
			} else if(tree[parent].rchild == current) {
				stack[filled] = true;
				filled++;
				current = parent;
			} else {
				throw runtime_error(
					"Haffman::decode_char(unsigned char, std::ostream&): parent has no record of child!");
			}
			parent = tree[current].parent;
		}
		//put the encoded bits int buffer and write when full.
		filled--;
		while(filled >= 0) {
			while(char_offset < 8 && (filled >= 0)) {
				if(stack[filled])
					buffer.set_true(char_offset);
				else buffer.set_false(char_offset);
				char_offset++;
				filled--;
			}
			if(char_offset == 8) {
				output << buffer;
				buffer.reset();
				char_offset = 0;
			}
		}
	}

	void Haffman::print_tree() const {
		for (int i = 0; i < NUM_NODE; i++) {
			cout << "Index:\t\t" << i << '\n';
			tree[i].print(cout);
		}
	}

	void node::print(std::ostream& ost) const {
		ost << "Weight:\t\t" << weight << "\n";
		ost << "Parent:\t\t" << parent << "\n";
		ost << "Left child:\t" << lchild << "\n";
		ost << "Right child:\t" << rchild << "\n";
		ost << endl;
	}
}
