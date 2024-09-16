#include <iostream>
#include <string>
#include <vector>
#include "include/nlohmann/json.hpp"  
#include <cmath>

using json = nlohmann::json;
using namespace std;

// Function to convert a string from a given base to decimal
int decode_base(const string &value, int base) {
    return stoi(value, nullptr, base);
}

// Function to perform Lagrange interpolation and find the constant term
double lagrange_interpolation(const vector<int> &x_values, const vector<int> &y_values, int x) {
    double result = 0.0;
    size_t n = x_values.size();

    for (size_t i = 0; i < n; ++i) {
        double term = y_values[i];
        for (size_t j = 0; j < n; ++j) {
            if (i != j) {
                term *= (x - x_values[j]) / static_cast<double>(x_values[i] - x_values[j]);
            }
        }
        result += term;
    }
    return result;
}

int main() {
    // Sample JSON input
    string json_input = R"({
        "keys": {
            "n": 4,
            "k": 3
        },
        "1": {
            "base": "10",
            "value": "4"
        },
        "2": {
            "base": "2",
            "value": "111"
        },
        "3": {
            "base": "10",
            "value": "12"
        },
        "6": {
            "base": "4",
            "value": "213"
        }
    })";

    // Parse the JSON input
    auto data = json::parse(json_input);
    int n = data["keys"]["n"];
    int k = data["keys"]["k"];

    // Extract points and decode values
    vector<int> x_values;
    vector<int> y_values;

    for (auto& el : data.items()) {
        if (el.key() != "keys") {
            int x = stoi(el.key());
            int base = stoi(el.value()["base"].get<string>());
            int y = decode_base(el.value()["value"].get<string>(), base);
            x_values.push_back(x);
            y_values.push_back(y);
        }
    }

    // Ensure we have enough points
    if (x_values.size() < k) {
        cerr << "Not enough points to determine the polynomial" << endl;
        return 1;
    }

    // Calculate the constant term by evaluating the polynomial at x = 0
    double constant_term = lagrange_interpolation(x_values, y_values, 0);
    cout << "The constant term is: " << constant_term << endl;

    return 0;
}
