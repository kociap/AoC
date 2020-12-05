#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using i64 = long long;

i64 decode_pass(std::string const& pass) {
    i64 id = 0;
    for(char const c: pass) {
        id <<= 1;
        id += (c == 'B' || c == 'R');
    }
    return id;
}

int main() {
    std::vector<std::string> boarding_passes;
    for(std::string pass;;) {
        std::cin >> pass;
        if(!std::cin) {
            break;
        }
        boarding_passes.push_back(pass);
    }

    i64 max_id = 0;
    std::vector<i64> ids;
    for(std::string const& pass: boarding_passes) {
        i64 const id = decode_pass(pass);
        if(id > max_id) {
            max_id = id;
        }

        ids.push_back(id);
    }

    std::cout << max_id << '\n';

    std::sort(ids.begin(), ids.end());

    // visual solution
    // for(i64 const id: ids) {
    //     if(id % 8 == 0) {
    //         std::cout << '\n';
    //     }
    //     std::cout << id << ' ';
    // }

    for(i64 i = 1; i < ids.size(); ++i) {
        if(ids[i - 1] + 1 != ids[i]) {
            std::cout << (ids[i - 1] + 1) << '\n';
        }
    }

    return 0;
}
