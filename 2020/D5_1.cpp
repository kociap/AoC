#include <iostream>
#include <vector>
#include <string>

using i64 = long long;

struct Seat {
    i64 row;
    i64 column;
};

Seat decode_pass(std::string const& pass) {
    i64 row_l = 0;
    i64 row_h = 127;
    for(i64 i = 0; i < 7; ++i) {
        if(pass[i] == 'F') {
            row_h = row_l + (row_h - row_l) / 2;
        } else {
            row_l = row_l + (row_h - row_l + 1) / 2;
        }
    }

    i64 col_l = 0;
    i64 col_h = 7;
    for(i64 i = 0; i < 3; ++i) {
        if(pass[7 + i] == 'L') {
            col_h = col_l + (col_h - col_l) / 2;
        } else {
            col_l = col_l + (col_h - col_l + 1) / 2;
        }
    }

    return Seat{row_l, col_l};
}

int main() {
    std::vector<std::string> boarding_passes;
    for(std::string pass;;) {
        std::cin >> pass;
        if(!std::cin) {
            break;
        }
        boarding_passes.push_back(pass);
    }

    i64 max_id = 0;
    for(std::string const& pass: boarding_passes) {
        Seat const s = decode_pass(pass);
        i64 const id = s.row * 8 + s.column;
        if(id > max_id) {
            max_id = id;
        }
    }

    std::cout << max_id;

    return 0;
}
