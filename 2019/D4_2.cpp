#include <iostream>

using i64 = long long;

bool has_double(i64 num) {
    i64 digits[10] = {};
    while(num != 0) {
        ++digits[num % 10];
        num /= 10;
    }

    for(i64 count: digits) {
        if(count == 2) {
            return true;
        }
    }
    return false;
}

bool is_monotone_increasing(i64 num) {
    i64 last_digit = 9;
    while(num != 0) {
        if(num % 10 > last_digit) {
            return false;
        }
        last_digit = num % 10;
        num /= 10;
    }
    return true;
}

int main() {
    std::ios_base::sync_with_stdio(false);

    i64 number = 266666;
    i64 limit = 699999;
    i64 count = 0;
    for(i64 i = number; i <= limit; ++i) {
        if(is_monotone_increasing(i) && has_double(i)) {
            ++count;
        }
    }

    std::cout << count;

    return 0;
}
