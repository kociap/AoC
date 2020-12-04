#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

using i64 = long long;

struct Object {
    std::string name;
    i64 parent_index = -1;
};

int main() {
    std::ios_base::sync_with_stdio(false);

    std::vector<Object> objects;

    auto find = [&](std::string n) -> i64 {
        for(i64 i = 0; i < objects.size(); ++i) {
            if(objects[i].name == n) {
                return i;
            }
        }
        return -1;
    };

    while(!std::cin.eof()) {
        std::string p1(3, ' ');
        std::cin.read(p1.data(), 3);
        std::cin.get();
        std::string p2(3, ' ');
        std::cin.read(p2.data(), 3);
        std::cin.get();

        i64 p1i = find(p1);
        i64 p2i = find(p2);
        
        if(p1i == -1) {
            objects.push_back(Object{p1, -1});
            p1i = objects.size() - 1;
        }

        if(p2i == -1) {
            objects.push_back(Object{p2, p1i});
            p2i = objects.size() - 1;
        } else {
            objects[p2i].parent_index = p1i;
        }
    }
    
    auto hierarchy_length = [&](i64 i) {
        i64 length = 0;
        for(i64 parent = objects[i].parent_index; parent != -1;) {
            ++length;
            parent = objects[parent].parent_index;
        }
        return length;
    };

    i64 orbits = 0;
    for(i64 i = 0; i < objects.size(); ++i) {
        orbits += hierarchy_length(i);
    }

    std::cout << orbits;

    return 0;
}
