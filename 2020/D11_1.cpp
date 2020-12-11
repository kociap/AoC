#include <iostream>
#include <vector>

using i64 = long long;

struct Map {
    i64 width;
    i64 height;
    std::vector<char> map;

    i64 size() const {
        return map.size();
    }

    char& operator()(i64 x, i64 y) {
        return map[width * y + x];
    }

    bool operator==(Map const& other) const {
        for(i64 i = 0; i < map.size(); ++i) {
            if(map[i] != other.map[i]) {
                return false;
            }
        }
        return true;
    }
};

int main() {
    std::vector<char> map_v;
    i64 height = 0;
    while(true) {
        char c = std::cin.get();
        if(!std::cin) {
            break;
        }

        if(c == '\n') {
            height += 1;
            continue;
        }
        map_v.push_back(c);
    }

    Map map{(i64)map_v.size() / height, height, map_v};
    while(true) {
        Map map_c = map;
        for(i64 i = 0; i < map.size(); ++i) {
            i64 y = i / map.width;
            i64 x = i % map.width;
            i64 occupied_seats = 0;
            occupied_seats += (x - 1) >= 0 && (y - 1) >= 0 && (map(x - 1, y - 1) == '#');
            occupied_seats += (y - 1) >= 0 && (map(x, y - 1) == '#');
            occupied_seats += (x + 1) < map.width && (y - 1) >= 0 && (map(x + 1, y - 1) == '#');
            occupied_seats += (x - 1) >= 0 && (map(x - 1, y) == '#');
            occupied_seats += (x + 1) < map.width && (map(x + 1, y) == '#');
            occupied_seats += (x - 1) >= 0 && (y + 1) < map.height && (map(x - 1, y + 1) == '#');
            occupied_seats += (y + 1) < map.height && (map(x, y + 1) == '#');
            occupied_seats += (x + 1) < map.width && (y + 1) < map.height && (map(x + 1, y + 1) == '#');
            if(map(x, y) == '#' && occupied_seats >= 4) {
                map_c(x, y) = 'L';
            } else if(map(x, y) == 'L' && occupied_seats == 0) {
                map_c(x, y) = '#';
            }
        }

        if(map_c == map) {
            break;
        }

        map = map_c;
    }

    i64 taken_seats = 0;
    for(char c: map.map) {
        taken_seats += c == '#';
    }
    std::cout << taken_seats;

    return 0;
}
