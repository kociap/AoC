#include <array>
#include <cmath>
#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

using i64 = long long;

struct Nested_Bag {
    i64 name;
    i64 count;
};

bool can_contain_bag(std::unordered_map<i64, std::vector<Nested_Bag>> const& bags, i64 const bag_name, i64 const target_bag_name) {
    auto iter = bags.find(bag_name);
    for (auto& b : iter->second) {
        if (b.name == target_bag_name) {
            return true;
        } else {
            bool res = can_contain_bag(bags, b.name, target_bag_name);
            if (res) {
                return true;
            }
        }
    }

    return false;
}

int main() {
    std::unordered_map<i64, std::vector<Nested_Bag>> bags;
    std::string buf;
    std::hash<std::string> h;
    while (std::cin.peek() != EOF) {
        for (i64 space_count = 0; space_count < 2;) {
            char const c = std::cin.get();
            if (c != ' ') {
                buf.push_back(c);
            } else {
                space_count += 1;
            }
        }

        i64 const bag_name = h(buf);
        buf.clear();

        // skip "bags contain "
        for (i64 i = 0; i < 13; ++i) {
            std::cin.get();
        }

        if (std::cin.peek() == 'n') {
            // skip "no other bags.\n"
            for (i64 i = 0; i < 15; ++i) {
                std::cin.get();
            }

            bags.emplace(bag_name, std::vector<Nested_Bag>());
        } else {
            std::vector<Nested_Bag> nested_bags;
            while (true) {
                // all bags hold 1 digit number of other bags
                i64 const count = std::cin.get() - '0';
                std::cin.get();

                for (i64 space_count = 0; space_count < 2;) {
                    char const c = std::cin.get();
                    if (c != ' ') {
                        buf.push_back(c);
                    } else {
                        space_count += 1;
                    }
                }

                i64 const nested_bag_name = h(buf);
                buf.clear();

                nested_bags.push_back(Nested_Bag{nested_bag_name, count});

                if(count == 1) {
                    // skip "bag"
                    for (i64 i = 0; i < 3; ++i) {
                        std::cin.get();
                    }
                } else {
                    // skip "bags"
                    for (i64 i = 0; i < 4; ++i) {
                        std::cin.get();
                    }
                }

                // skip comma or dot
                std::cin.get();
                char const line_end = std::cin.get();
                if (line_end == '\n') {
                    break;
                }
            }
            bags.emplace(bag_name, nested_bags);
        }
    }

    i64 const bag_name = h("shinygold");
    i64 count = 0;
    for (auto [bag, nbags] : bags) {
        count += can_contain_bag(bags, bag, bag_name);
    }
    std::cout << count;

    return 0;
}
