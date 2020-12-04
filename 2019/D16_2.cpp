#include <iostream>
#include <vector>
#include <algorithm>
#include <string.h>
#include <utility>

using i64 = signed long long;
using u8 = unsigned char;
using i8 = signed char;
using f32 = float;

i64 abs_i64(i64 a) {
	return a < 0 ? -a : a;
}

// key_offset is > sequence.size() / 2
std::vector<i64> fft_phase(std::vector<i64> sequence, i64 const key_offset, i64 const iterations) {
	i64 sum = 0;
	for (i64 i = key_offset; i < sequence.size(); ++i) {
		sum += sequence[i];
	}

	i64 sum_out = 0;
	std::vector<i64> out(sequence.size(), 0);
	for (i64 iter = 0; iter < iterations; ++iter) {
		out[key_offset] = abs_i64(sum % 10);
		sum_out += abs_i64(sum % 10);

		for (i64 i = key_offset + 1; i < sequence.size(); ++i) {
			sum -= sequence[i - 1];
			out[i] = abs_i64(sum % 10);
			sum_out += abs_i64(sum % 10);
		}

		sum = sum_out;
		sum_out = 0;
		std::swap(out, sequence);
	}

	return sequence;
}

std::vector<i64> repeat(std::vector<i64> const& sequence, i64 times) {
	std::vector<i64> out(sequence.size() * times, 0);
	for (i64 i = 0; i < times; ++i) {
		memcpy(out.data() + sequence.size() * i, sequence.data(), sequence.size() * sizeof(i64));
	}
	return out;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	std::vector<i64> sequence;
	for (i8 c = std::cin.get(); c >= '0'; c = std::cin.get()) {
		sequence.push_back(c - '0');
	}

    // Could repeat just the end, but meh
	std::vector<i64> repeated = repeat(sequence, 10000);

	i64 key_offset = 0;
	for (i64 i = 0; i < 7; ++i) {
		key_offset *= 10;
		key_offset += sequence[i];
	}

	repeated = fft_phase(repeated, key_offset, 100);

	for (i64 i = 0; i < 8; ++i) {
		std::cout << repeated[key_offset + i];
	}

	return 0;
}
