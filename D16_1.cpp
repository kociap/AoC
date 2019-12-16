#include <iostream>
#include <vector>
#include <algorithm>

using i64 = signed long long;
using u8 = unsigned char;
using i8 = signed char;
using f32 = float;

i64 abs(i64 a) {
    return a < 0 ? -a : a;
}

std::vector<i64> fft_phase(std::vector<i64> const& sequence) {
    std::vector<i64> out(sequence.size(), 0);
    for(i64 i = 0; i < sequence.size(); ++i) {
        i64 offset = -1;
        i64 element = 0;
        while(offset < (i64)sequence.size()) {
            // Skip multiplications by 0
            offset += i + 1;

            for(i64 k = 0; k < i + 1 && offset < sequence.size(); ++k, ++offset) {
                element += sequence[offset];
            }

            // Skip multiplications by 0 again
            offset += i + 1;

            for(i64 k = 0; k < i + 1 && offset < sequence.size(); ++k, ++offset) {
                element -= sequence[offset];
            }
        }
        out[i] = abs(element % 10);
    }
    return out;
}

int main() {
	std::ios_base::sync_with_stdio(false);

    std::vector<i64> sequence;
    for(i8 c = std::cin.get(); c != EOF; c = std::cin.get()) {
        sequence.push_back(c - '0');
    }

    for(i64 i = 1; i <= 100; ++i) {
        sequence = fft_phase(sequence);
    }

    for(i64 i = 0; i < 8; ++i) {
        std::cout << sequence[i];
    }
    
	return 0;
}
