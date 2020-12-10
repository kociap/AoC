#include <iostream>
#include <vector>
#include <algorithm>

using i64 = long long;

int main() {
    std::vector<i64> adapters;

    while(true) {
        i64 a;
        std::cin >> a;
        if(!std::cin) {
            break;
        }

        adapters.push_back(a);
    }

    std::sort(adapters.begin(), adapters.end());

    i64 jolt_1 = 0;
    i64 jolt_3 = 0;
    i64 last_adapter = 0;
    for(i64 adapter: adapters) {
        i64 dj = adapter - last_adapter;
        std::cout << adapter << ' ' << dj << '\n';
        if(dj == 1) {
            jolt_1 += 1;
        } else {
            jolt_3 += 1;
        }

        last_adapter = adapter;
    }

    std::cout << (jolt_1 * (jolt_3 + 1));

    return 0;
}
