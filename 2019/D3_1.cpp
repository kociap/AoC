#include <iostream>
#include <vector>

using i64 = long long;
using u64 = unsigned long long;

struct Vec {
    i64 x;
    i64 y;
};

Vec operator+(Vec v1, Vec v2) {
    return {v1.x + v2.x, v1.y + v2.y};
}

Vec operator*(Vec v, i64 a) {
    return {v.x * a, v.y * a};
}

bool is_within(i64 v, i64 p1, i64 p2) {
    return (v < p1 && v > p2) || (v > p1 && v < p2);
}

i64 abs(i64 a) {
    return a < 0 ? -a : a;
}

void read_dirs(std::vector<Vec>& dirs) {
    for(i64 a; !std::cin.eof();) {
        char dirc = std::cin.get();
        Vec dir = {0, 0};
        switch(dirc) {
            case 'R':
                dir = {1, 0};
                break;
            case 'L':
                dir = {-1, 0};
                break;
            case 'U':
                dir = {0, 1};
                break;
            case 'D':
                dir = {0, -1};
                break;
        }

        std::cin >> a;
        dirs.push_back(dir * a);
        std::cin.clear();
        char next_c = std::cin.get();
        if(next_c == '\n') {
            break;
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);

    std::vector<Vec> dirs1;
    std::vector<Vec> dirs2;
    read_dirs(dirs1);
    read_dirs(dirs2);

    i64 closest_point = -1ULL >> 1;
    Vec p1 = {0, 0};
    for(Vec l1: dirs1) {
        Vec p2 = p1 + l1;
        Vec p3 = {0, 0};
        for(Vec l2: dirs2) {
            Vec p4 = p3 + l2;
            if(l1.x == 0 && l2.x != 0) {
                // Perpendicular
                if(is_within(p2.x, p4.x, p3.x) && is_within(p4.y, p2.y, p1.y) && (abs(p2.x) + abs(p4.y)) < closest_point) {
                    closest_point = abs(p2.x) + abs(p4.y);
                }
            } else if(l1.x != 0 && l2.x == 0) {
                // Perpendicular
                if(is_within(p4.x, p2.x, p1.x) && is_within(p2.y, p4.y, p3.y) && (abs(p4.x) + abs(p2.y)) < closest_point) {
                    closest_point = abs(p4.x) + abs(p2.y);
                }
            }

            p3 = p4;
        }
        p1 = p2;
    }

    std::cout << closest_point;

    return 0;
}
