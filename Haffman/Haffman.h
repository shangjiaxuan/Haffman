#pragma once

#include "Header.h"
#include "Bitwise_Manip.h"

//buffer for storing the intermediate code
extern Byte buffer;

namespace haffman {
	struct node {
#pragma pack(push,2)
		int lchild{ -1 };
		int rchild{ -1 };
		int parent{ -1 };
		size_t count{ 0 };
		size_t weight{ 0 };
#pragma pack(pop)
		void print(std::ostream& ost) const;
	};

#define MAX_CHAR 256
#define NUM_NODE 511
#pragma pack(push,2)
	typedef struct {
		const char magic[8]{ 'H','F',0,0,0x0A,0x0D,0x07,0x08 }; // HF/0/0 LF CR BEL BS
		uint64_t begin{0};
		uint64_t end{0};
		uint64_t flags{0};		//bitwise flags. (indexed from left to right)
								//index 0-2: offset of last char
								//index	3:	 reserved for encryption
								//index 4-15:reserved
		short tree[NUM_NODE*3]{};	//the tree to store
	} HF_Header;
#pragma pack(pop)

	class Haffman {
#define root 510
	public:
		bool encode(const std::string& input_file, const std::string& output_file);
		bool decode(const std::string& input_file, const std::string& output_file);
	private:
		node tree[NUM_NODE]{};
		unsigned short find_smallest(short range) const;
		void init(std::ifstream& ifs);
		void init_tree();
		void print_tree() const;

		int encode(std::istream& input, std::ostream& output) const;
		bool decode(std::istream& input, std::ostream& output, int last_offset, size_t end_pos) const;
		void encode_char(unsigned char ch, std::ostream& output) const;
		void decode_byte(std::ostream & output, int& current, int end_offset) const;
	};
}
