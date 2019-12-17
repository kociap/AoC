#include <deque>
#include <iostream>
#include <string.h>
#include <string>
#include <vector>

using i64 = long long;

template <typename T>
class Vector : public std::vector<T> {
	using base = std::vector<T>;

public:
	using base::base;
	Vector(base const& b) : base(b) {}

	T& operator[](std::size_t index) {
		if (index >= base::size()) {
			if (index > 300000) {
				throw "out of bounds access " + std::to_string(index) + "\n";
			}

			i64 min_size = base::size();
			while (min_size <= index) {
				min_size *= 2;
			}

			std::cout << "out of bounds access " << index << ". resizing to " << min_size << std::endl;
			base::resize(min_size, 0);
		}

		return base::operator[](index);
	}
};

class Stream {
public:
	template <typename... Vals>
	Stream(Vals... vals) : values{ vals... } {}

	// end of input
	bool eoi() {
		return values.size() == 0;
	}

	i64 read() {
		if (values.size() == 0) {
			throw std::string("can't read from empty stream\n");
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
		auto value = [this](i64 mode, i64 offset) -> i64 {
			switch (mode) {
			case 0:
				return ops[offset];
			case 1:
				return offset;
			case 2:
				return ops[relative_base + offset];
			default:
				throw "unknown access mode " + std::to_string(mode) + "\n";
			}
		};

		auto write = [this](i64 mode, i64 offset, i64 value) -> void {
			switch (mode) {
			case 0:
				ops[offset] = value;
				break;
			case 2:
				ops[relative_base + offset] = value;
				break;
			default:
				throw "unknown write mode " + std::to_string(mode) + "\n";
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
					}
					else {
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
					}
					else {
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
					return 0;

				default:
					throw "unknown op (" + std::to_string(ops[i]) + ") at " + std::to_string(i) + "\n";
				}
			}

			throw std::string("program terminated unexpectedly\n");

		} catch (std::string const& message) {
			std::string msg_with_program_info =
				message + "relative_base " + std::to_string(relative_base) + " offset " + std::to_string(i) + " op " + std::to_string(ops[i]);
			throw msg_with_program_info;
		}
	}

	void mem_dump() {
		std::cout << "\nmem dump: ";
		for (i64 a : ops) {
			std::cout << a << ' ';
		}
		std::cout << '\n';
	}

private:
	Vector<i64> ops;
	i64 i;
	i64 relative_base;
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

	Intcode_Exec exec(ops);
	Stream in, out;
	try {
		i64 r = exec.run(in, out);
        std::vector<char> map;
        while(!out.eoi()) {
            map.push_back(out.read());
        }

        i64 width = 0;
        for(i64 i = 0; i < map.size(); ++i) {
            if(map[i] == '\n') {
                width = i + 1;
                break;
            }
        }

        i64 height = map.size() / width;
        auto at = [width, &map](i64 x, i64 y) -> char& {
            return map[y * width + x];
        };

        i64 sum = 0;
        for(i64 y = 1; y < height - 1; ++y) {
            for(i64 x = 1; x < width - 1; ++x) {
                if(at(x, y) == '#' && at(x - 1, y) == '#' && at(x + 1, y) == '#' && at(x, y - 1) == '#' && at(x, y + 1) == '#') {
                    sum += x * y;
                }
            }
        }

        std::cout << sum;
	} catch (std::string const& msg) {
		std::cout << msg;
		exec.mem_dump();
	}


	return 0;
}
