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
        bool const is_valid = (p.password[p.lower_limit - 1] == p.character) ^ (p.password[p.upper_limit - 1] == p.character);
        valid_passwords += is_valid;
    }

    std::cout << valid_passwords;

    return 0;
}
