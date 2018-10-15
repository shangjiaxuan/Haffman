#pragma once

//offset of the current read in the byte
extern int buffer_offset;

struct Byte {
	char data{ 0 };
	bool operator[](const int i) const {
		return data & (0x80 >> i);
	}
	operator unsigned char() const {
		return data;
	}
	void set_true(const int index) {
		data |= (0x80 >> index);
	}
	void set_false(const int index) {
		data &= (0xFF ^ (0x80 >>index));
	}
	void reset() {
		data = 0;
	}
	friend std::ostream&operator<<(std::ostream&ost,const Byte& byte) {
		ost << byte.data;
		return  ost;
	}
};



