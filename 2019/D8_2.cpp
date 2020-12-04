#include <iostream>
#include <string.h>
#include <string>

using i64 = signed long long;
using u8 = unsigned char;

int main() {
	std::ios_base::sync_with_stdio(false);

    std::string in;
    std::getline(std::cin, in, '\n');
	
    constexpr i64 layer_width = 25;
    constexpr i64 layer_height = 6;
    constexpr i64 layer_size = 150;

    u8 image[layer_size];
    memset(image, '2', layer_size);

    i64 const layer_count = in.size() / layer_size;
    for(i64 i = 0; i < layer_count; ++i) {
        i64 const in_offset = i * layer_size;
        for(i64 j = 0; j < layer_size; ++j) {
            if(image[j] == '2') {
                image[j] = in[in_offset + j];
            }
        }
    }

    for(i64 i = 0; i < layer_height; ++i) {
        for(i64 j = 0; j < layer_width; ++j) {
            std::cout << image[i * layer_width + j];
        }
        std::cout << '\n';
    }

	return 0;
}
