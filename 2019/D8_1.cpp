#include <iostream>
#include <string>

using i64 = long long;

int main() {
	std::ios_base::sync_with_stdio(false);

    std::string in;
    std::getline(std::cin, in, '\n');
	
    constexpr i64 layer_size = 150;

    i64 multiplied = 0;
    i64 least_0 = -1ULL >> 1;

    for(i64 i = 0; i < in.size();) {
        i64 digits[3] = {};
        for(i64 j = 0; j < layer_size; ++j, ++i) {
            ++digits[in[i] - '0'];
        }

        if(digits[0] < least_0) {
            least_0 = digits[0];
            multiplied = digits[1] * digits[2];
        }
    }

    std::cout << multiplied;

	return 0;
}
