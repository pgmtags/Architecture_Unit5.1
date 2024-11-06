#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <algorithm>
#include <string>
#include <limits>
#include <sstream>

using namespace std;

const string RED_COLOR = "\033[31m";
const string GREEN_COLOR = "\033[32m";
const string CYAN_COLOR = "\033[36m";
const string YELLOW_COLOR = "\033[33m";
const string GRAY_COLOR = "\033[90m";

string getFilename() {
    string filename;
    cout << "Enter the filename: ";
    cin >> filename;
    return filename;
}

int getValidatedByteCount() {
    int n;
    do {
        cout << "Enter the number of bytes (3-32): ";
        cin >> n;
        if (cin.fail() || n < 3 || n > 32) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number between 3 and 32." << endl;
        } else {
            break;
        }
    } while (true);
    return n;
}

vector<int> readNumbersFromFile(ifstream &file, const int n) {
    vector<int> numbers;
    int number;
    while (file >> number && numbers.size() < n) {
        numbers.push_back(number);
    }
    return numbers;
}

string color(const string& str, const string& color) {
    return color + str + "\033[0m";
}

string decimalToBinary(const int number) {
    if (number < 0) {
        cout << color("Negative numbers are not supported.", RED_COLOR) << endl;
        return "";
    }
    return bitset<8>(number).to_string();
}

string outputNumbers(const vector<int> &numbers) {
    string result;
    for (const auto &number : numbers) {
        result += to_string(number) + " ";
    }
    return result;
}

string outputBinary(const vector<int> &numbers) {
    string result;
    for (const auto &number : numbers) {
        result += decimalToBinary(number) + " ";
    }
    return result;
}

string circularShiftRight(const string& bitString, size_t shift) {
    const size_t bitSize = bitString.size();
    shift %= bitSize; // Ограничиваем сдвиг размером строки
    return bitString.substr(bitSize - shift) + bitString.substr(0, bitSize - shift);
}

string formatBitString(const string& bitString) {
    string formatted;
    for (size_t i = 0; i < bitString.size(); ++i) {
        formatted += bitString[i];
        if ((i + 1) % 8 == 0 && i + 1 < bitString.size()) {
            formatted += " ";
        }
    }
    return formatted;
}

string highlightStartingWithOne(const string& shiftedBitString) {
    stringstream highlighted;
    size_t pos = 0;
    string temp = shiftedBitString;
    while (temp.size() >= 8 && (pos = temp.find('1')) != string::npos) {
        highlighted << temp.substr(0, pos); // Part before '1'
        if (pos + 8 <= temp.size()) {
            highlighted << color(temp.substr(pos, 8), YELLOW_COLOR); // Highlight the byte starting with '1'
            temp = temp.substr(pos + 8); // Remove processed part
        } else {
            highlighted << temp.substr(pos); // Highlight the remaining part
            temp = "";
        }
    }
    highlighted << temp; // Remaining part
    return highlighted.str();
}

int main() {
    const string filename = getFilename();
    ifstream file(filename);
    if (!file) {
        cout << color("Error opening file '" + filename + "'!", RED_COLOR) << endl;
        return 1;
    }

    int n = getValidatedByteCount();
    n = (n > 32) ? 32 : (n < 3) ? 13 : n;

    vector<int> numbers = readNumbersFromFile(file, n);
    file.close();

    cout << color("Number sequence: ", CYAN_COLOR) << outputNumbers(numbers) << endl;

    string bitString = outputBinary(numbers);
    cout << color("Binary sequence: ", CYAN_COLOR) << bitString << endl;

    bitString.erase(remove(bitString.begin(), bitString.end(), ' '), bitString.end());
    cout << color("After right shift by 3 bits: ", CYAN_COLOR);
    cout << highlightStartingWithOne(formatBitString(circularShiftRight(bitString, 3))) << endl;

    return 0;
}