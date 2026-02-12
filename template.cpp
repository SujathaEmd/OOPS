#include <iostream>
#include <string>

// --- 1. Function Template ---
template <typename T>
void printSwap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
    std::cout << "Swapped values: " << a << " and " << b << std::endl;
}

// --- 2. Class Template ---
template <class T>
class Pair {
private:
    T first, second;
public:
    Pair(T a, T b) : first(a), second(b) {}
    
    void show() {
        std::cout << "Pair: [" << first << ", " << second << "]" << std::endl;
    }
};

int main() {
    // Testing Function Template with integers
    int x = 10, y = 20;
    std::cout << "Original ints: " << x << ", " << y << std::endl;
    printSwap(x, y);

    // Testing Class Template with strings
    Pair<std::string> wordPair("Hello", "World");
    wordPair.show();

    return 0;
}