#pragma once

#include "Header.h"
#include "Bitwise_Manip.h"

//buffer for storing the intermediate code
extern Byte buffer;

namespace haffman {
	typedef struct {
#pragma pack(push,2)
		int lchild{-1};
		int rchild{-1};
		int parent{-1};
		size_t count{0};
		size_t weight{0};
#pragma pack(pop)
	} node;

#define MAX_CHAR 256
#define NUM_NODE 511
#pragma pack(push,2)
	typedef struct {
		const char magic[8]{'H', 'F', 0, 0, 0x0A, 0x0D, 0x07, 0x08}; // HF/0/0 LF CR BEL BS
		uint64_t begin{0};
		uint64_t end{0};
		uint64_t flags{0}; //bitwise flags. (indexed from left to right)
		//index 0-5: offset of last char in buffer
		//index	6:	 reserved for encryption
		//index 7-15:reserved
		short tree[NUM_NODE * 3]{}; //the tree to store
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

		int encode(std::istream& input, std::ostream& output) const;
		bool decode(std::istream& input, std::ostream& output, int last_offset, size_t end_pos) const;
		void encode_buffer(Byte temp, std::ostream& output) const;
		void decode_buffer(std::ostream& output, int& current, int end_offset) const;
	};
}
