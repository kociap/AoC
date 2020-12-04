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
		if (index > base::size()) {
			if (index > 300000) {
				throw "out of bounds access " + std::to_string(index) + "\n";
			}

			i64 min_size = base::size();
			while (min_size < index) {
				min_size *= 2;
			}

			std::cout << "out of bounds access " << index << ". resizing to " << min_size << '\n';
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

		}
		catch (std::string const& message) {
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

class Bot {
public:
    Bot(Intcode_Exec program): _program(program), _steps_counter(0) {}

    i64 go(i64 dir) {
        _in.write(dir);
        _program.run(_in, _out);
        i64 res = _out.read();
        if(res != 0) {
            ++_steps_counter;
        }
        return res;
    }

    i64 undo(i64 dir) {
        _in.write(dir);
        _program.run(_in, _out);
        i64 res = _out.read();
        if(res != 0) {
            --_steps_counter;
        }
        return res;
    }

    i64 get_steps() {
        return _steps_counter;
    }

private:
    Intcode_Exec _program;
    i64 _steps_counter;
    Stream _in;
    Stream _out;
};

struct Vec2 {
    i64 x;
    i64 y;
};

i64 steps_to_target = (~0ULL) >> 1;

i64 min(i64 a, i64 b) {
    return a < b ? a : b;
}

i64 max(i64 a, i64 b) {
    return a > b ? a : b;
}

class Map {
public:
    Map(i64 w, i64 h): _width(w), _height(h), _map(_width * _height, -1) {}

    void write(i64 x, i64 y, i64 v) {
        _map[y * _width + x] = v;
    }

    i64 read(i64 x, i64 y) {
        return _map[y * _width + x];
    }

    i64 width() const {
        return _width;
    }

    i64 height() const {
        return _height;
    }

    std::vector<i64>& get_storage() {
        return _map;
    }

    void print() {
        for(i64 y = 0; y < _height; ++y) {
            for(i64 x = 0; x < _width; ++x) {
                i64 const p = read(x, y);
                switch(p) {
                case -1:
                    std::cout << ' ';
                    break;
                case 0:
                    std::cout << '#';
                    break;
                case 1:
                    std::cout << '.';
                    break;
                default:
                    std::cout << 'O';
                    break;
                }
            }
            std::cout << '\n';
        }
    }

private:
    i64 _width;
    i64 _height;
    std::vector<i64> _map;
};

void try_go(Bot& bot, i64 const blocked_dir, Map& map, Vec2 pos) {
    if(blocked_dir != 1) {
        i64 res = bot.go(1);
        map.write(pos.x, pos.y - 1, res);

        if(res == 2) {
            steps_to_target = min(bot.get_steps(), steps_to_target);
        }

        if(res != 0) {
            try_go(bot, 2, map, {pos.x, pos.y - 1});
            bot.undo(2);
        }
    }

    if(blocked_dir != 2) {
        i64 res = bot.go(2);
        map.write(pos.x, pos.y + 1, res);

        if(res == 2) {
            steps_to_target = min(bot.get_steps(), steps_to_target);
        }

        if(res != 0) {
            try_go(bot, 1, map, {pos.x, pos.y + 1});
            bot.undo(1);
        }
    }

    if(blocked_dir != 3) {
        i64 res = bot.go(3);
        map.write(pos.x + 1, pos.y, res);

        if(res == 2) {
            steps_to_target = min(bot.get_steps(), steps_to_target);
        }

        if(res != 0) {
            try_go(bot, 4, map, {pos.x + 1, pos.y});
            bot.undo(4);
        }
    }

    if(blocked_dir != 4) {
        i64 res = bot.go(4);
        map.write(pos.x - 1, pos.y, res);

        if(res == 2) {
            steps_to_target = min(bot.get_steps(), steps_to_target);
        }

        if(res != 0) {
            try_go(bot, 3, map, {pos.x - 1, pos.y});
            bot.undo(3);
        }
    }
}

void ff(Map& map, Vec2 pos, i64 time) {
    i64 const p = map.read(pos.x, pos.y);
    if(p == 1 || (p > 2 && time < p)) {
        map.write(pos.x, pos.y, time);
        ff(map, {pos.x, pos.y - 1}, time + 1);
        ff(map, {pos.x, pos.y + 1}, time + 1);
        ff(map, {pos.x - 1, pos.y}, time + 1);
        ff(map, {pos.x + 1, pos.y}, time + 1);
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
    Bot bot(exec);

    Map map(100, 100);
    Vec2 pos = {50, 50};
	try {
        try_go(bot, 1, map, pos);
        try_go(bot, 2, map, pos);
        try_go(bot, 3, map, pos);
        try_go(bot, 4, map, pos);
        std::cout << steps_to_target << '\n';
	} catch (std::string const& msg) {
		std::cout << msg;
		exec.mem_dump();
	}

    // map.print();

    Map map_c(map);
    Vec2 ff_pos;
    std::vector<i64>& map_vec = map_c.get_storage();
    for(i64 i = 0; i < map_vec.size(); ++i) {
        if(map_vec[i] == 2) {
            ff_pos.x = i % map.width();
            ff_pos.y = i / map.width();
            map_vec[i] = 1;
            break;
        } 
    }

    // std::cout << ff_pos.x << ' ' << ff_pos.y << " - " << map.read(ff_pos.x, ff_pos.y) << '\n';

    ff(map_c, ff_pos, 10);
    // map_c.print();

    i64 max_time = 0;
    for(i64 i = 0; i < map_vec.size(); ++i) {
        max_time = max(max_time, map_vec[i]);
    }
    std::cout << (max_time - 10);

	return 0;
}
