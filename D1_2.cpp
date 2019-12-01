#include <iostream>

using i64 = long long;

int main() {
    std::ios_base::sync_with_stdio(false);

    i64 counter_upper = 0;
    i64 module_mass;
    while(std::cin >> module_mass) {
        for(i64 fuel_mass = module_mass / 3 - 2; fuel_mass > 0;) {
            counter_upper += fuel_mass;
            fuel_mass = fuel_mass / 3 - 2;
        }
    }

    std::cout << counter_upper;
    return 0;
}
