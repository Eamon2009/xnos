#include <cmath>
#include <iostream>
using namespace std;

int main() {
    double num1, num2;
    double expo = 2;

    cout << "number1:";
    cin >> num1;

    cout << "number2:";
    cin >> num2;
    double add = num1 + num2;
    double mul = num1 * num2;
    double subt = num1 - num2;
    double exp1 = std::pow(num1, expo);
    double exp2 = std::pow(num2, expo);
    double div;
    if (num2 != 0) {
        div = num1 / num2;
    } else {
        cout << "$error";
        div = 0;
    }
    cout << "output" << endl;
    cout << "Addition" << add << endl;
    cout << "power1" << exp1 << endl;
    cout << "power2" << exp2 << endl;
    cout << "Multiplication" << mul << endl;
    if (num2 != 0) {
        cout << "Division" << div << endl;
    }
    return 0;
}