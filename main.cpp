#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <algorithm>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

// Define color codes based on platform
#ifdef _WIN32
const int RED_COLOR = 4;
const int GREEN_COLOR = 2;
const int CYAN_COLOR = 3;
const int YELLOW_COLOR = 6;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#else
const string RED_COLOR = "\033[31m";
const string GREEN_COLOR = "\033[32m";
const string CYAN_COLOR = "\033[36m";
const string YELLOW_COLOR = "\033[33m";
#endif

// Platform-specific function for coloring text
void setColor(const string &colorCode) {
#ifdef _WIN32
    SetConsoleTextAttribute(hConsole, stoi(colorCode));
#else
    cout << colorCode;
#endif
}

void resetColor() {
#ifdef _WIN32
    SetConsoleTextAttribute(hConsole, 7); // Default console color for Windows
#else
    cout << "\033[0m";
#endif
}

// Function to color text (platform-independent)
string color(const string &str, const string &colorCode) {
#ifdef _WIN32
    setColor(to_string(stoi(colorCode)));
    cout << str;
    resetColor();
    return "";
#else
    return colorCode + str + "\033[0m";
#endif
}

// Function to read numbers from a file
vector<int> readNumbersFromFile(ifstream &file, const int n) {
    vector<int> numbers;
    int number;

    while (file >> number && numbers.size() < n) {
        numbers.push_back(number);
    }
    return numbers;
}

string printNumbers(const vector<int> &numbers) {
    string result;
    for (const auto &number: numbers) {
        result += to_string(number) + " ";
    }
    return result;
}

// Function to create a binary array from decimal numbers
vector<int> decimalToBinaryArray(const vector<int> &numbers) {
    vector<int> binaryArray;
    for (const auto &number: numbers) {
        if (number < 0) {
            cout << color("Negative numbers are not supported.", RED_COLOR) << endl;
            continue;
        }
        bitset<8> bits(number);
        for (int i = 0; i < 8; ++i) {
            binaryArray.push_back(bits[i]);
        }
    }
    return binaryArray;
}

// Function to print the binary array with spacing
void printBinaryArray(const vector<int> &binaryArray) {
    for (size_t i = 0; i < binaryArray.size(); i += 8) {
        for (int j = 0; j < 8 && (i + j) < binaryArray.size(); ++j) {
            cout << binaryArray[i + j];
        }
        cout << " ";
    }
    cout << endl;
}

vector<int> rightShiftArray(const vector<int> &binaryArray, int shift) {
    size_t n = binaryArray.size();
    vector<int> shiftedArray(n);
    shift = shift % n;

    for (size_t i = 0; i < n; ++i) {
        shiftedArray[(i + shift) % n] = binaryArray[i];
    }
    return shiftedArray;
}

vector<int> binaryArrayToBytes(const vector<int> &binaryArray) {
    vector<int> bytes;
    for (size_t i = 0; i < binaryArray.size(); i += 8) {
        int byteValue = 0;
        for (int j = 0; j < 8; ++j) {
            if (i + j < binaryArray.size()) {
                byteValue = (byteValue << 1) | binaryArray[i + j];
            }
        }
        bytes.push_back(byteValue);
    }
    return bytes;
}

int main() {
    string filename;
    cout << "Enter the file name: ";
    cin >> filename;

    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        cout << color("Error opening file '" + filename + "'!", RED_COLOR) << endl;
        return 1;
    }

    int n;
    cout << "Enter the number of bytes (1-32): ";
    cin >> n;

    n = max(1, min(n, 32));
    vector<unsigned char> bytes(n);
    file.read(reinterpret_cast<char*>(bytes.data()), n);

    cout << color("Byte sequence: ", CYAN_COLOR);
    for (unsigned char byte : bytes) {
        cout << static_cast<int>(byte) << " ";
    }
    cout << endl;

    vector<int> binaryArray;
    for (unsigned char byte : bytes) {
        for (int i = 7; i >= 0; --i) {
            binaryArray.push_back((byte >> i) & 1);
        }
    }

    cout << color("Binary sequence: ", CYAN_COLOR);
    for (size_t i = 0; i < binaryArray.size(); i += 8) {
        for (int j = 0; j < 8; ++j) {
            cout << binaryArray[i + j];
        }
        cout << " ";
    }
    cout << endl;

    rotate(binaryArray.begin(), binaryArray.begin() + 4, binaryArray.end());

    cout << color("Left shift -> by 4 bits: ", CYAN_COLOR);
    int countReversed = 0;
    for (size_t i = 0; i < binaryArray.size(); i += 8) {
        bool isReversed = binaryArray[i] == 1;
        for (int j = 0; j < 8; ++j) {
            if (isReversed) {
                cout << color(to_string(binaryArray[i + 7 - j]), RED_COLOR);
            } else {
                cout << binaryArray[i + j];
            }
        }
        if (isReversed) {
            countReversed++;
        }
        cout << " ";
    }
    cout << endl;

    cout << color("Number of reversed bytes: ", GREEN_COLOR) << countReversed << endl;

    return 0;
}
