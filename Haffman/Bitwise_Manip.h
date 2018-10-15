#pragma once

//offset of the current read in the byte
extern int buffer_offset;

struct Byte {
	char data[8]{ 0,0,0,0,0,0,0,0 };
	bool operator[](const int i) const {
		return data[i >> 3] & (0x80 >> (i & 0x07));
	}
	void set_true(const int index) {
		data[index >> 3] |= (0x80 >> (index & 0x07));
	}
	void set_false(const int index) {
		data[index >> 3] &= (0xFF ^ (0x80 >> (index & 0x07)));
	}
	void reset() {
		for(int i=0; i<8; i++)
		data[i] = 0;
	}
	friend std::ostream&operator<<(std::ostream&ost,const Byte& byte) {
		ost.write(byte.data, 8);
		return  ost;
	}
};



