#include <iostream>
#include <vector>
#include <deque>
#include <string.h>

using i64 = long long;

i64 max(i64 a, i64 b) {
	return a > b ? a : b;
}

class Fake_Input_Stream {
public:
	template<typename... Vals>
	Fake_Input_Stream(Vals... vals) : values{ vals... } {}

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
	Intcode_Exec(std::vector<i64> const& intcode) : ops(intcode), i(0) {}

	// returns code
	// 0 - halted
	// 1 - waiting for input
	i64 run(Fake_Input_Stream& input, Fake_Input_Stream& output) {
		auto mem_dump = [this]() {
			// std::cout << "\nmem dump: ";
			// for(i64 a : ops) {
			//     std::cout << a << ' ';
			// }
			// std::cout << '\n';
		};

		auto value = [this, &mem_dump](i64 mode, i64 val) {
			switch (mode) {
			case 0:
				return ops[val];
			case 1:
				return val;
			default:
				std::cout << "unknown access mode " << mode << '\n';
				mem_dump();
				throw - 1;
			}
		};

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
				if (input.eoi()) {
					return 1;
				}

				ops[ops[i + 1]] = input.read();
				i += 2;
			} break;

			case 4: {
				i64 const val = value(mode1, ops[i + 1]);
				// std::cout << "op 4 (" << ops[i] << ") at " << i << '\n'
				//           << "4 " << ops[i + 1] << " ; " << val << '\n';
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
				return 0;

			default:
				std::cout << "unknown op (" << ops[i] << ") at " << i << "\n";
				mem_dump();
				throw - 1;
			}
		}

		std::cout << "program terminated unexpectedly\n";
		throw - 1;
	}

private:
	std::vector<i64> ops;
    i64 i;
};

i64 run_amps(std::vector<i64> ops, i64 a, i64 b, i64 c, i64 d, i64 e) {
	Intcode_Exec amps[5] = { ops, ops, ops, ops, ops };
    i64 settings[5] = {a, b, c, d, e};
	Fake_Input_Stream stream1;
	Fake_Input_Stream stream2(0);

    i64 return_code = -1;
    for(i64 i = 0; i < 5; ++i) {
        stream1.write(settings[i]);
        stream1.write(stream2.read());
        return_code = amps[i].run(stream1, stream2);
    }

    if (return_code == 0) {
        return stream2.read();
    }

	while (true) {
        i64 return_code = -1;
        for(i64 i = 0; i < 5; ++i) {
            stream1.write(stream2.read());
            return_code = amps[i].run(stream1, stream2);
        }

		if (return_code == 0) {
			return stream2.read();
		}
	}
}

bool has_no_duplicates(i64 a, i64 b, i64 c, i64 d, i64 e) {
    i64 vals[5] = {};
	++vals[a - 5];
	++vals[b - 5];
	++vals[c - 5];
	++vals[d - 5];
	++vals[e - 5];
	return vals[0] == 1 && vals[1] == 1 && vals[2] == 1 && vals[3] == 1 && vals[4] == 1;
}

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
		i64 max_thrusters = 0;
		for (i64 a = 5; a <= 9; ++a) {
			for (i64 b = 5; b <= 9; ++b) {
				for (i64 c = 5; c <= 9; ++c) {
					for (i64 d = 5; d <= 9; ++d) {
						for (i64 e = 5; e <= 9; ++e) {
							if (has_no_duplicates(a, b, c, d, e)) {
								i64 amps_out = run_amps(ops, a, b, c, d, e);
                                if(max_thrusters < amps_out) {
                                    std::cout << amps_out << ' ' << a << ' ' << b << ' ' << c << ' ' << d << ' ' << e << '\n';
                                }
								max_thrusters = max(max_thrusters, amps_out);
							}
						}
					}
				}
			}
		}
		std::cout << max_thrusters;
	}
	catch (...) {}

	return 0;
}
