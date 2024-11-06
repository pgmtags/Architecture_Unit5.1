#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <algorithm>
#include <string>
#include <limits>

using namespace std;

const string RED_COLOR = "\033[31m";
const string GREEN_COLOR = "\033[32m";
const string CYAN_COLOR = "\033[36m";
const string YELLOW_COLOR = "\033[33m";
const string GRAY_COLOR = "\033[90m";

// Function to read numbers from a file
vector<int> readNumbersFromFile(ifstream &file, const int n) {
    vector<int> numbers;
    int number;

    // Read integers from the file
    while (file >> number && numbers.size() < n) {
        numbers.push_back(number);
    }

    return numbers;
}

// Function to print numbers in a colorful way
string color(const string& str, const string& color) {
    return color + str + "\033[0m"; // ANSI escape code to reset the color
}

// Function to check if a number is a space character
bool isSpace(const int number) {
    return number == 32;
}

// Function to convert a decimal number to its binary representation
string decimalToBinary(const int number) {
    // Check if the number is negative
    if (number < 0) {
        cout << color("Negative numbers are not supported.", RED_COLOR) << endl;
        return "";
    }

    // Use 8 bits to represent the number
    return bitset<8>(number).to_string();
}

// Function to print a list of numbers
string printNumbers(const vector<int> &numbers) {
    string result;
    for (const auto &number : numbers) {
        if (isSpace(number)) {
            result += color(to_string(number), GRAY_COLOR) + " ";
        } else {
            result += to_string(number) + " ";
        }
    }
    return result;
}

// Function to print a list of numbers in binary format
string printBinary(const vector<int> &numbers) {
    string result;
    for (const auto &number : numbers) {
        if (isSpace(number)) {
            result += color(decimalToBinary(number), GRAY_COLOR) + " ";
        } else {
            result += decimalToBinary(number) + " "; // Convert each number to binary
        }
    }

    return result;
}

// Function to get the filename from the user
string getFilename() {
    string filename;

    // Loop until the user enters a valid filename
    while (true) {
        cout << "Enter the file name (only name with extension): ";
        cin >> filename;

        // Try to open the file
        if (const ifstream file(filename); file) {
            cout << color("Check: OK", GREEN_COLOR) << endl;
            return filename;
        }

        cout << color("Invalid file name! Please try again.", RED_COLOR) << endl;
    }
}

// Function to get the number of bytes from the user
int getValidatedByteCount() {
    int n;

    while (true) {
        cout << "Enter the number of integers (3-30): ";
        cin >> n;

        // Check if the input is valid and within the range
        if (!cin.fail()) {
            if (n >= 3 && n <= 30) { return n; } else { return 10; }
        }

        // Clear the error flag and ignore the rest of the line if the input is invalid
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int main() {
    // Get the filename from the user
    const string filename = getFilename();

    // Open the file
    ifstream file(filename);

    // Check if the file was successfully opened
    if (!file) {
        cout << color("Error opening file '" + filename + "'!", RED_COLOR) << endl;
        return 1;
    }

    // Get the number of integers from the user
    const int n = getValidatedByteCount();
    cout << "Accept the value of n: " << n << endl;

    // Read the integers from the file
    vector<int> numbers = readNumbersFromFile(file, n);
    file.close();

    // Print the number sequence
    cout << color("Number sequence: ", CYAN_COLOR) << printNumbers(numbers) << endl;

    // Print the binary sequence
    cout << color("Binary sequence: ", CYAN_COLOR) << printBinary(numbers) << endl;

    // Circular right shift by 3 bits for each number
    for (int &number: numbers) {
        number = (number >> 3) | (number << (8 - 3)); // Only considering 8 bits for the shift
    }

    // Find the minimum number
    const int minNumber = *min_element(numbers.begin(), numbers.end());
    const auto minCount = static_cast<int>(count(numbers.begin(), numbers.end(), minNumber));

    // Print the numbers after the shift
    cout << color("After right shift by 3 bits: ", CYAN_COLOR);
    for (const int number: numbers) {
        cout << color(decimalToBinary(number), number == minNumber ? RED_COLOR : "") << " ";
    }
    cout << endl;

    // Display the minimum number in color
    cout << color("Minimum number: ", YELLOW_COLOR) << minCount << " - ";
    cout << color(decimalToBinary(minNumber), RED_COLOR);
    cout << ", it is number: " << minNumber << endl;

    return 0;
}
