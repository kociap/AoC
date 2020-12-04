#include <iostream>
#include <vector>
#include <string>

using i64 = long long;

struct Password {
    i64 lower_limit;
    i64 upper_limit;
    char character;
    std::string password;
};

int main() {
    std::vector<Password> passwords;
    while(true) {
        Password p;
        std::cin >> p.lower_limit;
        std::cin.get(); // discard -
        std::cin >> p.upper_limit;
        std::cin.get(); // discard space
        p.character = std::cin.get();
        std::cin.get(); // discard :
        std::cin >> p.password;
        if(!std::cin) {
            break;
        }
        passwords.push_back(std::move(p));
    }

    i64 valid_passwords = 0;
    for(Password const& p: passwords) {
        i64 char_count = 0;
        for(char c: p.password) {
            char_count += (c == p.character);
        }

        bool const is_valid = char_count >= p.lower_limit && char_count <= p.upper_limit;
        valid_passwords += is_valid;
    }

    std::cout << valid_passwords;

    return 0;
}
