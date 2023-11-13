#include "bitmap.h"

//size_t Size;
//uint8_t* Buffer;
bool Bitmap::operator[](uint64_t index) {
	// get byte the bit is stored in
	uint64_t byteIndex = index / 8;
	uint8_t bitIndex = index % 8;
	uint8_t bitIndexer = 0b10000000 >> bitIndex; 
	if ((Buffer[byteIndex] & bitIndexer) > 0) {
		return true; 
	}
	return false; 
}
void Bitmap::Set(uint64_t index, bool value) {
	uint64_t byteIndex = index / 8;
	uint8_t bitIndex = index % 8;
	uint8_t bitIndexer = 0b10000000 >> bitIndex;
	//bit locking question mark 

	Buffer[byteIndex] &= ~bitIndexer;
	if (value) {
		Buffer[byteIndex] |= bitIndexer; 
	}
}