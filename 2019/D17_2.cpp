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


class Map {
public:
    Map(std::vector<char> m, i64 w, i64 h): _map(m), _width(w), _height(h) {}

    char read(i64 x, i64 y) const {
        if(x < 0 || x >= _width || y < 0 || y >= _height) {
            return '.';
        } else {
            return _map[y * _width + x];
        }
    }

    i64 width() const {
        return _width;
    }

    i64 height() const {
        return _height;
    }

private:
    std::vector<char> _map;
    i64 _width;
    i64 _height;
};


// -1 - turn right
// -2 - turn left
// x > 0 - move x tiles forward
std::vector<i64> trace_path(Map const& map, i64 bot_x, i64 bot_y, i64 orientation) {
    std::vector<i64> path;
    while(true) {
        // std::cout << bot_x << ' ' << bot_y << "; orientation: " << orientation << '\n';
        i64 next_x = bot_x;
        i64 next_y = bot_y;
        switch(orientation) {
            case 0:
                next_y -= 1;
                break;
            case 2:
                next_y += 1;
                break;
            case 1:
                next_x += 1;
                break;
            case 3:
                next_x -= 1;
                break;
        }

        if(map.read(next_x, next_y) == '#') {
            bot_x = next_x;
            bot_y = next_y;
            if(path.size() > 0 && path.back() > 0) {
                path.back() += 1;
            } else {
                path.push_back(1);
            }
        } else {
            switch(orientation) {
                case 0:
                case 2: {
                    if(map.read(bot_x - 1, bot_y) == '#') {
                        i64 o = (orientation == 0 ? -2 : -1);
                        path.push_back(o);
                        orientation = 3;
                        break;
                    }

                    if(map.read(bot_x + 1, bot_y) == '#') {
                        i64 o = (orientation == 0 ? -1 : -2);
                        path.push_back(o);
                        orientation = 1;
                        break;
                    }

                    std::cout << "reached end\n" << std::flush;
                    return path;
                }

                case 1:
                case 3: {
                    if(map.read(bot_x, bot_y - 1) == '#') {
                        i64 o = (orientation == 1 ? -2 : -1);
                        path.push_back(o);
                        orientation = 0;
                        break;
                    }

                    if(map.read(bot_x, bot_y + 1) == '#') {
                        i64 o = (orientation == 1 ? -1 : -2);
                        path.push_back(o);
                        orientation = 2;
                        break;
                    }

                    std::cout << "reached end\n" << std::flush;
                    return path;
                }
            }
        }
    }
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

	Intcode_Exec exec(ops);
	Stream in, out;
	try {
		i64 r = exec.run(in, out);
        std::vector<char> map;
        // Intcode outputs height + 1 newlines
        i64 height = -1;
        i64 width = 0;
        while(!out.eoi()) {
            char o = out.read();
            if(o == '\n') {
                height += 1;
            } else {
                map.push_back(o);
            }
            std::cout << o;
        }
        width = map.size() / height;

        Map m(map, width, height);
        i64 bot_x;
        i64 bot_y;
        i64 orientation;
        for(i64 i = 0; i < map.size(); ++i) {
            if(map[i] == '^' || map[i] == 'v' || map[i] == '>' || map[i] == '<') {
                bot_x = i % width;
                bot_y = i / width;
                switch(map[i]) {
                    case '^':
                        orientation = 0;
                        break;
                    case 'v': 
                        orientation = 2;
                        break;
                    case '>':
                        orientation = 1;
                        break;
                    case '<':
                        orientation = 3;
                        break;
                }
                break;
            }
        }

        std::vector<i64> path = trace_path(m, bot_x, bot_y, orientation);
        for(i64 p: path) {
            if(p == -1) {
                std::cout << "R ";
            } else if(p == -2) {
                std::cout << "L ";
            } else {
                std::cout << p << ' ';
            }
        }
		
		// TODO: Implement compression algorithm to solve the problem automatically

	} catch (std::string const& msg) {
		std::cout << msg;
		exec.mem_dump();
	}

	return 0;
}
