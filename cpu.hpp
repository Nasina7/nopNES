#include "include.hpp"

int nesPowerOn()
{
    NESOB.pc = NESOB.memory[0xFFFD] << 8 | NESOB.memory[0xFFFC]; // Sets Initial PC Value
    NESOB.pflag = 0x34;
    NESOB.sp = 0xFD;
    NESOB.a = 0x00;
    NESOB.x = 0x00;
    NESOB.y = 0x00;
}

int doOpcode()
{
    switch (NESOB.opcode)
    {
        case 0x10:
            NESOB.Pbitbuffer = NESOB.pflag;
            if(NESOB.Pbitbuffer[7] == 0)
            {
                NESOB.lowerPC = NESOB.pc;
                NESOB.lowerPC = NESOB.lowerPC + NESOB.memory[NESOB.pc + 1];
                NESOB.higherPC = NESOB.pc >> 8;
                NESOB.pc = NESOB.higherPC << 8 | NESOB.lowerPC;
                NESOB.pc += 2;
                NESOB.cycles += 3;
            }
            if(NESOB.Pbitbuffer[7] == 1)
            {
                NESOB.pc += 2;
                NESOB.cycles += 2;
            }
        break;

        case 0x29:
            NESOB.prevValue = NESOB.a;
            NESOB.a = NESOB.a & NESOB.memory[NESOB.pc + 1];
            handleFlags(NESOB.a,NESOB.prevValue);
            NESOB.pc += 2;
            NESOB.cycles += 2;
        break;

        case 0x78:
            NESOB.Pbitbuffer = NESOB.pflag;
            NESOB.Pbitbuffer[2] = 1;
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            NESOB.pc++;
            NESOB.cycles += 2;
        break;

        case 0x88:
            NESOB.prevValue = NESOB.y;
            NESOB.y--;
            handleFlags(NESOB.y,NESOB.prevValue);
            NESOB.pc += 1;
            NESOB.cycles += 2;
        break;

        case 0x8C:
            NESOB.memory[NESOB.memory[NESOB.pc + 2] << 8 | NESOB.memory[NESOB.pc + 1]] = NESOB.y;
            NESOB.pc += 3;
            NESOB.cycles += 4;
        break;

        case 0x8D:
            NESOB.memory[NESOB.memory[NESOB.pc + 2] << 8 | NESOB.memory[NESOB.pc + 1]] = NESOB.a;
            NESOB.pc += 3;
            NESOB.cycles += 4;
        break;

        case 0x8E:
            NESOB.memory[NESOB.memory[NESOB.pc + 2] << 8 | NESOB.memory[NESOB.pc + 1]] = NESOB.x;
            NESOB.pc += 3;
            NESOB.cycles += 4;
        break;

        case 0x9A:
            NESOB.sp = NESOB.x;
            NESOB.pc += 1;
            NESOB.cycles += 2;
        break;

        case 0xA0:
            NESOB.y = NESOB.memory[NESOB.pc + 1];
            NESOB.Xbitbuffer = NESOB.y;
            NESOB.Pbitbuffer = NESOB.pflag;
            if(NESOB.Xbitbuffer[7] == 1)
            {
                NESOB.Pbitbuffer[7] = 1;
            }
            if(NESOB.Xbitbuffer[7] == 0)
            {
                NESOB.Pbitbuffer[7] = 0;
            }
            if(NESOB.y == 0x00)
            {
                NESOB.Pbitbuffer[1] = 1;
            }
            if(NESOB.y != 0x00)
            {
                NESOB.Pbitbuffer[1] = 0;
            }
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            NESOB.pc += 2;
            NESOB.cycles += 2;
        break;

        case 0xA2:
            NESOB.x = NESOB.memory[NESOB.pc + 1];
            NESOB.Xbitbuffer = NESOB.x;
            NESOB.Pbitbuffer = NESOB.pflag;
            if(NESOB.Xbitbuffer[7] == 1)
            {
                NESOB.Pbitbuffer[7] = 1;
            }
            if(NESOB.Xbitbuffer[7] == 0)
            {
                NESOB.Pbitbuffer[7] = 0;
            }
            if(NESOB.x == 0x00)
            {
                NESOB.Pbitbuffer[1] = 1;
            }
            if(NESOB.x != 0x00)
            {
                NESOB.Pbitbuffer[1] = 0;
            }
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            NESOB.pc += 2;
            NESOB.cycles += 2;
        break;

        case 0xA9:
            NESOB.a = NESOB.memory[NESOB.pc + 1];
            NESOB.Xbitbuffer = NESOB.a;
            NESOB.Pbitbuffer = NESOB.pflag;
            if(NESOB.Xbitbuffer[7] == 1)
            {
                NESOB.Pbitbuffer[7] = 1;
            }
            if(NESOB.Xbitbuffer[7] == 0)
            {
                NESOB.Pbitbuffer[7] = 0;
            }
            if(NESOB.a == 0x00)
            {
                NESOB.Pbitbuffer[1] = 1;
            }
            if(NESOB.a != 0x00)
            {
                NESOB.Pbitbuffer[1] = 0;
            }
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            NESOB.pc += 2;
            NESOB.cycles += 2;
        break;

        case 0xAD:
            NESOB.prevValue = NESOB.a; // Load PrevValue with A.
            NESOB.a = NESOB.memory[NESOB.memory[NESOB.pc + 2] << 8 | NESOB.memory[NESOB.pc + 1]]; // Load data in memory at location stored at pc+1 and pc+2.
            handleFlags(NESOB.a,NESOB.prevValue); // Handles the flags
            NESOB.pc += 3;
            NESOB.cycles += 4;
        break;

        case 0xB9:
            NESOB.prevValue = NESOB.a; // Load PrevValue with A.
            NESOB.a = NESOB.memory[((NESOB.memory[NESOB.pc + 2] << 8 | NESOB.memory[NESOB.pc + 1]) + NESOB.y)]; // Load data in memory at location stored at pc+1 and pc+2 + x.
            handleFlags(NESOB.a,NESOB.prevValue); // Handles the flags
            NESOB.pc += 3;
            NESOB.cycles += 4;
        break;

        case 0xBD:
            NESOB.prevValue = NESOB.a; // Load PrevValue with A.
            NESOB.a = NESOB.memory[((NESOB.memory[NESOB.pc + 2] << 8 | NESOB.memory[NESOB.pc + 1]) + NESOB.x)]; // Load data in memory at location stored at pc+1 and pc+2 + x.
            handleFlags(NESOB.a,NESOB.prevValue); // Handles the flags
            NESOB.pc += 3;
            NESOB.cycles += 4;
        break;

        case 0xCA:
            NESOB.prevValue = NESOB.x;
            NESOB.x--;
            handleFlags(NESOB.x,NESOB.prevValue);
            NESOB.pc += 1;
            NESOB.cycles += 2;
        break;

        case 0xD0:
            NESOB.Pbitbuffer = NESOB.pflag;
            if(NESOB.Pbitbuffer[1] == 0)
            {
                NESOB.lowerPC = NESOB.pc;
                NESOB.lowerPC = NESOB.lowerPC + NESOB.memory[NESOB.pc + 1];
                NESOB.higherPC = NESOB.pc >> 8;
                NESOB.pc = NESOB.higherPC << 8 | NESOB.lowerPC;
                NESOB.pc += 2;
                NESOB.cycles += 3;
            }
            if(NESOB.Pbitbuffer[1] == 1)
            {
                NESOB.pc += 2;
                NESOB.cycles += 2;
            }
        break;

        case 0xD8:
            NESOB.Pbitbuffer = NESOB.pflag;
            NESOB.Pbitbuffer[3] = 0;
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            NESOB.pc++;
            NESOB.cycles += 2;
        break;

        case 0xE8:
            NESOB.prevValue = NESOB.x;
            NESOB.x++;
            handleFlags(NESOB.x,NESOB.prevValue);
            NESOB.pc += 1;
            NESOB.cycles += 2;
        break;

        default:
            printf("Unknown Opcode 0x%X!\n",NESOB.opcode);
            NESOB.closeProgram = true;
        break;

    }
    return true;
}
