#include <iostream>
#include <vector>
#include <deque>

using i64 = long long;

i64 max(i64 a, i64 b) {
    return a > b ? a : b;
}

class Fake_Input_Stream {
public:
    template<typename... Vals>
    Fake_Input_Stream(Vals... vals): values{vals...} {}

    i64 read() {
        i64 const v = values.front();
        values.pop_front();
        return v;
    }

    void write(i64 v) {
        values.push_back(v);
    }

private:
    std::deque<i64> values;
};

std::vector<i64> run_intcode_program(std::vector<i64> ops, Fake_Input_Stream& input) {
    auto mem_dump = [&ops]() {
        // std::cout << "\nmem dump: ";
        // for(i64 a : ops) {
        //     std::cout << a << ' ';
        // }
        // std::cout << '\n';
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

    std::vector<i64> output;

    for(i64 i = 0;;) {
        i64 mode1 = ops[i] / 100 % 10;
        i64 mode2 = ops[i] / 1000 % 10;
        i64 mode3 = ops[i] / 10000 % 10;
        switch(ops[i] % 100) {
            case 1: {
                i64 a1 = value(mode1, ops[i + 1]);
                i64 a2 = value(mode2, ops[i + 2]);
                i64 r = ops[i + 3];
                // std::cout << "op 1 (" << ops[i] << ") at " << i << '\n' 
                //           << "1 " << ops[i + 1] << " " << ops[i + 2] << " " << ops[i + 3] 
                //           << " ; ops[" << r << "] = " << a1 << " + " << a2 << '\n';
                ops[r] = a1 + a2;
                i += 4;
            } break;

            case 2: {
                i64 a1 = value(mode1, ops[i + 1]);
                i64 a2 = value(mode2, ops[i + 2]);
                i64 r = ops[i + 3];
                // std::cout << "op 2 (" << ops[i] << ") at " << i << '\n' 
                //           << "2 " << ops[i + 1] << " " << ops[i + 2] << " " << ops[i + 3] 
                //           << " ; ops[" << r << "] = " << a1 << " * " << a2 << '\n';
                ops[r] = a1 * a2;
                i += 4;
            } break;

            case 3: {
                ops[ops[i + 1]] = input.read();
                i += 2;
            } break;

            case 4: {
                i64 const val = value(mode1, ops[i + 1]);
                // std::cout << "op 4 (" << ops[i] << ") at " << i << '\n'
                //           << "4 " << ops[i + 1] << " ; " << val << '\n';
                output.push_back(val);
                i += 2;
            } break;

            case 5: {
                i64 a1 = value(mode1, ops[i + 1]);
                i64 a2 = value(mode2, ops[i + 2]);
                // std::cout << "op 5 (" << ops[i] << ") at " << i << '\n' 
                //             << "5 " << ops[i + 1] << " " << ops[i + 2]
                //             << " ; i = (" << a1 << " != 0 ? " << a2 << " : " << i + 3 << ")\n";
                if(a1 != 0) {
                    i = a2;
                } else {
                    i += 3;
                }
            } break;

            case 6: {
                i64 a1 = value(mode1, ops[i + 1]);
                i64 a2 = value(mode2, ops[i + 2]);
                // std::cout << "op 6 (" << ops[i] << ") at " << i << '\n' 
                //             << "6 " << ops[i + 1] << " " << ops[i + 2]
                //             << " ; i = (" << a1 << " == 0 ? " << a2 << " : " << i + 3 << ")\n";
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
                // std::cout << "op 7 (" << ops[i] << ") at " << i << '\n' 
                //             << "7 " << ops[i + 1] << " " << ops[i + 2] << " " << ops[i + 3]
                //             << " ; ops[" << r << "] = " << a1 << " < " << a2 << '\n';
                ops[r] = (a1 < a2 ? 1 : 0);
                i += 4;
            } break;

            case 8: {
                i64 a1 = value(mode1, ops[i + 1]);
                i64 a2 = value(mode2, ops[i + 2]);
                i64 r = ops[i + 3];
                // std::cout << "op 8 (" << ops[i] << ") at " << i << '\n' 
                //             << "8 " << ops[i + 1] << " " << ops[i + 2] << " " << ops[i + 3]
                //             << " ; ops[" << r << "] = " << a1 << " == " << a2 << '\n';
                ops[r] = (a1 == a2 ? 1 : 0);
                i += 4;
            } break;

            case 99:
                // std::cout << "op 99 ; halting\n";
                mem_dump();
                return output;

            default:
                std::cout << "unknown op (" << ops[i] << ") at " << i << "\n";
                mem_dump();
                throw -1;
        }
    }

    return output;
}

i64 run_amps(std::vector<i64> ops, i64 a, i64 b, i64 c, i64 d, i64 e) {
    Fake_Input_Stream input(a, 0);
    std::vector<i64> out_a = run_intcode_program(ops, input);
    input.write(b);
    input.write(out_a.at(0));
    std::vector<i64> out_b = run_intcode_program(ops, input);
    input.write(c);
    input.write(out_b.at(0));
    std::vector<i64> out_c = run_intcode_program(ops, input);
    input.write(d);
    input.write(out_c.at(0));
    std::vector<i64> out_d = run_intcode_program(ops, input);
    input.write(e);
    input.write(out_d.at(0));
    std::vector<i64> out_e = run_intcode_program(ops, input);
    return out_e.at(0);
}

bool is_unique_amp_setting(i64 a, i64 b, i64 c, i64 d, i64 e) {
    i64 vals[5] = {};
    ++vals[a];
    ++vals[b];
    ++vals[c];
    ++vals[d];
    ++vals[e];
    return vals[a] == 1 && vals[b] == 1 && vals[c] == 1 && vals[d] == 1 && vals[e] == 1;
}

int main() {
    std::ios_base::sync_with_stdio(false);

    std::vector<i64> ops;
    for(i64 a; !std::cin.eof();) {
        std::cin >> a;
        ops.push_back(a);
        std::cin.clear();
        std::cin.get();
    }

    try {
        i64 max_thrusters = 0;
        for(i64 a = 0; a <= 4; ++a) {
            for(i64 b = 0; b <= 4; ++b) {
                for(i64 c = 0; c <= 4; ++c) {
                    for(i64 d = 0; d <= 4; ++d) {
                        for(i64 e = 0; e <= 4; ++e) {
                            if(is_unique_amp_setting(a, b, c, d, e)) {
                                i64 amps_out = run_amps(ops, a, b, c, d, e);
                                max_thrusters = max(max_thrusters, amps_out);
                            }
                        }
                    }
                }
            }
        }
        std::cout << max_thrusters;
    } catch(...) {}

    return 0;
}
