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

        while(true) {
            char const c = std::cin.get();
            if(char const p = std::cin.peek(); c == '\n' && (p == '\n' || p == EOF)) {
                break;
            }

            if(c == '\n') {
                continue;
            }

            ++answers[c - 'a'];
        }

        std::cin.get();

        for(i64 i = 0; i < 26; ++i) {
            total_answers += answers[i] > 0;
        }

        if(std::cin.peek() == EOF) {
            break;
        }
    }

    std::cout << total_answers;

    return 0;
}
