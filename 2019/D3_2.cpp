#include <iostream>
#include <vector>

using i64 = long long;

i64 abs(i64 a) {
    return a < 0 ? -a : a;
}

i64 min(i64 a, i64 b) {
    return a < b ? a : b;
}

struct Vec {
    i64 x;
    i64 y;
};

Vec operator+(Vec v1, Vec v2) {
    return {v1.x + v2.x, v1.y + v2.y};
}

Vec operator-(Vec v1, Vec v2) {
    return {v1.x - v2.x, v1.y - v2.y};
}

Vec operator*(Vec v, i64 a) {
    return {v.x * a, v.y * a};
}

i64 length(Vec v) {
    return abs(v.x) + abs(v.y);
}

bool is_within(i64 v, i64 p1, i64 p2) {
    return (v < p1 && v > p2) || (v > p1 && v < p2);
}

struct Intersect_Result {
    bool intersected;
    Vec point;
};

Intersect_Result test_intersect(Vec p1, Vec p2, Vec p3, Vec p4) {
    Vec l1 = p2 - p1;
    Vec l2 = p4 - p3;
    if(l1.x == 0 && l2.x != 0) {
        // Perpendicular
        return {is_within(p2.x, p4.x, p3.x) && is_within(p4.y, p2.y, p1.y), {p2.x, p4.y}};
    } else if(l1.x != 0 && l2.x == 0) {
        // Perpendicular
        return {is_within(p4.x, p2.x, p1.x) && is_within(p2.y, p4.y, p3.y), {p4.x, p2.y}};
    }

    return {false, {0, 0}};
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

void fill_steps(std::vector<Vec> const& dirs, std::vector<i64>& out) {
    i64 w_steps = 0;
    Vec p1 = {0, 0};
    out.push_back(0);
    for(Vec l: dirs) {
        w_steps += abs(l.x) + abs(l.y);
        out.push_back(w_steps);
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);

    std::vector<Vec> dirs1;
    std::vector<Vec> dirs2;
    read_dirs(dirs1);
    read_dirs(dirs2);

    std::vector<i64> steps_w1;
    steps_w1.reserve(dirs1.size() + 1);
    std::vector<i64> steps_w2;
    steps_w2.reserve(dirs2.size() + 1);

    fill_steps(dirs1, steps_w1);
    fill_steps(dirs2, steps_w2);

    i64 min_steps = -1ULL >> 1;
    Vec p1 = {0, 0};
    for(i64 i = 0; i < dirs1.size(); ++i) {
        Vec p2 = p1 + dirs1[i];
        Vec p3 = {0, 0};
        for(i64 j = 0; j < dirs2.size(); ++j) {
            Vec p4 = p3 + dirs2[j];
            if(auto [passed, point] = test_intersect(p1, p2, p3, p4); passed) {
                min_steps = min(min_steps, steps_w1[i] + length(point - p1) + steps_w2[j] + length(point - p3));
            }
            p3 = p4;
        }
        p1 = p2;
    }

    std::cout << min_steps;

    return 0;
}
