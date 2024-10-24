#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>

using namespace std;

// Output text with color
auto coloredText(const string &text, const string &colorCode) -> string {
    return "\033[" + colorCode + "m" + text + "\033[0m";
}

// Check file name format
bool isValidFileName(const string &filename) {
    const filesystem::path filePath(filename);
    // Check if the file has an extension
    return filePath.has_extension() && filePath.is_relative();
}

// Reading bytes from a file
vector<unsigned char> readBytesFromFile(const string &filename, const int n, long &bytesRead) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        cout << coloredText("Error: file not found.", "31") << endl;
        return {};
    }

    vector<unsigned char> bytes(n);
    file.read(reinterpret_cast<char *>(bytes.data()), n);
    bytesRead = file.gcount();
    file.close();

    return bytes;
}

// Outputting bytes in decimal format with grey underline for spaces (ASCII 32)
void printBytes(const vector<unsigned char> &bytes, const long bytesRead) {
    for (int i = 0; i < bytesRead; ++i) {
        if (const int byteValue = static_cast<int>(bytes[i]); byteValue == 32) {
            // If the byte equals 32 (space), output it with a grey underline
            cout << coloredText("__32__", "90") << " "; // "90" — grey color
        } else {
            // Normal byte output
            cout << byteValue << " ";
        }
    }

    cout << endl;
}

// Outputting bytes in binary format
void printBinarySequence(const vector<unsigned char> &bytes, const long bytesRead) {
    cout << coloredText("Binary sequence:", "36") << endl;
    for (int i = 0; i < bytesRead; ++i) {
        const unsigned char byte = bytes[i];
        for (int j = 7; j >= 0; --j) {
            cout << ((byte >> j) & 1);
        }
        cout << " ";
    }
    cout << endl;
}


// Function to cyclically shift the entire array of bytes to the right by 3 bits
void performRightShift(vector<unsigned char> &bytes, const long bytesRead) {
    cout << "Right shift -> by 3 bits (entire array):" << endl;

    // First, save the three lowest bits of the last byte to later insert into the first byte
    const unsigned char carryOver = bytes[static_cast<size_t>(bytesRead) - 1] & 0x07; // 0x07 = 00000111

    // Shift bytes taking into account the transfer of bits between bytes
    for (size_t i = static_cast<size_t>(bytesRead) - 1; i > 0; --i) {
        const unsigned char nextCarry = bytes[i - 1] & 0x07; // Save the three lowest bits of the previous byte
        bytes[i] = (bytes[i] >> 3) | (nextCarry << 5); // Shift the byte and add 3 bits from the previous byte
    }

    // Process the first byte, add three bits from the last byte to the high order bits
    bytes[0] = (bytes[0] >> 3) | (carryOver << 5);
}


// Finding the minimum byte and outputting it
unsigned char findMinByte(const vector<unsigned char> &bytes, const long bytesRead) {
    // Search for the minimum byte only among the actually read bytes
    const unsigned char minByte = *min_element(bytes.begin(), bytes.begin() + bytesRead);

    // Output the minimum byte in binary format
    cout << coloredText("Minimum byte: ", "33");
    for (int j = 7; j >= 0; --j) {
        cout << ((minByte >> j) & 1);
    }
    cout << " " << static_cast<int>(minByte) << endl;

    return minByte;
}


// Counting all minimum bytes
void countMinBytes(const vector<unsigned char> &bytes, const long bytesRead, const unsigned char minByte) {
    int count = 0;
    for (int i = 0; i < bytesRead; ++i) {
        if (bytes[i] == minByte) {
            count++;
        }
    }
    cout << coloredText("Count: ", "33") << count << endl;
}

void processFile(const string &filename, const int n) {
    long bytesRead = 0;
    vector<unsigned char> bytes = readBytesFromFile(filename, n, bytesRead);
    if (bytes.empty()) return;

    printBytes(bytes, bytesRead);
    printBinarySequence(bytes, bytesRead);
    performRightShift(bytes, bytesRead);
    printBytes(bytes, bytesRead);
    printBinarySequence(bytes, bytesRead);
    const unsigned char minByte = findMinByte(bytes, bytesRead);
    countMinBytes(bytes, bytesRead, minByte);
}

int main() {
    string filename;
    cout << "Enter file name (only the name with extension): ";
    cin >> filename;

    int n;
    cout << "Enter the number of bytes (3-30): ";
    cin >> n;

    // Check the number of bytes
    if (n < 3 || n > 30) { n = 10; }

    cout << coloredText("Check: OK", "32") << endl;
    processFile(filename, n);

    return 0;
}
