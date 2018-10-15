#pragma once

#include <cstdint>

#include "Haffman.h"

#pragma pack(push,2)
typedef struct {
	const char magic[8]{'H','F',0,0,0x0A,0x0D,0x07,0x08}; // HF/0/0 LF CR BEL BS
	uint64_t begin{};
	uint64_t end{};
	uint64_t flags{};		//bitwise flags.
							//index 0-2: offset of last char
							//index	3:	 reserved for encryption
							//index 4-15:reserved
	haffman::node tree[NUM_NODE]{};	//the tree to store
} HF_Header;
#pragma pack(pop)


class FileFormat {
public:
	HF_Header header;
};

