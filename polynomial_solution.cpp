#include <iostream>
#include <string>
#include <vector>
#include "include/nlohmann/json.hpp"  
#include <cmath>

using json = nlohmann::json;
using namespace std;

// Function to convert a string from a given base to decimal
long long decode_base(const string &value, int base) {
    try {
        return std::stoll(value, nullptr, base);
    } catch (const std::out_of_range& e) {
        cerr << "Out of range error: " << e.what() << '\n';
        throw;
    } catch (const std::invalid_argument& e) {
        cerr << "Invalid argument error: " << e.what() << '\n';
        throw;
    }
}

// Function to perform Lagrange interpolation and find the constant term
double lagrange_interpolation(const vector<int> &x_values, const vector<long long> &y_values, int x) {
    double result = 0.0;
    size_t n = x_values.size();

    for (size_t i = 0; i < n; ++i) {
        double term = static_cast<double>(y_values[i]);
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
        "n": 9,
        "k": 6
    },
    "1": {
        "base": "10",
        "value": "28735619723837"
    },
    "2": {
        "base": "16",
        "value": "1A228867F0CA"
    },
    "3": {
        "base": "12",
        "value": "32811A4AA0B7B"
    },
    "4": {
        "base": "11",
        "value": "917978721331A"
    },
    "5": {
        "base": "16",
        "value": "1A22886782E1"
    },
    "6": {
        "base": "10",
        "value": "28735619654702"
    },
    "7": {
        "base": "14",
        "value": "71AB5070CC4B"
    },
    "8": {
        "base": "9",
        "value": "122662581541670"
    },
    "9": {
        "base": "8",
        "value": "642121030037605"
    }
})";

    // Parse the JSON input
    auto data = json::parse(json_input);
    int n = data["keys"]["n"];
    int k = data["keys"]["k"];

    // Extract points and decode values
    vector<int> x_values;
    vector<long long> y_values;

    for (auto& el : data.items()) {
        if (el.key() != "keys") {
            int x = std::stoi(el.key());
            int base = std::stoi(el.value()["base"].get<string>());
            long long y = decode_base(el.value()["value"].get<string>(), base);
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
