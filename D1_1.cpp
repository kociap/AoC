#include <iostream>

using i64 = long long;

i64 max(i64 const a, i64 const b) {
    return a > b ? a : b;
}

int main() {
    std::ios_base::sync_with_stdio(false);

    i64 counter_upper = 0;
    i64 module_mass;
    while(std::cin >> module_mass) {
        // The input numbers are large enough that the result is never < 0
        //  thus that `max` isn't necessary
        counter_upper += max(module_mass / 3 - 2, 0);
    }

    std::cout << counter_upper;
    return 0;
}
