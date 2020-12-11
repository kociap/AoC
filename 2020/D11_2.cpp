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

    char operator()(i64 x, i64 y) const {
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

bool seek_seat(Map const& map, i64 x, i64 y, i64 const dx, i64 const dy) {
    x += dx;
    y += dy;
    while(x >= 0 && x < map.width && y >= 0 && y < map.height) {
        char const c = map(x, y);
        if(c == '#') {
            return true;
        } else if(c == 'L') {
            return false;
        }
        x += dx;
        y += dy;
    }
    return false;
}

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
            occupied_seats += seek_seat(map, x, y, -1, -1);
            occupied_seats += seek_seat(map, x, y, 0, -1);
            occupied_seats += seek_seat(map, x, y, 1, -1);
            occupied_seats += seek_seat(map, x, y, -1, 0);
            occupied_seats += seek_seat(map, x, y, 1, 0);
            occupied_seats += seek_seat(map, x, y, -1, 1);
            occupied_seats += seek_seat(map, x, y, 0, 1);
            occupied_seats += seek_seat(map, x, y, 1, 1);
            if(map(x, y) == '#' && occupied_seats >= 5) {
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
