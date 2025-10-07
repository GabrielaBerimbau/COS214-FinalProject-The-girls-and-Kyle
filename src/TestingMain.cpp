#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main() {
    cout << "=== Main Program Test ===" << endl;
    cout << "This compiles without raylib!" << endl;
    
    // Test some basic C++ features
    vector<string> items = {"Item 1", "Item 2", "Item 3"};
    
    cout << "\nItems in vector:" << endl;
    for (const auto& item : items) {
        cout << "  - " << item << endl;
    }
    
    // Simple calculation
    int sum = 0;
    for (int i = 1; i <= 10; i++) {
        sum += i;
    }
    cout << "\nSum of 1-10: " << sum << endl;
    
    cout << "\nProgram completed successfully!" << endl;
    
    return 0;
}