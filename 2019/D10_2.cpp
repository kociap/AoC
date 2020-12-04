#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>

using i64 = signed long long;
using u8 = unsigned char;
using f32 = float;

i64 max(i64 a, i64 b) {
    return a > b ? a : b;
}

i64 sign(i64 a) {
    return (a > 0) - (a < 0);
}

i64 abs_i64(i64 a) {
    return a < 0 ? -a : a;
}

i64 gcd(i64 a, i64 b) {
    while(b != 0) {
        i64 t = b; 
        b = a % b; 
        a = t; 
    }
    return a;
}

struct Vec2f {
    f32 x;
    f32 y;
};

Vec2f operator-(Vec2f v1, Vec2f v2) {
    return {v1.x - v2.x, v1.y - v2.y};
}

Vec2f operator/(Vec2f v, f32 f) {
    return {v.x / f, v.y / f};
}

f32 length(Vec2f v) {
    return sqrt(v.x * v.x + v.y * v.y);
}

void vaporize(std::vector<Vec2f>& rel_pos, std::vector<Vec2f>& vaporized) {
    Vec2f last_vaporized = rel_pos[0];
    rel_pos.erase(rel_pos.begin());
    vaporized.push_back(last_vaporized);
    float a_last_vaporized = atan2(last_vaporized.x, last_vaporized.y);
    for(i64 i = 0; i < rel_pos.size();) {
        float a_current = atan2(rel_pos[i].x, rel_pos[i].y);
        if(a_last_vaporized != a_current) {
            a_last_vaporized = a_current;
            last_vaporized = rel_pos[i];
            vaporized.push_back(last_vaporized);
            rel_pos.erase(rel_pos.begin() + i);
        } else {
            ++i;
        }
    }
    
}

int main() {
	std::ios_base::sync_with_stdio(false);

    // 29, 28 is the best asteroid for my input

	std::vector<u8> map;
    i64 width = 0;
    i64 height = 0;
    while(std::cin) {
        for(u8 c = std::cin.get(); c == '#' || c == '.';) {
            map.push_back(c);
            c = std::cin.get();
        }
        ++height;
    }

    width = map.size() / height;

    std::vector<Vec2f> rel_pos;
    for(i64 i = 0; i < map.size(); ++i) {
        Vec2f p = Vec2f{(f32)(i % width), (f32)(i / width)};
        if(map[i] == '#' && (p.x != 29 || p.y != 28)) {
            rel_pos.push_back(p - Vec2f{29, 28});
        }
    }

    std::sort(rel_pos.begin(), rel_pos.end(), [](Vec2f v1, Vec2f v2) -> bool {
        float a1 = atan2(-v1.x, v1.y);
        float a2 = atan2(-v2.x, v2.y);
        return a1 < a2 || (a1 == a2 && length(v1) < length(v2));
    });

    std::vector<Vec2f> vaporized;
    while(vaporized.size() < 200) {
        vaporize(rel_pos, vaporized);
    }

    std::cout << (vaporized[199].x + 29) << ' ' << (vaporized[199].y + 28);

	return 0;
}
