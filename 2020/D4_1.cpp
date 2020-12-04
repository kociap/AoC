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
        bool valid = p.byr.size() != 0 && p.iyr.size() != 0 
                     && p.eyr.size() != 0 && p.hgt.size() != 0 
                     && p.hcl.size() != 0 && p.ecl.size() != 0 
                     && p.pid.size() != 0;
        valid_passports += valid;
    }

    std::cout << valid_passports;

    return 0;
}
