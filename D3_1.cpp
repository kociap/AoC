#include <iostream>
#include <vector>
#include <string>

using i64 = long long;
using f64 = double;

int main() {
    constexpr char tree = '#';
    std::vector<std::vector<char>> map;
    while(true) {
        std::vector<char> level;
        while(true) {
            char c = std::cin.get();
            if(c == '\n' || !std::cin) {
                break;
            }

            level.emplace_back(c);
        }

        if(!std::cin) {
            break;
        }

        map.emplace_back(std::move(level));
    }

    i64 x = 0;
    i64 y = 0;
    i64 const height = map.size();
    i64 const width = map[0].size();
    i64 trees_encountered = 0;
    while(y < height) {
        if(map[y][x] == tree) {
            trees_encountered += 1;
        }

        x = (x + 3) % width;
        y += 1;
    }

    std::cout << trees_encountered;

    return 0;
}
