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

	int Haffman::encode(std::istream& input, std::ostream& output) const {
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
		const int rtn = char_offset;
		char_offset = 0;
		return rtn;
	}

	bool Haffman::decode(std::istream& input, std::ostream& output, int last_offset, size_t end_pos) const {
		char_offset = 0;
		buffer.reset();
		int current = root;
		input.get(buffer.data);
		//loop for one decoded char or encoded char
		//(one loop when either one happens)
		while (input.good() && input.tellg() < end_pos) {
			//loop for one encoded char
			while(input.good() && input.tellg()<end_pos && char_offset < 8) {
				if(buffer[char_offset]) {
					char_offset++;
					if(tree[current].rchild < 0) {
						output << char(current);
						current = tree[root].rchild;
						break;
					}
					current = tree[current].rchild;
				} else {
					char_offset++;
					if (tree[current].lchild < 0) {
						output << char(current);
						current = tree[root].lchild;
						break;
					}
					current = tree[current].lchild;
				}
			}
			//re-get the buffer when full.
			//sets eof bit if gotten the eof bit.
			if(char_offset == 8) {
				input.get(buffer.data);
				char_offset = 0;
			}
			input.peek();
		}
		//decode the last char to the last bit set by the offset
		while (char_offset <= last_offset) {
			if (buffer[char_offset]) {
				char_offset++;
				if (tree[current].rchild < 0) {
					output << char(current);
					return true;
				}
				current = tree[current].rchild;
			}
			else {
				char_offset++;
				if (tree[current].lchild < 0) {
					output << char(current);
					return true;
				}
				current = tree[current].lchild;
			}
		}
		cout << "Decode does not end with given offset with last char!" << endl;
		return false;
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

	bool Haffman::encode(const std::string& input_file, const std::string& output_file) {
		ifstream ifs;
		ifs.open(input_file,ios::binary);
		if (!ifs) {
			cout << "Failed to open input file!" << endl;
			return false;
		}
		init(ifs);
		ifs.clear();
		ifs.seekg(0);
		HF_Header header;
		for(int i=0; i<NUM_NODE; i++) {
			header.tree[3 * i] = tree[i].lchild;
			header.tree[3 * i + 1] = tree[i].rchild;
			header.tree[3 * i + 2] = tree[i].parent;
		}
		ofstream ofs;
		ofs.open(output_file, ios::binary);
		if (!ofs) {
			cout << "Failed to open output file!" << endl;
			ifs.close();
			return false;
		}
		ofs.write((char*)(&header), sizeof(HF_Header));
		header.begin = ofs.tellp();
		header.flags|=((encode(ifs, ofs))<<5);
		header.end = ofs.tellp();
		ofs.seekp(0);
		ofs.write((char*)&header, sizeof(HF_Header));
		ofs.seekp(header.end);
		ifs.close();
		ofs.close();
		return true;
	}

	bool Haffman::decode(const std::string& input_file, const std::string& output_file) {
		ifstream ifs;
		ifs.open(input_file, ios::binary);
		if (!ifs) {
			cout << "Failed to open input file!" << endl;
			return false;
		}
		ofstream ofs;
		ofs.open(output_file, ios::binary);
		if (!ofs) {
			cout << "Failed to open output file!" << endl;
			ifs.close();
			return false;
		}
		HF_Header header;
		char standard[8];
		for (int i = 0; i < 8; i++) { standard[i] = header.magic[i]; }
		ifs.read((char*)(&header), sizeof(HF_Header));
		for(int i=0; i<8; i++) {
			if(header.magic[i]!=standard[i]) return false;
		}
		for(int i=0; i<NUM_NODE; i++) {
			tree[i].lchild = header.tree[3 * i];
			tree[i].rchild = header.tree[3 * i + 1];
			tree[i].parent = header.tree[3 * i + 2];
		}
		ifs.seekg(header.begin);
		return decode(ifs, ofs, 0x07&(header.flags>>5),header.end);
	}
}
