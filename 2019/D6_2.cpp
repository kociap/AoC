#include <iostream>
#include <vector>
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
    

    std::vector<i64> my_hierarchy;
    i64 me = find("YOU");
    for(i64 parent = objects[me].parent_index; parent != -1;) {
        my_hierarchy.push_back(parent);
        parent = objects[parent].parent_index;
    }

    auto is_common = [&](i64 i) -> i64 {
        for(i64 j = 0; j < my_hierarchy.size(); ++j) {
            if(my_hierarchy[j] == i) {
                return j;
            }
        }
        return -1;
    };

    i64 santa = find("SAN");
    i64 santa_length = 0;
    i64 santa_common_index = -1;
    for(i64 parent = objects[santa].parent_index; parent != -1;) {
        i64 common = is_common(parent);
        if(common != -1) {
            santa_common_index = common;
            break;
        }
        ++santa_length;
        parent = objects[parent].parent_index;
    }

    std::cout << santa_common_index + santa_length;

    return 0;
}
