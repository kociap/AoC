#include <iostream>
#include <vector>
#include <string.h>

using i64 = signed long long;

i64 abs(i64 a) {
    return a < 0 ? -a : a;
}

class Vec {
public:
    i64 x;
    i64 y;
    i64 z;
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
    if(m1.position.x < m2.position.x) {
        m1.velocity.x += 1;
        m2.velocity.x -= 1;
    } else if(m1.position.x > m2.position.x) {
        m1.velocity.x -= 1;
        m2.velocity.x += 1;
    }

    if(m1.position.y < m2.position.y) {
        m1.velocity.y += 1;
        m2.velocity.y -= 1;
    } else if(m1.position.y > m2.position.y) {
        m1.velocity.y -= 1;
        m2.velocity.y += 1;
    }

    if(m1.position.z < m2.position.z) {
        m1.velocity.z += 1;
        m2.velocity.z -= 1;
    } else if(m1.position.z > m2.position.z) {
        m1.velocity.z -= 1;
        m2.velocity.z += 1;
    }
}

int main() {
    Moon moons[4] = {{{-13, -13, -13}, {0, 0, 0}}, {{5, -8, 3}, {0, 0, 0}}, {{-6, -10, -3}, {0, 0, 0}}, {{0, 5, -5}, {0, 0, 0}}};
    for(i64 i = 0; i < 1000; ++i) {
        for(i64 j = 0; j < 3; ++j) {
            for(i64 k = j + 1; k < 4; ++k) {
                update_velocity(moons[j], moons[k]);
            }
        }

        for(i64 j = 0; j < 4; ++j) {
            moons[j].position = moons[j].position + moons[j].velocity;
        }
    }

    i64 total_energy = 0;
    for(Moon const m: moons) {
        i64 const potential = abs(m.position.x) + abs(m.position.y) + abs(m.position.z);
        i64 const kinetic = abs(m.velocity.x) + abs(m.velocity.y) + abs(m.velocity.z);
        total_energy += potential * kinetic;
    }
    std::cout << total_energy;

    return 0;
}
