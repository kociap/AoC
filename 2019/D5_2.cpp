#include <iostream>
#include <vector>

using i64 = long long;

class Fake_Input_Stream {
public:
    // In reverse order
    template<typename... Vals>
    Fake_Input_Stream(Vals... vals): values{vals...} {}

    i64 read() {
        i64 const v = values.back();
        values.pop_back();
        return v;
    }

private:
    std::vector<i64> values;
};

int main() {
    std::ios_base::sync_with_stdio(false);

    std::vector<i64> ops;
    for(i64 a; !std::cin.eof();) {
        std::cin >> a;
        ops.push_back(a);
        std::cin.clear();
        std::cin.get();
    }

    Fake_Input_Stream input(5);

    auto mem_dump = [&ops]() {
        std::cout << "\nmem dump: ";
        for(i64 a : ops) {
            std::cout << a << ' ';
        }
        std::cout << '\n';
    };

    auto value = [&](i64 mode, i64 val) {
        switch(mode) {
            case 0:
                return ops[val];
            case 1:
                return val;
            default:
                std::cout << "unknown access mode " << mode << '\n';
                mem_dump();
                throw -1;
        }
    };

    try {
        for(i64 i = 0;;) {
            i64 mode1 = ops[i] / 100 % 10;
            i64 mode2 = ops[i] / 1000 % 10;
            i64 mode3 = ops[i] / 10000 % 10;
            switch(ops[i] % 100) {
                case 1: {
                    std::cout << "op 1 (" << ops[i] << ") at " << i << '\n';
                    i64 a1 = value(mode1, ops[i + 1]);
                    i64 a2 = value(mode2, ops[i + 2]);
                    i64 r = ops[i + 3];
                    std::cout << "1 " << ops[i + 1] << " " << ops[i + 2] << " " << ops[i + 3] << " ; ops[" << r << "] = " << a1 << " + " << a2 << '\n';
                    ops[r] = a1 + a2;
                    i += 4;
                } break;

                case 2: {
                    std::cout << "op 2 (" << ops[i] << ") at " << i << '\n';
                    i64 a1 = value(mode1, ops[i + 1]);
                    i64 a2 = value(mode2, ops[i + 2]);
                    i64 r = ops[i + 3];
                    std::cout << "2 " << ops[i + 1] << " " << ops[i + 2] << " " << ops[i + 3] << " ; ops[" << r << "] = " << a1 << " * " << a2 << '\n';
                    ops[r] = a1 * a2;
                    i += 4;
                } break;

                case 3: {
                    ops[ops[i + 1]] = input.read();
                    i += 2;
                } break;

                case 4: {
                    std::cout << "op 4 (" << ops[i] << ") at " << i << '\n';
                    i64 const val = value(mode1, ops[i + 1]);
                    if(val != 0) {
                        std::cout << "4 " << ops[i + 1] << " ; " << val << '\n';
                    } else {
                        std::cout << "4 " << ops[i + 1] << " ; " << val << '\n';
                    }
                    i += 2;
                } break;

                case 5: {
                    i64 a1 = value(mode1, ops[i + 1]);
                    i64 a2 = value(mode2, ops[i + 2]);
                    std::cout << "op 5 (" << ops[i] << ") at " << i << '\n' 
                              << "5 " << ops[i + 1] << " " << ops[i + 2]
                              << " ; i = (" << a1 << " != 0 ? " << a2 << " : " << i + 3 << ")\n";
                    if(a1 != 0) {
                        i = a2;
                    } else {
                        i += 3;
                    }
                } break;

                case 6: {
                    i64 a1 = value(mode1, ops[i + 1]);
                    i64 a2 = value(mode2, ops[i + 2]);
                    std::cout << "op 6 (" << ops[i] << ") at " << i << '\n' 
                              << "6 " << ops[i + 1] << " " << ops[i + 2]
                              << " ; i = (" << a1 << " == 0 ? " << a2 << " : " << i + 3 << ")\n";
                    if(a1 == 0) {
                        i = a2;
                    } else {
                        i += 3;
                    }
                } break;

                case 7: {
                    i64 a1 = value(mode1, ops[i + 1]);
                    i64 a2 = value(mode2, ops[i + 2]);
                    i64 r = ops[i + 3];
                    std::cout << "op 7 (" << ops[i] << ") at " << i << '\n' 
                              << "7 " << ops[i + 1] << " " << ops[i + 2] << " " << ops[i + 3]
                              << " ; ops[" << r << "] = " << a1 << " < " << a2 << '\n';
                    ops[r] = (a1 < a2 ? 1 : 0);
                    i += 4;
                } break;

                case 8: {
                    i64 a1 = value(mode1, ops[i + 1]);
                    i64 a2 = value(mode2, ops[i + 2]);
                    i64 r = ops[i + 3];
                    std::cout << "op 8 (" << ops[i] << ") at " << i << '\n' 
                              << "8 " << ops[i + 1] << " " << ops[i + 2] << " " << ops[i + 3]
                              << " ; ops[" << r << "] = " << a1 << " == " << a2 << '\n';
                    ops[r] = (a1 == a2 ? 1 : 0);
                    i += 4;
                } break;

                case 99:
                    std::cout << "op 99 ; halting\n";
                    mem_dump();
                    throw -1;

                default:
                    std::cout << "unknown op (" << ops[i] << ") at " << i << "\n";
                    mem_dump();
                    throw -1;
            }
        }
    } catch(...) {}

    return 0;
}
