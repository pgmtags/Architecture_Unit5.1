#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>

using namespace std;

// Function to output text with color
string coloredText(const string& text, const string& colorCode) {
    return "\033[" + colorCode + "m" + text + "\033[0m";
}

// Function to check file name format
bool isValidFileName(const string& filename) {
    const filesystem::path filePath(filename);
    // Check if the file has an extension
    return filePath.has_extension() && filePath.is_relative();
}

// Function to read the file and output an array of bytes
void readBytesFromFile(const string& filename, int n) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        cout << coloredText("Error: file not found.", "31") << endl;
        return;
    }

    vector<unsigned char> bytes;
    bytes.resize(n);
    file.read(reinterpret_cast<char *>(bytes.data()), n);
    long bytesRead = file.gcount();
    file.close();

    // Output array of bytes
    cout << coloredText("Byte sequence:", "34") << endl;
    for (int i = 0; i < bytesRead; ++i) {
        cout << static_cast<int>(bytes[i]) << " ";
    }
    cout << endl;

    // Form binary sequence
    cout << coloredText("Binary sequence:", "36") << endl;
    for (int i = 0; i < bytesRead; ++i) {
        unsigned char byte = bytes[i];
        for (int j = 7; j >= 0; --j) { cout << ((byte >> j) & 1); }
        cout << " ";
    }
    cout << endl;

    // Perform a right circular shift by 3 bits
    cout << coloredText("Right shift -> by 3 bits:", "35") << endl;
    for (int i = 0; i < bytesRead; ++i) {
        unsigned char byte = bytes[i];
        byte = (byte >> 3) | ((byte & 0x07) << 5);
        bytes[i] = byte;
        // Output bytes after the shift in red color
        cout << coloredText("", "31");
        for (int j = 7; j >= 0; --j) { cout << ((byte >> j) & 1); }
        cout << " ";
    }
    cout << endl;

    // Find the minimum byte
    unsigned char minByte = *min_element(bytes.begin(), bytes.begin() + bytesRead);
    cout << coloredText("Minimum byte: ", "33");
    for (int j = 7; j >= 0; --j) {
        cout << ((minByte >> j) & 1);
    }
    cout << " " << static_cast<int>(minByte) << endl;

    // Count the number of minimum bytes
    int count = 0;
    for (int i = 0; i < bytesRead; ++i) {
        if (bytes[i] == minByte) {
            count++;
        }
    }
    cout << coloredText("Count: ", "33") << count << endl;
}

int main() {
    string filename;
    cout << "Enter file name (only the name with extension): ";
    cin >> filename;

    // Check file name
    if (!isValidFileName(filename)) {
        cout << coloredText("Error: file name must include an extension and be relative.", "31") << endl;
        return 1;
    }

    int n;
    cout << "Enter the number of bytes (3-30): ";
    cin >> n;

    // Check the number of bytes
    if (n < 3 || n > 30) { n = 10; }

    cout << coloredText("Check: OK", "32") << endl;
    readBytesFromFile(filename, n);

    return 0;
}
