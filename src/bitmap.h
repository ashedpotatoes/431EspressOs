#pragma once

#include <stddef.h> 
#include <stdint.h>

class Bitmap {
public: 
	size_t Size; 
	uint8_t* Buffer; // where all bits are stores
	bool operator[](uint64_t index); // can use it like a standard away
	void Set(uint64_t index, bool value); 

};