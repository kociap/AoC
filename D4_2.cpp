#include <iostream>

using i64 = long long;
using u64 = unsigned long long;

i64 pow_10(i64 pow) {
    switch(pow) {
        case 0: return 1;
        case 1: return 10;
        case 2: return 100;
        case 3: return 1000;
        case 4: return 10000;
        case 5: return 100000;
        case 6: return 1000000;
        default: return -1;
    }
}

i64 digit_at(i64 num, i64 pos) {
    return (num / pow_10(pos)) % 10;
}

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
        // std::cout << i << ' ' << has_double(i) << ' ' << is_monotone_increasing(i) << '\n';
        if(is_monotone_increasing(i) && has_double(i)) {
            ++count;
        }
    }

    std::cout << count;

    return 0;
}
