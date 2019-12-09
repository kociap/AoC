#include <iostream>
#include <vector>
#include <deque>
#include <string.h>

using i64 = long long;

template<typename T>
class Vector: public std::vector<T> {
    using base = std::vector<T>;

public:
    using base::base;

    Vector(base const& b): base(b) {}

    T& operator[](std::size_t index) {
        if(index > base::size()) {
            std::cout << "out of bounds access " << index << "\n";
            if(index > 300000) {
                throw -1;
            }

            i64 min_size = base::size();
            while(min_size < index) {
                min_size *= 2;
            }

            std::cout << "resizing to " << min_size << '\n';
            base::resize(min_size, 0);
        }

        return base::operator[](index);
    }
};

class Stream {
public:
	template<typename... Vals>
	Stream(Vals... vals) : values{ vals... } {}

	// end of input
	bool eoi() {
		return values.size() == 0;
	}

	i64 read() {
		if (values.size() == 0) {
			std::cout << "\ncan't read from empty stream\n";
			throw - 1;
		}

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

class Intcode_Exec {
public:
	Intcode_Exec(std::vector<i64> const& intcode) : ops(intcode), i(0), relative_base(0) {}

	// returns code
	// 0 - halted
	// 1 - waiting for input
	i64 run(Stream& input, Stream& output) {
		auto mem_dump = [this]() {
			std::cout << "\nmem dump: ";
			for(i64 a : ops) {
			    std::cout << a << ' ';
			}
			std::cout << '\n';
		};

        auto value = [this, &mem_dump](i64 mode, i64 offset) -> i64 {
			switch (mode) {
			case 0:
				return ops[offset];
			case 1:
				return offset;
            case 2:
                return ops[relative_base + offset];
			default:
				std::cout << "unknown access mode " << mode << '\n';
				mem_dump();
				throw - 1;
			}
		};

        auto write = [this, &mem_dump](i64 mode, i64 offset, i64 value) -> void {
			switch (mode) {
			case 0:
				ops[offset] = value;
                break;
            case 2:
                ops[relative_base + offset] = value;
                break;
			default:
				std::cout << "unknown write mode " << mode << '\n';
				mem_dump();
				throw - 1;
			}
        };

        try {
            while (true) {
                i64 mode1 = ops[i] / 100 % 10;
                i64 mode2 = ops[i] / 1000 % 10;
                i64 mode3 = ops[i] / 10000 % 10;
                switch (ops[i] % 100) {
                case 1: {
                    i64 a1 = value(mode1, ops[i + 1]);
                    i64 a2 = value(mode2, ops[i + 2]);
                    i64 r = ops[i + 3];
                    // std::cout << "op 1 (" << ops[i] << ") at " << i << '\n' 
                    //         << "1 " << ops[i + 1] << " " << ops[i + 2] << " " << ops[i + 3] 
                    //         << " ; ops[" << r << "] = " << a1 << " + " << a2 << '\n';
                    write(mode3, r, a1 + a2);
                    i += 4;
                } break;

                case 2: {
                    i64 a1 = value(mode1, ops[i + 1]);
                    i64 a2 = value(mode2, ops[i + 2]);
                    i64 r = ops[i + 3];
                    // std::cout << "op 2 (" << ops[i] << ") at " << i << '\n' 
                    //         << "2 " << ops[i + 1] << " " << ops[i + 2] << " " << ops[i + 3] 
                    //         << " ; ops[" << r << "] = " << a1 << " * " << a2 << " (" << (a1 * a2) << ")\n";
                    write(mode3, r, a1 * a2);
                    i += 4;
                } break;

                case 3: {
                    if (input.eoi()) {
                        return 1;
                    }

                    i64 input_value = input.read();
                    write(mode1, ops[i + 1], input_value);
                    i += 2;
                } break;

                case 4: {
                    i64 const val = value(mode1, ops[i + 1]);
                    // std::cout << "op 4 (" << ops[i] << ") at " << i << '\n'
                    //         << "4 " << ops[i + 1] << " ; " << val << '\n';
                    output.write(val);
                    i += 2;
                } break;

                case 5: {
                    i64 a1 = value(mode1, ops[i + 1]);
                    i64 a2 = value(mode2, ops[i + 2]);
                    // std::cout << "op 5 (" << ops[i] << ") at " << i << '\n' 
                    //             << "5 " << ops[i + 1] << " " << ops[i + 2]
                    //             << " ; i = (" << a1 << " != 0 ? " << a2 << " : " << i + 3 << ")\n";
                    if (a1 != 0) {
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
                    if (a1 == 0) {
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
                    write(mode3, r, (a1 < a2 ? 1 : 0));
                    i += 4;
                } break;

                case 8: {
                    i64 a1 = value(mode1, ops[i + 1]);
                    i64 a2 = value(mode2, ops[i + 2]);
                    i64 r = ops[i + 3];
                    // std::cout << "op 8 (" << ops[i] << ") at " << i << '\n' 
                    //             << "8 " << ops[i + 1] << " " << ops[i + 2] << " " << ops[i + 3]
                    //             << " ; ops[" << r << "] = " << a1 << " == " << a2 << '\n';
                    write(mode3, r, (a1 == a2 ? 1 : 0));
                    i += 4;
                } break;

                case 9: {
                    i64 a1 = value(mode1, ops[i + 1]);
                    // std::cout << "op 9 (" << ops[i] << ") at " << i << '\n'
                    //         << "9 " << ops[i + 1] << " "
                    //         << " ; relative_base = " << relative_base << " + " << a1 << '\n';
                    relative_base += a1;
                    i += 2;
                } break;

                case 99:
                    // std::cout << "op 99 ; halting\n";
                    mem_dump();
                    return 0;

                default:
                    std::cout << "unknown op (" << ops[i] << ") at " << i << "\n";
                    throw - 1;
                }
            }
        } catch(...) {
            std::cout << "relative_base " << relative_base << " offset " << i << " op " << ops[i];
            mem_dump();
            throw -1;
        }

		std::cout << "program terminated unexpectedly\n";
		throw - 1;
	}

private:
	Vector<i64> ops;
    i64 i;
    i64 relative_base;

    i64 exec(Stream& input, Stream& output) {
        while (true) {
            i64 mode1 = ops[i] / 100 % 10;
            i64 mode2 = ops[i] / 1000 % 10;
            i64 mode3 = ops[i] / 10000 % 10;
            switch (ops[i] % 100) {
            case 1: {
                i64 a1 = value(mode1, ops[i + 1]);
                i64 a2 = value(mode2, ops[i + 2]);
                i64 r = ops[i + 3];
                // std::cout << "op 1 (" << ops[i] << ") at " << i << '\n' 
                //         << "1 " << ops[i + 1] << " " << ops[i + 2] << " " << ops[i + 3] 
                //         << " ; ops[" << r << "] = " << a1 << " + " << a2 << '\n';
                write(mode3, r, a1 + a2);
                i += 4;
            } break;

            case 2: {
                i64 a1 = value(mode1, ops[i + 1]);
                i64 a2 = value(mode2, ops[i + 2]);
                i64 r = ops[i + 3];
                // std::cout << "op 2 (" << ops[i] << ") at " << i << '\n' 
                //         << "2 " << ops[i + 1] << " " << ops[i + 2] << " " << ops[i + 3] 
                //         << " ; ops[" << r << "] = " << a1 << " * " << a2 << " (" << (a1 * a2) << ")\n";
                write(mode3, r, a1 * a2);
                i += 4;
            } break;

            case 3: {
                if (input.eoi()) {
                    return 1;
                }

                i64 input_value = input.read();
                write(mode1, ops[i + 1], input_value);
                i += 2;
            } break;

            case 4: {
                i64 const val = value(mode1, ops[i + 1]);
                // std::cout << "op 4 (" << ops[i] << ") at " << i << '\n'
                //         << "4 " << ops[i + 1] << " ; " << val << '\n';
                output.write(val);
                i += 2;
            } break;

            case 5: {
                i64 a1 = value(mode1, ops[i + 1]);
                i64 a2 = value(mode2, ops[i + 2]);
                // std::cout << "op 5 (" << ops[i] << ") at " << i << '\n' 
                //             << "5 " << ops[i + 1] << " " << ops[i + 2]
                //             << " ; i = (" << a1 << " != 0 ? " << a2 << " : " << i + 3 << ")\n";
                if (a1 != 0) {
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
                if (a1 == 0) {
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
                write(mode3, r, (a1 < a2 ? 1 : 0));
                i += 4;
            } break;

            case 8: {
                i64 a1 = value(mode1, ops[i + 1]);
                i64 a2 = value(mode2, ops[i + 2]);
                i64 r = ops[i + 3];
                // std::cout << "op 8 (" << ops[i] << ") at " << i << '\n' 
                //             << "8 " << ops[i + 1] << " " << ops[i + 2] << " " << ops[i + 3]
                //             << " ; ops[" << r << "] = " << a1 << " == " << a2 << '\n';
                write(mode3, r, (a1 == a2 ? 1 : 0));
                i += 4;
            } break;

            case 9: {
                i64 a1 = value(mode1, ops[i + 1]);
                // std::cout << "op 9 (" << ops[i] << ") at " << i << '\n'
                //         << "9 " << ops[i + 1] << " "
                //         << " ; relative_base = " << relative_base << " + " << a1 << '\n';
                relative_base += a1;
                i += 2;
            } break;

            case 99:
                // std::cout << "op 99 ; halting\n";
                mem_dump();
                return 0;

            default:
                std::cout << "unknown op (" << ops[i] << ") at " << i << "\n";
                throw - 1;
            }
        }
    }
};

int main() {
	std::ios_base::sync_with_stdio(false);

	std::vector<i64> ops;
	for (i64 a; !std::cin.eof();) {
		std::cin >> a;
		ops.push_back(a);
		std::cin.clear();
		std::cin.get();
	}

	try {
		Intcode_Exec exec(ops);
        Stream in(2), out;
        exec.run(in, out);
        while(!out.eoi()) {
            std::cout << out.read() << ' ';
        }
	}
	catch (...) {}

	return 0;
}
