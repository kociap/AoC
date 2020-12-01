#include <iostream>
#include <vector>

using i64 = long long;

int main() {
    std::vector<i64> n;
    i64 a;
    while(std::cin >> a) {
        n.push_back(a);
    }

    for(i64 i = 0; i < n.size(); ++i) {
        for(i64 j = i + 1; j < n.size(); ++j) {
            if(n[i] + n[j] == 2020) {
                std::cout << n[i] << ' ' << n[j] << ' ' << (n[i] * n[j]) << '\n';
            }
        }
    }

    return 0;
}
