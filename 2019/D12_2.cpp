#include <iostream>
#include <string.h>
#include <vector>

using i64 = signed long long;

i64 sign(i64 a) {
    return (a > 0) - (a < 0);
}

i64 gcd(i64 a, i64 b) {
    while (b != 0) {
        i64 t = b;
        b = a % b;
        a = t;
    }
    return a;
}

i64 lcm(i64 a, i64 b) {
    return a / gcd(a, b) * b;
}

class Vec {
public:
    i64 x;
    i64 y;
    i64 z;

    i64 operator[](i64 index) const {
        return (&x)[index];
    }
};

Vec operator+(Vec v1, Vec v2) {
    return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

class Moon {
public:
    Vec position;
    Vec velocity;
};

void update_velocity(Moon& m1, Moon& m2) {
    i64 diff_x = m2.position.x - m1.position.x;
    i64 sgn_x = sign(diff_x);
    m1.velocity.x += sgn_x;
    m2.velocity.x -= sgn_x;

    i64 diff_y = m2.position.y - m1.position.y;
    i64 sgn_y = sign(diff_y);
    m1.velocity.y += sgn_y;
    m2.velocity.y -= sgn_y;

    i64 diff_z = m2.position.z - m1.position.z;
    i64 sgn_z = sign(diff_z);
    m1.velocity.z += sgn_z;
    m2.velocity.z -= sgn_z;
}

class Moon_State {
public:
    Moon moons[4];
};

bool velocity_component_zero(Moon (&moons)[4], i64 c) {
    return moons[0].velocity[c] == 0 && moons[1].velocity[c] == 0 && moons[2].velocity[c] == 0 && moons[3].velocity[c] == 0;
}

int main() {
    std::ios_base::sync_with_stdio(false);

    Moon moons[4] = {{{-13, -13, -13}, {0, 0, 0}}, {{5, -8, 3}, {0, 0, 0}}, {{-6, -10, -3}, {0, 0, 0}}, {{0, 5, -5}, {0, 0, 0}}};
    // Moon moons[4] = {{{-8, -10, -0}, {0, 0, 0}}, {{5, 5, 10}, {0, 0, 0}}, {{2, -7, 3}, {0, 0, 0}}, {{9, -8, -3}, {0, 0, 0}}};
    // Moon moons[4] = {{{-1, 0, 2}, {0, 0, 0}}, {{2, -10, -7}, {0, 0, 0}}, {{4, -8, 8}, {0, 0, 0}}, {{3, 5, -1}, {0, 0, 0}} };
    i64 vel_x = -1;
    i64 vel_y = -1;
    i64 vel_z = -1;
    for (i64 i = 0; i < 1000000; ++i) {
        for (i64 j = 0; j < 3; ++j) {
            for (i64 k = j + 1; k < 4; ++k) {
                update_velocity(moons[j], moons[k]);
            }
        }

        for (i64 j = 0; j < 4; ++j) {
            moons[j].position = moons[j].position + moons[j].velocity;
        }

        if (vel_x == -1 && velocity_component_zero(moons, 0)) {
            vel_x = i + 1;
        }

        if (vel_y == -1 && velocity_component_zero(moons, 1)) {
            vel_y = i + 1;
        }

        if (vel_z == -1 && velocity_component_zero(moons, 2)) {
            vel_z = i + 1;
        }
    }

    std::cout << vel_x << ' ' << vel_y << ' ' << vel_z << " product: " << lcm(lcm(vel_x, vel_y), vel_z);

    return 0;
}
