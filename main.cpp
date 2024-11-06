#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <windows.h>

using namespace std;

// Определение цветовых кодов для Windows
constexpr int RED_COLOR = 4;
constexpr int GREEN_COLOR = 2;
constexpr int CYAN_COLOR = 3;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

// Функция для установки цвета текста в консоли
void setColor(const int colorCode) {
    SetConsoleTextAttribute(hConsole, colorCode);
}

// Сброс цвета текста в консоли
void resetColor() {
    SetConsoleTextAttribute(hConsole, 7);
}

// Функция для окрашивания текста
void printColor(const string &str, const int colorCode) {
    setColor(colorCode);
    cout << str;
    resetColor();
}

// Функция для чтения чисел из файла
vector<int> readNumbersFromFile(const string &filename, const int n) {
    ifstream file(filename);
    vector<int> numbers;
    int number;

    if (!file.is_open()) {
        printColor("Error opening file '" + filename + "'!\n", RED_COLOR);
        return numbers;
    }

    // Чтение чисел построчно
    while (file >> number && numbers.size() < n) {
        numbers.push_back(number);
    }

    return numbers;
}

// Функция для вывода чисел
void printNumbers(const vector<int> &numbers) {
    printColor("Byte sequence: ", CYAN_COLOR);
    for (const auto &num : numbers) {
        cout << num << " ";
    }
    cout << endl;
}

// Функция для создания бинарного массива из чисел
vector<int> decimalToBinaryArray(const vector<int> &numbers) {
    vector<int> binaryArray;
    for (const auto &number : numbers) {
        if (number < 0) {
            printColor("Negative numbers are not supported.\n", RED_COLOR);
            continue;
        }
        bitset<8> bits(number);
        for (int i = 7; i >= 0; --i) {
            binaryArray.push_back(bits[i]);
        }
    }
    return binaryArray;
}

// Функция для вывода бинарного массива
void printBinaryArray(const vector<int> &binaryArray) {
    printColor("Binary sequence: ", CYAN_COLOR);
    for (size_t i = 0; i < binaryArray.size(); i += 8) {
        for (int j = 0; j < 8 && i + j < binaryArray.size(); ++j) {
            cout << binaryArray[i + j];
        }
        cout << " ";
    }
    cout << endl;
}

// Основная функция
int main() {
    string filename;
    cout << "Enter the file name: ";
    cin >> filename;

    int n;
    cout << "Enter the number of bytes (1-32): ";
    cin >> n;
    n = max(1, min(n, 32));

    // Чтение и вывод чисел из файла
    vector<int> numbers = readNumbersFromFile(filename, n);
    if (numbers.empty()) return 1;
    printNumbers(numbers);

    // Создание бинарного массива
    vector<int> binaryArray = decimalToBinaryArray(numbers);
    printBinaryArray(binaryArray);

    // Сдвиг массива и реверс байтов с единицей в первом бите
    rotate(binaryArray.begin(), binaryArray.begin() + 4, binaryArray.end());

    printColor("Left shift -> by 4 bits: ", CYAN_COLOR);
    int countReversed = 0;
    for (size_t i = 0; i < binaryArray.size(); i += 8) {
        bool isReversed = binaryArray[i] == 1;
        for (int j = 0; j < 8; ++j) {
            if (isReversed) {
                printColor(to_string(binaryArray[i + 7 - j]), RED_COLOR);
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

    printColor("Number of reversed bytes: ", GREEN_COLOR);
    cout << countReversed << endl;

    return 0;
}
