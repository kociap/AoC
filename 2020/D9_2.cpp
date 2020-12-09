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

    i64 const num = 731031916;
    for(i64 i = 0; i < nums.size(); ++i) {
        i64 count = 0;
        i64 sum = 0;
        for(i64 j = i; j < nums.size() && sum <= num; ++j) {
            sum += nums[j];
            count += 1;
            if(sum == num && count > 1) {
                i64 max = nums[i];
                i64 min = nums[i];
                for(i64 k = i + 1; k <= j; ++k) {
                    max = (nums[k] > max ? nums[k] : max);
                    min = (nums[k] < min ? nums[k] : min);
                }
                std::cout << max << ' ' << min << ' ' << (max + min);
                return 0;
            }
        }
    }

    return 0;
}
