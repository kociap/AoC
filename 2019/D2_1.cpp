#include <iostream>
#include <vector>

using i64 = long long;

int main() {
    std::ios_base::sync_with_stdio(false);

    std::vector<i64> ops;
    for(i64 a; !std::cin.eof();) {
        std::cin >> a;
        ops.push_back(a);
        std::cin.clear();
        std::cin.get();
    }

    for(i64 i = 0;;) {
        switch(ops[i]) {
            case 1: {
                i64 a1 = ops[i + 1];
                i64 a2 = ops[i + 2];
                i64 r = ops[i + 3];
                ops[r] = ops[a1] + ops[a2];
                i += 4;
            } break;
            case 2: {
                i64 a1 = ops[i + 1];
                i64 a2 = ops[i + 2];
                i64 r = ops[i + 3];
                ops[r] = ops[a1] * ops[a2];
                i += 4;
            } break;
            case 99:
                goto loop_end;
            default:
                std::cout << "unknown op at " << i << " (" << ops[i] << ")\n";
                goto loop_end;
        }
    }

    loop_end:
    for(i64 a : ops) {
        std::cout << a << ' ';
    }

    return 0;
}
