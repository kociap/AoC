#include <array>
#include <iostream>
#include <random>
#include <vector>
#include <string>
#include <cmath>

using i64 = long long;

int main() {
    std::vector<i64> nums;
    while(true) {
        i64 arg;
        std::cin >> arg;
        if(!std::cin) {
            break;
        }

        nums.push_back(arg);
    }

    for(i64 i = 25; i < nums.size(); ++i) {
        bool found = false;
        for(i64 j = i - 25; j < i; ++j) {
            for(i64 k = j + 1; k < i; ++k) {
                found |= (nums[j] + nums[k] == nums[i]);
            }
        }

        if(!found) {
            std::cout << nums[i];
            break;
        }
    }

    return 0;
}
