#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <algorithm>
#include <cstdint>
#include <string>
#include <iomanip>

using namespace std;

// Output text with color
/*
    \033[30m = Black
    \033[31m = Red
    \033[32m = Green
    \033[33m = Yellow
    \033[34m = Blue
    \033[35m = Magenta
    \033[36m = Cyan
    \033[37m = White
    \033[0m = Reset to default
*/
auto coloredText(const string &text, const string &colorCode) -> string {
    return "\033[" + colorCode + "m" + text + "\033[0m";
}

void printBinary(const vector<uint8_t> &bytes) {
    for (const auto &byte: bytes) {
        bitset<8> b(byte);
        cout << b << " ";
    }
    cout << endl;
}

int main() {
    string filename;
    cout << "Enter file name (only the name with extension): ";
    cin >> filename;

    // Check the correct file name
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << coloredText("Invalid file name!", "31") << endl;
        return 1;
    }
    cout << coloredText("Check: OK", "32") << endl;

    int n;
    cout << "Enter the number of bytes (3-30): ";
    cin >> n;

    // Adjust the value of n
    if (n < 3) { n = 10; } else if (n > 26) { n = 26; }

    vector<uint8_t> bytes(n);
    file.read(reinterpret_cast<char *>(bytes.data()), n);
    if (size_t bytesRead = file.gcount(); bytesRead < n) {
        bytes.resize(bytesRead);
    }

    // Output the array in decimal form
    cout << coloredText("Byte sequence: ", "36");
    for (const uint8_t &byte: bytes) { cout << static_cast<int>(byte) << " "; }
    cout << endl;

    // Output the binary array
    cout << coloredText("Binary sequence(32 - is Space): ", "36");
    printBinary(bytes);

    // Cyclic shift to the right by 3 bits
    vector<uint8_t> shiftedBytes(bytes);
    for (uint8_t &byte: shiftedBytes) { byte = (byte >> 3) | (byte << (8 - 3)); }

    // Finding the minimum byte
    auto minByte = *min_element(shiftedBytes.begin(), shiftedBytes.end());
    auto minCount = count(shiftedBytes.begin(), shiftedBytes.end(), minByte);

    // Output the binary array after the shift
    cout << coloredText("Shift to the right by 3 bits: ", "36");
    for (const auto &byte: shiftedBytes) {
        if (byte == minByte) {
            cout << coloredText(bitset<8>(byte).to_string(), "31") << " ";
        } else {
            cout << bitset<8>(byte) << " ";
        }
    }
    cout << endl;

    // Display the minimum byte in color
    cout << coloredText("Minimum byte: ", "33");
    cout << minCount << " - " << coloredText(static_cast<string>(bitset<8>(minByte).to_string()), "31") << ", that's number: ";
    cout << static_cast<int>(minByte) << endl;

    return 0;
}
