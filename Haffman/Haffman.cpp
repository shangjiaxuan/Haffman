#include "Haffman.h"
#include "UI.h"

using namespace std;

//buffer for storing the intermediate code
Byte buffer{};
//offset of the current read in the byte
int buffer_offset{0};

namespace haffman {
	void Haffman::init(std::ifstream& ifs) {
//		for(auto& i : tree) {
//			i = node();
//		}
//		cout << "Reading file to initialize frequency list...\n";
		char ch;
		ifs.get(ch);
		while(ifs.good()) {
			const unsigned char temp = ch;
			tree[temp].count++;
			ifs.get(ch);
		}
//		cout << endl;
//		cout << "Done initializing list, initializing Haffman tree...\n";
		init_tree();
//		cout << endl;
//		cout << "Done!\n";
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
		Byte temp;
		input.read(temp.data, 8);
		buffer_offset = 0;
		buffer.reset();
		while(input.good()) {
			encode_buffer(temp, output);
			input.read(temp.data, 8);
		}
		//write the last few bits from buffer
		output << buffer;
		const int rtn = buffer_offset;
		buffer_offset = 0;
		return rtn;
	}

	//use all the info in the buffer, decode whatever there is.
	//using a separate function here instead of using "break" decrease runtime by 90%
	//(i.e. at the improvement time, from 360 seconds to 36 seconds (now it's just a few seconds,
	//and decreasing steadily))
	void Haffman::decode_buffer(std::ostream& output, int& current, int end_offset) const {
		//loop for one encoded char
		while (buffer_offset <= end_offset) {
			if (buffer[buffer_offset]) {
				buffer_offset++;
				if (tree[current].rchild < 0) {
					output << char(current);
					current = tree[root].rchild;
					return;
				}
				current = tree[current].rchild;
			}
			else {
				buffer_offset++;
				if (tree[current].lchild < 0) {
					output << char(current);
					current = tree[root].lchild;
					return;
				}
				current = tree[current].lchild;
			}
		}
	}

	//input.tellg() takes too much time to execute, only call this once
	bool Haffman::decode(std::istream& input, std::ostream& output, int last_offset, size_t end_pos) const {
		buffer_offset = 0;
		buffer.reset();
		int current = root;
		uint64_t pos = input.tellg();
		input.read(buffer.data,8);
		pos += 8;
		//loop for one decoded char or encoded char
		//(one loop when either one happens)
		while (input.good() && pos <= end_pos) {// && input.tellg() < end_pos) {
			decode_buffer(output, current, 63);
			//re-get the buffer when full.
			//sets eof bit if gotten the eof bit.
			if(buffer_offset == 64) {
				input.read(buffer.data,8);
				pos += 8;
				buffer_offset = 0;
			}
			input.peek();
		}
		//decode the last char to the last bit set by the offset
		decode_buffer(output, current, last_offset);
		return true;
	}

	void Haffman::encode_buffer(Byte temp, std::ostream& output) const {
		for(int i=0; i<8; i++) {
			bool stack[256]{};
			int filled = 0;
			short current = unsigned char(temp.data[i]);
			short parent = tree[unsigned char(temp.data[i])].parent;
			//encode a char into a series of bits.
			while (parent >= 0 && filled < 256) {
				if (tree[parent].lchild == current) {
					stack[filled] = false;
					filled++;
					current = parent;
				}
				else if (tree[parent].rchild == current) {
					stack[filled] = true;
					filled++;
					current = parent;
				}
				else {
					cout << "Haffman::encode_buffer(unsigned char, std::ostream&): parent has no record of child!" << endl;
					return;
				}
				parent = tree[current].parent;
			}
		//put the encoded bits in the buffer and write when full.
			filled--;
			while(filled >= 0) {
				while(buffer_offset < 64 && (filled >= 0)) {
					if(stack[filled])
						buffer.set_true(buffer_offset);
					else buffer.set_false(buffer_offset);
					buffer_offset++;
					filled--;
				}
				if(buffer_offset == 64) {
					output << buffer;
					buffer.reset();
					buffer_offset = 0;
				}
			}
		}
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
		header.flags|=((encode(ifs, ofs))<<2);
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
		return decode(ifs, ofs, 0x3F&(header.flags>>2),header.end);
	}
}
