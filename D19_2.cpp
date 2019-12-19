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
					write(mode3, r, a1 + a2);
					i += 4;
				} break;

				case 2: {
					i64 a1 = value(mode1, ops[i + 1]);
					i64 a2 = value(mode2, ops[i + 2]);
					i64 r = ops[i + 3];
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
					output.write(val);
					i += 2;
				} break;

				case 5: {
					i64 a1 = value(mode1, ops[i + 1]);
					i64 a2 = value(mode2, ops[i + 2]);
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
					write(mode3, r, (a1 < a2 ? 1 : 0));
					i += 4;
				} break;

				case 8: {
					i64 a1 = value(mode1, ops[i + 1]);
					i64 a2 = value(mode2, ops[i + 2]);
					i64 r = ops[i + 3];
					write(mode3, r, (a1 == a2 ? 1 : 0));
					i += 4;
				} break;

				case 9: {
					i64 a1 = value(mode1, ops[i + 1]);
					relative_base += a1;
					i += 2;
				} break;

				case 99:
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

    void reset(std::vector<i64> const& program) {
        i = 0;
        relative_base = 0;
        ops.reserve(program.size());
        memcpy(ops.data(), program.data(), program.size() * sizeof(i64));
    }

private:
	Vector<i64> ops;
	i64 i;
	i64 relative_base;
};

class Vec2 {
public:
    i64 x;
    i64 y;

    void operator+=(Vec2 v) {
        x += v.x;
        y += v.y;
    }
};

Vec2 operator+(Vec2 v1, Vec2 v2) {
    return {v1.x + v2.x, v1.y + v2.y};
}

Vec2 find_box_within_tractor(std::vector<i64> const& ops, i64 starting_y, i64 const box_width, i64 const box_height) {
    Intcode_Exec exec(ops);
    Stream in, out;

    auto check_point = [&exec, &ops, &in, &out](Vec2 p) -> i64 {
        in.write(p.x);
        in.write(p.y);
        exec.reset(ops);
        exec.run(in, out);
        return out.read();
    };

	try {
        Vec2 up = {0, starting_y};
        while(check_point(up) != 1) {
            up += {1, 0};
        }

        Vec2 bottom = {0, starting_y + box_height - 1};
        while(check_point(bottom) != 1) {
            bottom += {1, 0};
        }

        while(true) {
            // Assumes 2 adjacent diagonals may not end in the same row

            i64 bottom_check = check_point(bottom + Vec2{1, 1});
            if(bottom_check) {
                bottom += Vec2{1, 1};
            } else {
                bottom += Vec2{2, 1};
            }

            i64 up_check = check_point(up + Vec2{1, 1});
            if(up_check) {
                up += Vec2{1, 1};
            } else {
                up += Vec2{0, 1};
            }

            i64 bottom_expand = check_point(bottom + Vec2{-1, 0});
            if(bottom_expand) {
                bottom += Vec2{-1, 0};
            }

            if(up.x - bottom.x == box_width - 1) {
                return {bottom.x, up.y};
            }

            i64 up_expand = check_point(up + Vec2{1, 0});
            if(up_expand) {
                up += Vec2{1, 0};
            }

            if(up.x - bottom.x == box_width - 1) {
                return {bottom.x, up.y};
            }
        }
	} catch (std::string const& msg) {
		std::cout << msg;
		exec.mem_dump();
        return {-1, -1};
	}

    throw std::string("No return");
}

i64 min(i64 a, i64 b) {
    return a < b ? a : b;
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

    Vec2 point = find_box_within_tractor(ops, 5, 100, 100);
    std::cout << point.x << ' ' << point.y << ' ' << (point.x * 10000 + point.y) << '\n';
	return 0;
}
