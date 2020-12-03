#include <iostream>
#include <vector>
#include <string>

using i64 = long long;
using f64 = double;

i64 count_trees(std::vector<std::vector<char>> const& map, i64 const x_slope, i64 const y_slope) {
    constexpr char tree = '#';
    i64 x = 0;
    i64 y = 0;
    i64 const height = map.size();
    i64 const width = map[0].size();
    i64 trees_encountered = 0;
    while(y < height) {
        if(map[y][x] == tree) {
            trees_encountered += 1;
        }

        x = (x + x_slope) % width;
        y += y_slope;
    }
    return trees_encountered;
}

int main() {
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

    i64 s11 = count_trees(map, 1, 1);
    i64 s31 = count_trees(map, 3, 1);
    i64 s51 = count_trees(map, 5, 1);
    i64 s71 = count_trees(map, 7, 1);
    i64 s12 = count_trees(map, 1, 2);
    std::cout << (s11 * s31 * s51 * s71 * s12);

    return 0;
}
