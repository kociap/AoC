#include <iostream>
#include <string.h>
#include <string>
#include <vector>

using i64 = long long;

enum class operation {
	cut,
	deal_increment,
	deal_new_stack,
};

operation get_next_operation() {
	char c = std::cin.get();
	char buf[20];
	if (c == 'c') {
		std::cin.read(buf, 3);
		return operation::cut;
	} else {
		std::cin.read(buf, 4);
		char d = std::cin.get();
		if (d == 'w') {
			std::cin.read(buf, 14);
			return operation::deal_increment;
		} else {
			std::cin.read(buf, 13);
			return operation::deal_new_stack;
		}
	}
}

int main() {
	std::ios_base::sync_with_stdio(false);

	constexpr i64 card_count = 10007;

	std::vector<i64> stack1;
	stack1.reserve(card_count);
	std::vector<i64> stack2(card_count, -1);
	for (i64 i = 0; i < card_count; ++i) {
		stack1.push_back(i);
	}

	while (!std::cin.eof()) {
		operation op = get_next_operation();
		switch (op) {
		case operation::cut: {
			i64 cut_count;
			std::cin >> cut_count;
			if (cut_count < 0) {
				cut_count = card_count + cut_count;
			}

			i64 i = 0;
			for (i64 j = cut_count; j < card_count; ++j, ++i) {
				stack2[i] = stack1[j];
			}

			for (i64 j = 0; j < cut_count; ++j, ++i) {
				stack2[i] = stack1[j];
			}
			std::swap(stack1, stack2);
		} break;

		case operation::deal_increment: {
			i64 increment;
			std::cin >> increment;
			for (i64 i = 0, j = 0; i < card_count; ++i) {
				stack2[j] = stack1[i];
				j += increment;
				j %= card_count;
			}
			std::swap(stack1, stack2);
		} break;

		case operation::deal_new_stack: {
			for (i64 i = 0; i < card_count; ++i) {
				stack2[card_count - 1 - i] = stack1[i];
			}
			std::swap(stack1, stack2);
		} break;
		}
		std::cin.get();
	}

	for (i64 i = 0; i < card_count; ++i) {
		if (stack1[i] == 2019) {
			std::cout << i;
			break;
		}
	}

	return 0;
}
