#include <iostream>
#include <vector>

using i64 = long long;
using u8 = unsigned char;

i64 max(i64 a, i64 b) {
    return a > b ? a : b;
}

static bool trace_line(std::vector<u8>& map, i64 const width, i64 const height, i64 const x, i64 const y, i64 const w, i64 const h) {
    bool hit = false;
    for(i64 f = 1, line_x = x + w * f, line_y = y + h * f; line_x >= 0 && line_x < width && line_y >= 0 && line_y < height; ++f, line_x = x + w * f, line_y = y + h * f) {
        i64 const map_index = line_y * width + line_x;
        if(map[map_index] == '#') {
            hit = true;
        }

        map[map_index] = 'X';
    }
    
    return hit;
}

static i64 compute_visible(std::vector<u8> map, i64 const width, i64 const height, i64 const x, i64 const y) {
    i64 count = 0;
    for(i64 w = 1; w < width; ++w) {
        for(i64 h = -height + 1; h < height; ++h) {
            count += trace_line(map, width, height, x, y, w, h);
            count += trace_line(map, width, height, x, y, -w, h);
        }
    }
    count += trace_line(map, width, height, x, y, 1, 0);
    count += trace_line(map, width, height, x, y, -1, 0);
    count += trace_line(map, width, height, x, y, 0, 1);
    count += trace_line(map, width, height, x, y, 0, -1);
    return count;
}

int main() {
	std::ios_base::sync_with_stdio(false);

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

    i64 highest_count = 0;
    for(i64 i = 0; i < map.size(); ++i) {
        i64 const x = i % width;
        i64 const y = i / width;
        if(map[i] == '#') {
            i64 const count = compute_visible(map, width, height, x, y);
            highest_count = max(count, highest_count);
        }
    }

    std::cout << highest_count;

	return 0;
}
