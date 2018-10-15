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
	class Haffman {
#define MAX_CHAR 256
#define NUM_NODE 511
#define root 510
	public:
#pragma pack(push,2)
		node tree[NUM_NODE];
#pragma pack(pop)
		unsigned short find_smallest(short range) const;
		void init(std::ifstream& ifs);
		void init_tree();
		void print_tree() const;

		void encode(std::istream& input, std::ostream& output) const;
		void decode(std::istream& input, std::ostream& output) const;
		void encode_char(unsigned char ch, std::ostream& output) const;
	};
}
