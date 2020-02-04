/**
 * @file BerInteger.cpp
 * @brief Class to hold an Integer
 */

// Includes C/C++
#include <stdexcept>

// Own includes
#include "asn1/BerInteger.h"

namespace NetMan {

/**
 * @brief Constructor for a Integer field
 * @param value Value to be stored
 * @param tagOptions Specific tag scope (optional)
 * @param tag Tag value (optional)
 */
BerInteger::BerInteger(s32 value, u8 tagOptions, u32 tag) : BerField(tagOptions, tag){

	this->value = value;

	u8 length = 1;													// 8 bits by default
    for(u8 i = 1; i < 4; i++) {
        if(value < INTEGER_MIN(8 * i) || value > INTEGER_MAX(8 * i)) {
            length ++;
        } else {
            i = 4;      // Exit the loop
        }
    }

	this->setLength(length);
}

/**
 * @brief Parse integer data
 * @param out Output buffer
 */
void BerInteger::parseData(u8 **out) {

	u8 *data = *out;
	u32 len = this->getLength();

    u8 *v = (u8*)&this->value;
	for(u8 i = 0; i < len; i++) {
		data[i] = v[len - i - 1];
	}

	// Advance write pointer
	*out += this->getLength();
}

/**
 * @brief Decode an integer value
 * @param data Input buffer
 * @param len Length of the integer value
 * @return The decoded integer value
 */
s32 BerInteger::decodeIntegerValue(u8 **data, u8 len) {

	u8 *in = *data;
	s32 value = 0;

    if(len > 4) {
        throw std::runtime_error("Invalid length for INTEGER32: " + std::to_string(len));
    }

	switch(len) {
		case 1:
			value = in[0];
			break;
		default:
		{
			u8 *v = (u8*)&value;
			for(u8 i = 0; i < len; i++) {
				v[i] = in[len - i - 1];
			}

			// Fill remaining bytes, if negative
			if(in[0] &(1 << 7)) {
				for(u8 i = len; i < 4; i++) {
					v[i] = 0xFF;
				}
			}
		}
	}

	// Update write pointer
	*data += len;

	// Return decoded value
	return value;
}

/**
 * @brief Decode an Integer
 * @param data Data pointer
 * @return A decoded BerInteger
 */
std::shared_ptr<BerInteger> BerInteger::decode(u8 **data) {

	// Skip tag
	*data += 1;
	
	try {
		// Get length
		u32 len = BerField::decodeLength(data);

		// Return decoded integer
		return std::make_shared<BerInteger>(BerInteger::decodeIntegerValue(data, len));
	} catch (const std::runtime_error &e) {
		throw e;
	}
}

/**
 * @brief Print an Integer
 */
void BerInteger::print() {
	FILE *f = fopen("sdmc:/log.txt", "a+");
	fprintf(f, "INTEGER: %ld\n", this->value);
	fclose(f);
}

/**
 * @brief Get an Integer value
 * @return The integer value
 */
s32 BerInteger::getValue() {
	return this->value;
}

}
