#include <array>
#include <iostream>
#include <random>
#include <vector>
#include <string>
#include <cmath>

using i64 = long long;

enum struct Opcode {
    nop,
    acc,
    jmp
};

struct Instruction {
    Opcode opcode;
    i64 arg1;
};

int main() {
    std::vector<Instruction> instructions;
    std::string buf;
    while(true) {
        i64 arg;
        std::cin >> buf >> arg;
        if(!std::cin) {
            break;
        }

        Opcode op;
        if(buf == "nop") {
            op = Opcode::nop;
        } else if(buf == "acc") {
            op = Opcode::acc;
        } else if(buf == "jmp") {
            op = Opcode::jmp;
        }

        instructions.push_back(Instruction{op, arg});
        buf.clear();
    }

    for(Instruction& instr: instructions) {
        Opcode const orig_opcode = instr.opcode;
        if(instr.opcode == Opcode::nop) {
            instr.opcode = Opcode::jmp;
        } else if(instr.opcode == Opcode::jmp) {
            instr.opcode = Opcode::nop;
        } else {
            continue;
        }

        std::vector<i64> exec_count(instructions.size(), 0);
        i64 rcx = 0;
        i64 rip = 0;
        while(rip != instructions.size()) {
            Instruction const instruction = instructions[rip];

            exec_count[rip] += 1;
            if(exec_count[rip] == 2) {
                break;
            }

            switch(instruction.opcode) {
                case Opcode::acc: {
                    rcx += instruction.arg1;
                    rip += 1;
                } break;

                case Opcode::jmp: {
                    rip += instruction.arg1;
                } break;

                case Opcode::nop: {
                    rip += 1;
                } break;
            }
        }

        instr.opcode = orig_opcode;

        if(rip == instructions.size()) {
            std::cout << rcx;
            break;
        }
    }

    return 0;
}
