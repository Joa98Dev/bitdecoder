#include <string>
#include <bitset>
#include "../includes/TranslatorLogic.h"

// ----------- Text to Binary Translator ---------

std::string TextToBinary(const std::string& text) {
    std::string result;

    // Run each char of the string
    // Use unsigned char to handle the values from 0 to 255
    for (unsigned char c : text) {
	
	// Convert the ASCII value of the char to 8 bits data
        result += std::bitset<8>(c).to_string();

	// Add a space to separate each byte to be legible
        result += ' ';
    }
    return result;
}


//----------- Binary to Text Translator ----------

std::string BinaryToText(const std::string& binary) {
    std::string result;
    std::string clean;

    // Filter the input to keep only with '0's and '1's
    // Removes any space or accidental character
    for (char c : binary)
        if (c == '0' || c == '1')
            clean += c;

    // Iterate through the clean string, jumping 8 bits at a time (one byte)
    // The condition i + 7 < clean.size() ensures there's a full byte left to read
    for (size_t i = 0; i + 7 < clean.size(); i += 8) {

	// Extract a substring of 8 characters
        std::bitset<8> bits(clean.substr(i, 8));

	// Converts the bitsets back into a numerical value
	// Casts that number into its ASCII character
        result += static_cast<char>(bits.to_ulong());
    }

    return result;
}

