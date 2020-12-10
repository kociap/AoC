#include <iostream>
#include <vector>
#include <algorithm>

using i64 = long long;

int main() {
    std::vector<i64> adapters;
    adapters.push_back(0);

    while(true) {
        i64 a;
        std::cin >> a;
        if(!std::cin) {
            break;
        }

        adapters.push_back(a);
    }


    std::sort(adapters.begin(), adapters.end());
    adapters.push_back(adapters[adapters.size() - 1] + 3);
    std::vector<i64> combinations(adapters.size(), 0);
    combinations[combinations.size() - 1] = 1;
    for(i64 i = adapters.size() - 1; i >= 0; --i) {
        for(i64 j = i - 1; j >= 0; --j) {
            if(adapters[i] - adapters[j] <= 3) {
                combinations[j] += combinations[i];
            } else {
                break;
            }
        }
    }

    std::cout << combinations[0];

    return 0;
}
