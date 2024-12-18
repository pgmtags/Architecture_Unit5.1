#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <algorithm>
#include <string>

using namespace std;

const string RED_COLOR = "\033[31m";
const string GREEN_COLOR = "\033[32m";
const string CYAN_COLOR = "\033[36m";
const string YELLOW_COLOR = "\033[33m";

// Function to read numbers from a file
vector<int> readNumbersFromFile(ifstream &file, const int n) {
    vector<int> numbers;
    int number;

    // Reading integers from the file
    while (file >> number && numbers.size() < n) {
        numbers.push_back(number);
    }

    return numbers;
}

// Function to print numbers with color
string color(const string &str, const string &color) {
    return color + str + "\033[0m"; // ANSI escape code to reset color
}

// Function to print a list of numbers
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
        const int validNumber = abs(number) % 256; // Ensure the number is within 0-255
        // Use 8 bits to represent the number
        bitset<8> bits(validNumber);
        for (int i = 7; i >= 0; --i) {
            // Use reverse order to match standard bit representation
            binaryArray.push_back(bits[i]); // Add each bit to the vector
        }
    }
    return binaryArray;
}

// Function to print the binary array, separating every 8 bits with a space
void printBinaryArray(const vector<int> &binaryArray) {
    for (size_t i = 0; i < binaryArray.size(); i += 8) {
        for (int j = 0; j < 8 && (i + j) < binaryArray.size(); ++j) {
            cout << binaryArray[i + j];
        }
        cout << " "; // Space between bytes
    }
    cout << endl;
}

// Function to right shift the array by 3
vector<int> rightShiftArray(const vector<int> &binaryArray, int shift) {
    const int n = static_cast<int>(binaryArray.size());
    vector<int> shiftedArray(n);
    shift = shift % n; // Handle cases where the shift is greater than the array size

    for (int i = 0; i < n; ++i) {
        shiftedArray[(i + shift) % n] = binaryArray[i];
    }
    return shiftedArray;
}

// Function to convert the binary array back to bytes
vector<int> binaryArrayToBytes(const vector<int> &binaryArray) {
    vector<int> bytes;
    for (size_t i = 0; i < binaryArray.size(); i += 8) {
        int byteValue = 0;
        for (int j = 0; j < 8; ++j) {
            if (i + j < binaryArray.size()) {
                byteValue = (byteValue << 1) | binaryArray[i + j]; // Construct the byte
            }
        }
        bytes.push_back(byteValue);
    }
    return bytes;
}

void printShiftedArrayWithMinByteHighLight(const vector<int> &shiftedArray, const vector<int> &byteArray,
                                           int minByte) {
    cout << color("Right shift -> by 3 bits: ", CYAN_COLOR);
    for (size_t i = 0; i < shiftedArray.size(); i += 8) {
        for (int j = 0; j < 8 && (i + j) < shiftedArray.size(); ++j) {
            if (byteArray[i / 8] == minByte) {
                cout << color(to_string(shiftedArray[i + j]), RED_COLOR); // Highlight the minimum byte
            } else {
                cout << shiftedArray[i + j];
            }
        }
        cout << " "; // Space between bytes
    }
    cout << endl;
}

int main() {
    // Get the file name from the user
    string filename;
    cout << "Enter the file name (just the name with extension): ";
    cin >> filename;

    // Open the file
    ifstream file(filename);
    if (!file) {
        cout << color("Error opening file '" + filename + "'!", RED_COLOR) << endl;
        return 1;
    }

    // Get the number of integers from the user
    int n;
    cout << "Enter the number of bytes (3-30): ";
    cin >> n;

    if (!cin || n < 3 || n > 30) {
        n = 10;
    } else if (n > 26) {
        n = 26;
    }

    // Read integers from the file
    vector<int> numbers = readNumbersFromFile(file, n);
    file.close();

    // Print the original sequence of numbers
    cout << color("Byte sequence: ", CYAN_COLOR) << printNumbers(numbers) << endl;

    // Convert to binary array
    vector<int> binaryArray = decimalToBinaryArray(numbers);

    // Print the binary array
    cout << color("Binary sequence: ", CYAN_COLOR);
    printBinaryArray(binaryArray);

    // Right shift by 3
    vector<int> shiftedArray = rightShiftArray(binaryArray, 3);

    // Convert the binary array to bytes
    vector<int> byteArray = binaryArrayToBytes(shiftedArray);

    // Find the minimum byte in the shifted array
    int minByte = *min_element(byteArray.begin(), byteArray.end());

    // Print the shifted binary array, highlighting the minimum byte
    // Call the new function in main
    printShiftedArrayWithMinByteHighLight(shiftedArray, byteArray, minByte);

    // Highlight the minimum byte in the output
    cout << color("Minimum byte: ", YELLOW_COLOR);
    cout << color(bitset<8>(minByte).to_string(), RED_COLOR) << " " << minByte << endl;

    // Count the number of minimum bytes and its decimal value
    long countMinByte = count(byteArray.begin(), byteArray.end(), minByte);
    cout << color("Count: ", GREEN_COLOR) << countMinByte << endl;

    return 0;
}
