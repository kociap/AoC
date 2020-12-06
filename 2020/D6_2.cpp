#include <array>
#include <iostream>
#include <random>
#include <vector>
#include <string>
#include <cmath>

using i64 = long long;

int main() {
    i64 total_answers = 0;
    while(true) {
        std::vector<i64> answers(26, 0);

        i64 group_size = 1;
        while(true) {
            char const c = std::cin.get();
            if(char const p = std::cin.peek(); c == '\n' && (p == '\n' || p == EOF)) {
                break;
            }

            if(c == '\n') {
                group_size += 1;
                continue;
            }

            ++answers[c - 'a'];
        }

        std::cin.get();

        for(i64 i = 0; i < 26; ++i) {
            total_answers += answers[i] == group_size;
        }

        if(std::cin.peek() == EOF) {
            break;
        }
    }

    std::cout << total_answers;

    return 0;
}
