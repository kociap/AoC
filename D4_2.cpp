#include <cmath>
#include <iostream>
#include <vector>
#include <string>

using i64 = long long;

struct Passport {
    std::string byr;
    std::string iyr;
    std::string eyr;
    std::string hgt;
    std::string hcl;
    std::string ecl;
    std::string pid;
    std::string cid;
};

// characters will be appended to out
// skips ' ', '\n' and ':'
// returns -1 on eof, 0 otherwise
i64 read(std::string& out) {
    bool eof = false;
    while(true) {
        char const c = std::cin.peek();
        if(c == EOF) {
            eof = true;
            break;
        } else if(c == ' ' || c == ':' || c == '\n') {
            std::cin.get();
        } else {
            break;
        }
    }

    if(eof) {
        return -1;
    }

    while(true) {
        char const c = std::cin.peek();
        if(c == ' ' || c == ':' || c == '\n') {
            break;
        }

        out.push_back(c);
        std::cin.get();
    }

    return 0;
}

int main() {
    std::vector<Passport> passports;
    for(std::string key, val; true;) {
        Passport passport;
        bool push_passport = true;
        while(true) {
            i64 const res1 = read(key);
            i64 const res2 = read(val);
            if(res1 == -1) {
                push_passport = false;
                break;
            }

            if(key == "byr") {
                passport.byr = val;
            } else if(key == "iyr") {
                passport.iyr = val;
            } else if(key == "eyr") {
                passport.eyr = val;
            } else if(key == "hgt") {
                passport.hgt = val;
            } else if(key == "hcl") {
                passport.hcl = val;
            } else if(key == "ecl") {
                passport.ecl = val;
            } else if(key == "pid") {
                passport.pid = val;
            } else if(key == "cid") {
                passport.cid = val;
            }

            key.clear();
            val.clear();

            if(std::cin.peek() == '\n') {
                std::cin.get();
                if(std::cin.peek() == '\n') {
                    break;
                }
            }
        }

        if(push_passport) {
            passports.push_back(std::move(passport));
        } else {
            break;
        }
    }

    i64 valid_passports = 0;
    for(Passport const& p: passports) {
        bool init_valid = p.byr.size() != 0 && p.iyr.size() != 0 
                          && p.eyr.size() != 0 && p.hgt.size() != 0 
                          && p.hcl.size() != 0 && p.ecl.size() != 0 
                          && p.pid.size() != 0;
        if(init_valid) {
            i64 const byr = std::stoi(p.byr);
            if(byr < 1920 || byr > 2020) {
                continue;
            }

            i64 const iyr = std::stoi(p.iyr);
            if(iyr < 2010 || iyr > 2020) {
                continue;
            }

            i64 const eyr = std::stoi(p.eyr);
            if(eyr < 2020 || eyr > 2030) {
                continue;
            }

            if(p.hgt.size() < 4) {
                continue;
            }

            if(p.hgt[p.hgt.size() - 1] == 'm' && p.hgt[p.hgt.size() - 2] == 'c') {
                if(p.hgt.size() != 5) {
                    continue;
                }

                i64 h = (p.hgt[0] - '0') * 100 + (p.hgt[1] - '0') * 10 + (p.hgt[2] - '0');
                if(h < 150 || h > 193) {
                    continue;
                }
            } else if(p.hgt[p.hgt.size() - 1] == 'n' && p.hgt[p.hgt.size() - 2] == 'i') {
                if(p.hgt.size() != 4) {
                    continue;
                }

                i64 h = (p.hgt[0] - '0') * 10 + (p.hgt[1] - '0');
                if(h < 59 || h > 76) {
                    continue;
                }
            } else {
                continue;
            }

            if(p.hcl.size() != 7) {
                continue;
            }

            if(p.hcl[0] != '#') {
                continue;
            }

            bool invalid_hcl = false;
            for(i64 i = 1; i < 7; ++i) {
                if((p.hcl[i] < '0' || p.hcl[i] > '9') && (p.hcl[i] < 'a' || p.hcl[i] > 'f')) {
                    invalid_hcl = true;
                }
            }

            if(invalid_hcl) {
                continue;
            }

            if(!(p.ecl == "amb" || p.ecl == "blu" || p.ecl == "brn" 
                 || p.ecl == "gry" || p.ecl == "grn" || p.ecl == "hzl" 
                 || p.ecl == "oth")) {
                continue;
            }

            if(p.pid.size() != 9) {
                continue;
            }

            bool invalid_pid = false;
            for(char c: p.pid) {
                if(c < '0' || c > '9') {
                    invalid_pid = true;
                }
            }

            if(invalid_pid) {
                break;
            }

            valid_passports += 1;
        }
    }

    std::cout << valid_passports;

    return 0;
}
