#include "Controls.hpp"

void nesPowerOn()
{
    //NESOB.pc = 0xC000;
    NESOB.pc = NESOB.memory[0xFFFD] << 8 | NESOB.memory[0xFFFC]; // Sets Initial PC Value
    NESOB.pflag = 0x24;
    NESOB.sp = 0xFD;
    NESOB.a = 0x00;
    NESOB.x = 0x00;
    NESOB.y = 0x00;
}
void handleNMI()
{
    NESOB.Xbitbuffer = NESOB.memory[0x2000];
    if(NESOB.Xbitbuffer[7] == 1 && NMIrequest == true)
    {
        NMIrequest = false;
        printf("NMI\n");
        NESOB.tempValue16 = 0x01 << 8 | NESOB.sp;
        NESOB.higherPC = NESOB.pc >> 8;
        NESOB.lowerPC = NESOB.pc;
        NESmemWrite(NESOB.higherPC,NESOB.tempValue16);
        NESOB.tempValue16--;
        NESmemWrite(NESOB.lowerPC,NESOB.tempValue16);
        NESOB.tempValue16--;
        NESmemWrite(NESOB.pflag,NESOB.tempValue16);
        NESOB.tempValue16--;
        NESOB.sp -= 3;
        NESOB.pc = NESOB.memory[0xFFFB] << 8 | NESOB.memory[0xFFFA];
    }
}

void handleInterrupts()
{
    handleNMI();
}
int doOpcode()
{
    switch (NESOB.opcode)
    {

        case 0x00:
            NESOB.pc += 2;
            NESOB.tempValue16 = 0x01 << 8 | NESOB.sp;
            NESmemWrite(NESOB.pc >> 8, NESOB.tempValue16);
            NESOB.tempValue16 = NESOB.tempValue16--;
            NESmemWrite(NESOB.pc, NESOB.tempValue16);
            NESOB.tempValue16 = NESOB.tempValue16--;
            NESOB.Pbitbuffer = NESOB.pflag;
            NESOB.Pbitbuffer[4] = 1;
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            NESmemWrite(NESOB.pflag, NESOB.tempValue16);
            NESOB.sp -= 3;
            NESOB.pc = NESmemRead(0xFFFF) << 8 | NESmemRead(0xFFFE);
            NESOB.cycles += 7;
        break;

        case 0x01:
            NESOB.prevValue = NESOB.a;
            NESOB.tempValue162 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue162 += NESOB.x;
            if(NESOB.tempValue162 >= 0x100)
            {
                NESOB.tempValue162 -= 0x0100;
            }
            NESOB.tempValue163 = NESmemRead(NESOB.tempValue162 + 1) << 8 | NESmemRead(NESOB.tempValue162);
            NESOB.a = NESOB.a | NESmemRead(NESOB.tempValue163);
            handleFlags7(NESOB.a, NESOB.prevValue);
            handleFlags1(NESOB.a, NESOB.prevValue);
            NESOB.pc += 2;
            NESOB.cycles += 6;
        break;

        case 0x05:
            NESOB.prevValue = NESOB.a;
            NESOB.tempValue16 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.a = NESOB.a | NESmemRead(NESOB.tempValue16);
            handleFlags7(NESOB.a, NESOB.prevValue);
            handleFlags1(NESOB.a, NESOB.prevValue);
            NESOB.pc += 2;
            NESOB.cycles += 3;
        break;

        case 0x06:
            NESOB.tempValue16 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.prevValue = NESOB.memory[NESOB.tempValue16];
            NESOB.Pbitbuffer = NESOB.pflag;
            NESOB.Xbitbuffer = NESOB.memory[NESOB.tempValue16];
            NESOB.Pbitbuffer[0] = NESOB.Xbitbuffer[7];
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            NESOB.Xbitbuffer = NESOB.Xbitbuffer << 1;
            NESOB.memory[NESOB.tempValue16] = NESOB.Xbitbuffer.to_ulong();
            handleFlags7(NESOB.memory[NESOB.tempValue16],NESOB.prevValue);
            handleFlags1(NESOB.memory[NESOB.tempValue16],NESOB.prevValue);
            NESOB.pc += 2;
            NESOB.cycles += 5;
        break;

        case 0x08:
            NESOB.tempValue16 = 0x01 << 8 | NESOB.sp;
            NESOB.Pbitbuffer = NESOB.pflag;
            NESOB.Pbitbuffer[4] = 1;
            NESOB.Pbitbuffer[5] = 1;
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            NESmemWrite(NESOB.pflag, NESOB.tempValue16);
            NESOB.sp--;
            NESOB.pc++;
            NESOB.cycles += 3;
        break;

        case 0x09:
            NESOB.prevValue = NESOB.a;
            NESOB.a = NESOB.a | NESmemRead(NESOB.pc + 1);
            handleFlags7(NESOB.a, NESOB.prevValue);
            handleFlags1(NESOB.a, NESOB.prevValue);
            NESOB.pc += 2;
            NESOB.cycles += 2;
        break;

        case 0x0A:
            NESOB.prevValue = NESOB.a;
            NESOB.Pbitbuffer = NESOB.pflag;
            NESOB.Xbitbuffer = NESOB.a;
            NESOB.Pbitbuffer[0] = NESOB.Xbitbuffer[7];
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            NESOB.Xbitbuffer = NESOB.Xbitbuffer << 1;
            NESOB.a = NESOB.Xbitbuffer.to_ulong();
            handleFlags7(NESOB.a,NESOB.prevValue);
            handleFlags1(NESOB.a,NESOB.prevValue);
            NESOB.pc += 1;
            NESOB.cycles += 2;
        break;

        case 0x0D:
            NESOB.prevValue = NESOB.a;
            NESOB.tempValue16 = NESmemRead(NESOB.pc + 2) << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.a = NESOB.a | NESmemRead(NESOB.tempValue16);
            handleFlags7(NESOB.a, NESOB.prevValue);
            handleFlags1(NESOB.a, NESOB.prevValue);
            NESOB.pc += 3;
            NESOB.cycles += 4;
        break;

        case 0x0E:
            NESOB.tempValue16 = NESmemRead(NESOB.pc + 2) << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.prevValue = NESOB.memory[NESOB.tempValue16];
            NESOB.Pbitbuffer = NESOB.pflag;
            NESOB.Xbitbuffer = NESOB.memory[NESOB.tempValue16];
            NESOB.Pbitbuffer[0] = NESOB.Xbitbuffer[7];
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            NESOB.Xbitbuffer = NESOB.Xbitbuffer << 1;
            NESOB.memory[NESOB.tempValue16] = NESOB.Xbitbuffer.to_ulong();
            handleFlags7(NESOB.memory[NESOB.tempValue16],NESOB.prevValue);
            handleFlags1(NESOB.memory[NESOB.tempValue16],NESOB.prevValue);
            NESOB.pc += 3;
            NESOB.cycles += 6;
        break;

        case 0x10:
            NESOB.Pbitbuffer = NESOB.pflag;
            if(NESOB.Pbitbuffer[7] == 0)
            {
                NESOB.Xbitbuffer = NESmemRead(NESOB.pc + 1);
                if(NESOB.Xbitbuffer[7] == 1)
                {

                    NESOB.pc = NESOB.pc + NESmemRead(NESOB.pc + 1);
                    NESOB.pc = NESOB.pc - 0x0100;
                }
                if(NESOB.Xbitbuffer[7] == 0)
                {
                    NESOB.pc = NESOB.pc + NESmemRead(NESOB.pc + 1);
                }
                NESOB.pc += 2;
                NESOB.cycles += 3;
            }
            if(NESOB.Pbitbuffer[7] == 1)
            {
                NESOB.pc += 2;
                NESOB.cycles += 2;
            }
        break;

        case 0x11:
            NESOB.a = NESOB.a | NESmemRead(indirectYget());
            handleFlags7(NESOB.a, NESOB.prevValue);
            handleFlags1(NESOB.a, NESOB.prevValue);
            NESOB.pc += 2;
            NESOB.cycles += 5;
        break;

        case 0x15:
            NESOB.prevValue = NESOB.a;
            NESOB.tempValue16 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue16 += NESOB.x;
            if(NESOB.tempValue16 >= 0x0100)
            {
                NESOB.tempValue16 -= 0x0100;
            }
            NESOB.a = NESOB.a | NESmemRead(NESOB.tempValue16);
            handleFlags7(NESOB.a, NESOB.prevValue);
            handleFlags1(NESOB.a, NESOB.prevValue);
            NESOB.pc += 2;
            NESOB.cycles += 3;
        break;

        case 0x16:
            NESOB.tempValue16 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue16 += NESOB.x;
            if(NESOB.tempValue16 >= 0x0100)
            {
                NESOB.tempValue16 -= 0x0100;
            }
            NESOB.prevValue = NESOB.memory[NESOB.tempValue16];
            NESOB.Pbitbuffer = NESOB.pflag;
            NESOB.Xbitbuffer = NESOB.memory[NESOB.tempValue16];
            NESOB.Pbitbuffer[0] = NESOB.Xbitbuffer[7];
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            NESOB.Xbitbuffer = NESOB.Xbitbuffer << 1;
            NESOB.memory[NESOB.tempValue16] = NESOB.Xbitbuffer.to_ulong();
            handleFlags7(NESOB.memory[NESOB.tempValue16],NESOB.prevValue);
            handleFlags1(NESOB.memory[NESOB.tempValue16],NESOB.prevValue);
            NESOB.pc += 2;
            NESOB.cycles += 6;
        break;

        case 0x18:
            NESOB.Pbitbuffer = NESOB.pflag;
            NESOB.Pbitbuffer[0] = 0;
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            NESOB.pc += 1;
            NESOB.cycles += 2;
        break;

        case 0x19:
            NESOB.prevValue = NESOB.a;
            NESOB.tempValue16 = NESmemRead(NESOB.pc + 2) << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue16 = NESOB.tempValue16 + NESOB.y;
            NESOB.a = NESOB.a | NESmemRead(NESOB.tempValue16);
            handleFlags7(NESOB.a, NESOB.prevValue);
            handleFlags1(NESOB.a, NESOB.prevValue);
            NESOB.pc += 3;
            NESOB.cycles += 4;
        break;

        case 0x1D:
            NESOB.prevValue = NESOB.a;
            NESOB.tempValue16 = NESmemRead(NESOB.pc + 2) << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue16 = NESOB.tempValue16 + NESOB.x;
            NESOB.a = NESOB.a | NESmemRead(NESOB.tempValue16);
            handleFlags7(NESOB.a, NESOB.prevValue);
            handleFlags1(NESOB.a, NESOB.prevValue);
            NESOB.pc += 3;
            NESOB.cycles += 4;
        break;

        case 0x1E:
            NESOB.tempValue16 = NESmemRead(NESOB.pc + 2) << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue16 += NESOB.x;
            NESOB.prevValue = NESOB.memory[NESOB.tempValue16];
            NESOB.Pbitbuffer = NESOB.pflag;
            NESOB.Xbitbuffer = NESOB.memory[NESOB.tempValue16];
            NESOB.Pbitbuffer[0] = NESOB.Xbitbuffer[7];
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            NESOB.Xbitbuffer = NESOB.Xbitbuffer << 1;
            NESOB.memory[NESOB.tempValue16] = NESOB.Xbitbuffer.to_ulong();
            handleFlags7(NESOB.memory[NESOB.tempValue16],NESOB.prevValue);
            handleFlags1(NESOB.memory[NESOB.tempValue16],NESOB.prevValue);
            NESOB.pc += 3;
            NESOB.cycles += 7;
        break;

        case 0x20:
            NESOB.pc += 3;
            NESOB.tempValue16 = 0x01 << 8 | NESOB.sp;
            NESOB.tempValue = NESOB.pc >> 8;
            NESmemWrite(NESOB.tempValue,NESOB.tempValue16);
            NESOB.tempValue16--;
            NESOB.tempValue = NESOB.pc - 0x01;
            NESmemWrite(NESOB.tempValue,NESOB.tempValue16);
            NESOB.pc = NESmemRead(NESOB.pc - 0x01) << 8 | NESmemRead(NESOB.pc - 0x02);
            NESOB.sp -= 2;
            NESOB.cycles += 6;
        break;

        case 0x21:
            NESOB.prevValue = NESOB.a;
            NESOB.tempValue16 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue16 += NESOB.x;
            if(NESOB.tempValue16 >= 0x0100)
            {
                NESOB.tempValue16 -= 0x0100;
            }
            NESOB.tempValue162 = NESmemRead(NESOB.tempValue16 + 1) << 8 | NESmemRead(NESOB.tempValue16);
            NESOB.a = NESOB.a & NESmemRead(NESOB.tempValue162);
            handleFlags7(NESOB.a,NESOB.prevValue);
            handleFlags1(NESOB.a,NESOB.prevValue);
            NESOB.pc += 2;
            NESOB.cycles += 6;
        break;

        case 0x24:
            NESOB.tempValue16 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue = NESOB.a & NESmemRead(NESOB.tempValue16);
            handleFlags1(NESOB.tempValue, NESOB.a);
            NESOB.tempValue = NESmemRead(NESOB.tempValue16);
            NESOB.Xbitbuffer = NESOB.tempValue;
            NESOB.Pbitbuffer = NESOB.pflag;
            NESOB.Pbitbuffer[7] = NESOB.Xbitbuffer[7];
            NESOB.Pbitbuffer[6] = NESOB.Xbitbuffer[6];
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            NESOB.pc += 2;
            NESOB.cycles += 3;
        break;

        case 0x25:
            NESOB.prevValue = NESOB.a;
            NESOB.tempValue16 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.a = NESOB.a & NESmemRead(NESOB.tempValue16);
            handleFlags7(NESOB.a,NESOB.prevValue);
            handleFlags1(NESOB.a,NESOB.prevValue);
            NESOB.pc += 2;
            NESOB.cycles += 2;
        break;

        case 0x26:
            NESOB.tempValue16 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue = NESmemRead(NESOB.tempValue16);
            NESOB.prevValue = NESOB.tempValue;
            NESOB.Abitbuffer = NESOB.tempValue;
            NESOB.Pbitbuffer = NESOB.pflag;
            NESOB.Xbitbuffer[7] = NESOB.Pbitbuffer[0];
            NESOB.Pbitbuffer[0] = NESOB.Abitbuffer[7];
            NESOB.Abitbuffer = NESOB.Abitbuffer << 1;
            NESOB.Abitbuffer[0] = NESOB.Xbitbuffer[7];
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            NESOB.tempValue = NESOB.Abitbuffer.to_ulong();
            handleFlags7(NESOB.tempValue, NESOB.prevValue);
            NESOB.Pbitbuffer = NESOB.pflag;
            NESOB.Pbitbuffer[1] = 0;
            if(NESOB.prevValue != 0x00 && NESOB.tempValue == 0x00)
            {
                NESOB.Pbitbuffer[1] = 1;
            }
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            NESmemWrite(NESOB.tempValue, NESOB.tempValue16);
            NESOB.pc += 2;
            NESOB.cycles += 5;
        break;

        case 0x28:
            NESOB.sp++;
            NESOB.tempValue16 = 0x01 << 8 | NESOB.sp;
            NESOB.pflag = NESmemRead(NESOB.tempValue16);
            NESOB.pc++;
            NESOB.cycles += 4;
        break;

        case 0x29:
            NESOB.prevValue = NESOB.a;
            NESOB.a = NESOB.a & NESOB.memory[NESOB.pc + 1];
            handleFlags7(NESOB.a,NESOB.prevValue);
            handleFlags1(NESOB.a,NESOB.prevValue);
            NESOB.pc += 2;
            NESOB.cycles += 2;
        break;

        case 0x2A:
            NESOB.prevValue = NESOB.a;
            NESOB.Abitbuffer = NESOB.a;
            NESOB.Xbitbuffer[7] = NESOB.Abitbuffer[7];
            NESOB.Abitbuffer = NESOB.Abitbuffer << 1;
            NESOB.Pbitbuffer = NESOB.pflag;
            NESOB.Abitbuffer[0] = NESOB.Pbitbuffer[0];
            NESOB.Pbitbuffer[0] = NESOB.Xbitbuffer[7];
            NESOB.a = NESOB.Abitbuffer.to_ulong();
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            handleFlags7(NESOB.a, NESOB.prevValue);
            handleFlags1(NESOB.a, NESOB.prevValue);
            NESOB.pc++;
            NESOB.cycles += 2;
        break;

        case 0x2C:
            NESOB.tempValue16 = NESmemRead(NESOB.pc + 2) << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue = NESOB.a & NESmemRead(NESOB.tempValue16);
            handleFlags1(NESOB.tempValue, NESOB.a);
            NESOB.tempValue = NESmemRead(NESOB.tempValue16);
            NESOB.Xbitbuffer = NESOB.tempValue;
            NESOB.Pbitbuffer = NESOB.pflag;
            NESOB.Pbitbuffer[7] = NESOB.Xbitbuffer[7];
            NESOB.Pbitbuffer[6] = NESOB.Xbitbuffer[6];
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            NESOB.pc += 3;
            NESOB.cycles += 4;
        break;

        case 0x2D:
            NESOB.prevValue = NESOB.a;
            NESOB.tempValue16 = NESmemRead(NESOB.pc + 2) << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.a = NESOB.a & NESmemRead(NESOB.tempValue16);
            handleFlags7(NESOB.a,NESOB.prevValue);
            handleFlags1(NESOB.a,NESOB.prevValue);
            NESOB.pc += 3;
            NESOB.cycles += 4;
        break;

        case 0x2E:
            NESOB.tempValue16 = NESmemRead(NESOB.pc + 2) << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.prevValue = NESOB.memory[NESOB.tempValue16];
            NESOB.Abitbuffer = NESOB.memory[NESOB.tempValue16];
            NESOB.Xbitbuffer[7] = NESOB.Abitbuffer[7];
            NESOB.Abitbuffer = NESOB.Abitbuffer << 1;
            NESOB.Pbitbuffer = NESOB.pflag;
            NESOB.Abitbuffer[0] = NESOB.Pbitbuffer[0];
            NESOB.Pbitbuffer[0] = NESOB.Xbitbuffer[7];
            NESOB.memory[NESOB.tempValue16] = NESOB.Abitbuffer.to_ulong();
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            handleFlags7(NESOB.memory[NESOB.tempValue16], NESOB.prevValue);
            handleFlags1(NESOB.memory[NESOB.tempValue16], NESOB.prevValue);
            NESOB.pc += 3;
            NESOB.cycles += 6;
        break;

        case 0x30:
            NESOB.Pbitbuffer = NESOB.pflag;
            if(NESOB.Pbitbuffer[7] == 1)
            {
                NESOB.Xbitbuffer = NESmemRead(NESOB.pc + 1);
                if(NESOB.Xbitbuffer[7] == 1)
                {

                    NESOB.pc = NESOB.pc + NESmemRead(NESOB.pc + 1);
                    NESOB.pc = NESOB.pc - 0x0100;
                }
                if(NESOB.Xbitbuffer[7] == 0)
                {
                    NESOB.pc = NESOB.pc + NESmemRead(NESOB.pc + 1);
                }
                NESOB.pc += 2;
                NESOB.cycles += 3;
            }
            if(NESOB.Pbitbuffer[7] == 0)
            {
                NESOB.pc += 2;
                NESOB.cycles += 2;
            }
        break;

        case 0x31:
            NESOB.prevValue = NESOB.a;
            NESOB.a = NESOB.a & NESmemRead(indirectYget());
            handleFlags7(NESOB.a,NESOB.prevValue);
            handleFlags1(NESOB.a,NESOB.prevValue);
            NESOB.pc += 2;
            NESOB.cycles += 5;
        break;

        case 0x35:
            NESOB.prevValue = NESOB.a;
            NESOB.tempValue16 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue16 = NESOB.tempValue16 + NESOB.x;
            if(NESOB.tempValue16 >= 0x0100)
            {
                NESOB.tempValue16 = NESOB.tempValue16 - 0x0100;
            }
            NESOB.a = NESOB.a & NESmemRead(NESOB.tempValue16);
            handleFlags7(NESOB.a,NESOB.prevValue);
            handleFlags1(NESOB.a,NESOB.prevValue);
            NESOB.pc += 2;
            NESOB.cycles += 2;
        break;

        case 0x36:
            //breakpoint = true;
            NESOB.tempValue16 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue16 = NESOB.tempValue16 + NESOB.x;
            if(NESOB.tempValue16 >= 0x100)
            {
                NESOB.tempValue16 = NESOB.tempValue16 - 0x0100;
            }
            NESOB.tempValue2 = NESmemRead(NESOB.tempValue16);
            NESOB.prevValue = NESOB.tempValue2;
            NESOB.Pbitbuffer = NESOB.pflag;
            NESOB.Abitbuffer = NESOB.tempValue2;
            NESOB.Xbitbuffer[7] = NESOB.Pbitbuffer[0];
            NESOB.Xbitbuffer[0] = NESOB.Abitbuffer[7];
            NESOB.Abitbuffer <<= 1;
            NESOB.Abitbuffer[0] = NESOB.Xbitbuffer[7];
            NESOB.Pbitbuffer[0] = NESOB.Xbitbuffer[0];
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            NESOB.memory[NESOB.tempValue16] = NESOB.Abitbuffer.to_ulong();
            handleFlags7(NESOB.memory[NESOB.tempValue16], NESOB.prevValue);
            handleFlags1(NESOB.memory[NESOB.tempValue16], NESOB.prevValue);
            NESOB.pc += 2;
            NESOB.cycles += 6;
        break;

        case 0x38:
            NESOB.Pbitbuffer = NESOB.pflag;
            NESOB.Pbitbuffer[0] = 1;
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            NESOB.pc++;
            NESOB.cycles += 2;
        break;

        case 0x39:
            NESOB.prevValue = NESOB.a;
            NESOB.tempValue16 = NESmemRead(NESOB.pc + 2) << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue16 = NESOB.tempValue16 + NESOB.y;
            NESOB.a = NESOB.a & NESmemRead(NESOB.tempValue16);
            handleFlags7(NESOB.a,NESOB.prevValue);
            handleFlags1(NESOB.a,NESOB.prevValue);
            NESOB.pc += 3;
            NESOB.cycles += 4;
        break;

        case 0x3D:
            NESOB.prevValue = NESOB.a;
            NESOB.tempValue16 = NESmemRead(NESOB.pc + 2) << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue16 = NESOB.tempValue16 + NESOB.x;
            NESOB.a = NESOB.a & NESmemRead(NESOB.tempValue16);
            handleFlags7(NESOB.a,NESOB.prevValue);
            handleFlags1(NESOB.a,NESOB.prevValue);
            NESOB.pc += 3;
            NESOB.cycles += 4;
        break;

        case 0x3E:
            NESOB.tempValue16 = NESmemRead(NESOB.pc + 2) << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue16 += NESOB.x;
            NESOB.prevValue = NESOB.memory[NESOB.tempValue16];
            NESOB.Abitbuffer = NESOB.memory[NESOB.tempValue16];
            NESOB.Xbitbuffer[7] = NESOB.Abitbuffer[7];
            NESOB.Abitbuffer = NESOB.Abitbuffer << 1;
            NESOB.Pbitbuffer = NESOB.pflag;
            NESOB.Abitbuffer[0] = NESOB.Pbitbuffer[0];
            NESOB.Pbitbuffer[0] = NESOB.Xbitbuffer[7];
            NESOB.memory[NESOB.tempValue16] = NESOB.Abitbuffer.to_ulong();
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            handleFlags7(NESOB.memory[NESOB.tempValue16], NESOB.prevValue);
            handleFlags1(NESOB.memory[NESOB.tempValue16], NESOB.prevValue);
            NESOB.pc += 3;
            NESOB.cycles += 7;
        break;

        case 0x40:
            NESOB.tempValue16 = 0x01 << 8 | NESOB.sp;
            NESOB.tempValue16++;
            NESOB.pflag = NESmemRead(NESOB.tempValue16);
            NESOB.tempValue16++;
            NESOB.tempValue162 = NESmemRead(NESOB.tempValue16 + 1) << 8 | NESmemRead(NESOB.tempValue16);
            NESOB.pc = NESOB.tempValue162;
            NESOB.sp += 3;
            NESOB.cycles += 6;
        break;

        case 0x41:
            NESOB.prevValue = NESOB.a;
            NESOB.tempValue162 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue162 += NESOB.x;
            if(NESOB.tempValue162 >= 0x0100)
            {
                NESOB.tempValue162 -= 0x0100;
            }
            NESOB.tempValue16 = NESmemRead(NESOB.tempValue162 + 1) << 8 | NESmemRead(NESOB.tempValue162);
            NESOB.a = NESOB.a ^ NESmemRead(NESOB.tempValue16);
            handleFlags7(NESOB.a, NESOB.prevValue);
            handleFlags1(NESOB.a, NESOB.prevValue);
            NESOB.pc += 2;
            NESOB.cycles += 6;
        break;


        case 0x45:
            NESOB.prevValue = NESOB.a;
            NESOB.tempValue16 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.a = NESOB.a ^ NESmemRead(NESOB.tempValue16);
            handleFlags7(NESOB.a, NESOB.prevValue);
            handleFlags1(NESOB.a, NESOB.prevValue);
            NESOB.pc += 2;
            NESOB.cycles += 3;
        break;

        case 0x46:
            NESOB.tempValue16 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.prevValue = NESOB.memory[NESOB.tempValue16];
            NESOB.Pbitbuffer = NESOB.pflag;
            NESOB.Xbitbuffer = NESOB.memory[NESOB.tempValue16];
            NESOB.Pbitbuffer[0] = NESOB.Xbitbuffer[0];
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            NESOB.Xbitbuffer = NESOB.Xbitbuffer >> 1;
            NESOB.memory[NESOB.tempValue16] = NESOB.Xbitbuffer.to_ulong();
            handleFlags7(NESOB.memory[NESOB.tempValue16],NESOB.prevValue);
            handleFlags1(NESOB.memory[NESOB.tempValue16],NESOB.prevValue);
            NESOB.pc += 2;
            NESOB.cycles += 5;
        break;

        case 0x48:
            NESOB.tempValue16 = 0x01 << 8 | NESOB.sp;
            NESmemWrite(NESOB.a, NESOB.tempValue16);
            NESOB.sp--;
            NESOB.pc++;
            NESOB.cycles += 3;
        break;

        case 0x49:
            NESOB.prevValue = NESOB.a;
            NESOB.a = NESOB.a ^ NESmemRead(NESOB.pc + 1);
            handleFlags7(NESOB.a, NESOB.prevValue);
            handleFlags1(NESOB.a, NESOB.prevValue);
            NESOB.pc += 2;
            NESOB.cycles += 2;
        break;

        case 0x4A:
            NESOB.prevValue = NESOB.a;
            NESOB.Abitbuffer = NESOB.a;
            NESOB.Pbitbuffer = NESOB.pflag;
            NESOB.Pbitbuffer[0] = NESOB.Abitbuffer[0];
            NESOB.Abitbuffer = NESOB.Abitbuffer >> 1;
            NESOB.Abitbuffer[7] = 0;
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            NESOB.a = NESOB.Abitbuffer.to_ulong();
            handleFlags7(NESOB.a, NESOB.prevValue);
            handleFlags1(NESOB.a, NESOB.prevValue);
            NESOB.pc++;
            NESOB.cycles += 2;
        break;

        case 0x4C:
            NESOB.pc = NESmemRead(NESOB.pc + 2) << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.cycles += 3;
        break;

        case 0x4D:
            NESOB.prevValue = NESOB.a;
            NESOB.tempValue16 = NESmemRead(NESOB.pc + 2) << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.a = NESOB.a ^ NESmemRead(NESOB.tempValue16);
            handleFlags7(NESOB.a, NESOB.prevValue);
            handleFlags1(NESOB.a, NESOB.prevValue);
            NESOB.pc += 3;
            NESOB.cycles += 4;
        break;

        case 0x4E:
            NESOB.tempValue16 = NESmemRead(NESOB.pc + 2) << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.prevValue = NESOB.memory[NESOB.tempValue16];
            NESOB.Pbitbuffer = NESOB.pflag;
            NESOB.Xbitbuffer = NESOB.memory[NESOB.tempValue16];
            NESOB.Pbitbuffer[0] = NESOB.Xbitbuffer[0];
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            NESOB.Xbitbuffer = NESOB.Xbitbuffer >> 1;
            NESOB.memory[NESOB.tempValue16] = NESOB.Xbitbuffer.to_ulong();
            handleFlags7(NESOB.memory[NESOB.tempValue16],NESOB.prevValue);
            handleFlags1(NESOB.memory[NESOB.tempValue16],NESOB.prevValue);
            NESOB.pc += 3;
            NESOB.cycles += 6;
        break;

        case 0x50:
            NESOB.Pbitbuffer = NESOB.pflag;
            if(NESOB.Pbitbuffer[6] == 0)
            {
                NESOB.Xbitbuffer = NESmemRead(NESOB.pc + 1);
                if(NESOB.Xbitbuffer[7] == 1)
                {

                    NESOB.pc = NESOB.pc + NESmemRead(NESOB.pc + 1);
                    NESOB.pc = NESOB.pc - 0x0100;
                }
                if(NESOB.Xbitbuffer[7] == 0)
                {
                    NESOB.pc = NESOB.pc + NESmemRead(NESOB.pc + 1);
                }
                NESOB.pc += 2;
                NESOB.cycles += 3;
            }
            if(NESOB.Pbitbuffer[6] == 1)
            {
                NESOB.pc += 2;
                NESOB.cycles += 2;
            }
        break;

        case 0x51:
            NESOB.a = NESOB.a ^ NESmemRead(indirectYget());
            handleFlags7(NESOB.a, NESOB.prevValue);
            handleFlags1(NESOB.a, NESOB.prevValue);
            NESOB.pc += 2;
            NESOB.cycles += 5;
        break;

        case 0x55:
            NESOB.prevValue = NESOB.a;
            NESOB.tempValue16 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue16 = NESOB.tempValue16 + NESOB.x;
            if(NESOB.tempValue16 >= 0x0100)
            {
                NESOB.tempValue16 = NESOB.tempValue16 - 0x0100;
            }
            NESOB.a = NESOB.a ^ NESmemRead(NESOB.tempValue16);
            handleFlags7(NESOB.a, NESOB.prevValue);
            handleFlags1(NESOB.a, NESOB.prevValue);
            NESOB.pc += 2;
            NESOB.cycles += 4;
        break;

        case 0x56:
            NESOB.tempValue16 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue16 += NESOB.x;
            if(NESOB.tempValue16 >= 0x0100)
            {
                NESOB.tempValue16 -= 0x0100;
            }
            NESOB.prevValue = NESOB.memory[NESOB.tempValue16];
            NESOB.Pbitbuffer = NESOB.pflag;
            NESOB.Xbitbuffer = NESOB.memory[NESOB.tempValue16];
            NESOB.Pbitbuffer[0] = NESOB.Xbitbuffer[0];
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            NESOB.Xbitbuffer = NESOB.Xbitbuffer >> 1;
            NESOB.memory[NESOB.tempValue16] = NESOB.Xbitbuffer.to_ulong();
            handleFlags7(NESOB.memory[NESOB.tempValue16],NESOB.prevValue);
            handleFlags1(NESOB.memory[NESOB.tempValue16],NESOB.prevValue);
            NESOB.pc += 2;
            NESOB.cycles += 6;
        break;

        case 0x58:
            NESOB.Pbitbuffer = NESOB.pflag;
            NESOB.Pbitbuffer[2] = 0;
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            NESOB.pc += 1;
            NESOB.cycles += 2;
        break;

        case 0x59:
            NESOB.prevValue = NESOB.a;
            NESOB.tempValue16 = NESmemRead(NESOB.pc + 2) << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue16 += NESOB.y;
            NESOB.a = NESOB.a ^ NESmemRead(NESOB.tempValue16);
            handleFlags7(NESOB.a, NESOB.prevValue);
            handleFlags1(NESOB.a, NESOB.prevValue);
            NESOB.pc += 3;
            NESOB.cycles += 4;
        break;

        case 0x5D:
            NESOB.prevValue = NESOB.a;
            NESOB.tempValue16 = NESmemRead(NESOB.pc + 2) << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue16 += NESOB.x;
            NESOB.a = NESOB.a ^ NESmemRead(NESOB.tempValue16);
            handleFlags7(NESOB.a, NESOB.prevValue);
            handleFlags1(NESOB.a, NESOB.prevValue);
            NESOB.pc += 3;
            NESOB.cycles += 4;
        break;

        case 0x5E:
            NESOB.tempValue16 = NESmemRead(NESOB.pc + 2) << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue16 += NESOB.x;
            NESOB.prevValue = NESOB.memory[NESOB.tempValue16];
            NESOB.Pbitbuffer = NESOB.pflag;
            NESOB.Xbitbuffer = NESOB.memory[NESOB.tempValue16];
            NESOB.Pbitbuffer[0] = NESOB.Xbitbuffer[0];
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            NESOB.Xbitbuffer = NESOB.Xbitbuffer >> 1;
            NESOB.memory[NESOB.tempValue16] = NESOB.Xbitbuffer.to_ulong();
            handleFlags7(NESOB.memory[NESOB.tempValue16],NESOB.prevValue);
            handleFlags1(NESOB.memory[NESOB.tempValue16],NESOB.prevValue);
            NESOB.pc += 3;
            NESOB.cycles += 7;
        break;

        case 0x60:
            NESOB.tempValue16 = 0x01 << 8 | NESOB.sp;
            NESOB.pc = NESmemRead(NESOB.tempValue16 + 2) << 8 | NESmemRead(NESOB.tempValue16 + 1);
            NESOB.pc++;
            NESOB.sp += 2;
            NESOB.cycles += 6;
        break;

        case 0x61:
            NESOB.prevValue = NESOB.a;
            NESOB.tempValue162 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue162 += NESOB.x;
            if(NESOB.tempValue162 >= 0x100)
            {
                NESOB.tempValue162 -= 0x0100;
            }
            NESOB.tempValue163 = NESmemRead(NESOB.tempValue162 + 1) << 8 | NESmemRead(NESOB.tempValue162);
            NESOB.Pbitbuffer = NESOB.pflag;
            a = NESOB.a;
            b = NESmemRead(NESOB.tempValue163);
            diff = a + b + NESOB.Pbitbuffer[0];
            NESOB.a = (uint8_t)diff;
            if(NESOB.a == 0x00)
            {
                NESOB.Pbitbuffer[1] = 1;
            }
            NESOB.Pbitbuffer[0] = (diff >= 0x0100);
            NESOB.Pbitbuffer[6] = ((a^~b)&0x80) != 0 && ((a^NESOB.a)&0x80) != 0;
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            handleFlags7(NESOB.a, a);
            NESOB.pc += 2;
            NESOB.cycles += 6;
        break;

        case 0x65:
            NESOB.tempValue16 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.Pbitbuffer = NESOB.pflag;
            a = NESOB.a;
            b = NESmemRead(NESOB.tempValue16);
            diff = a + b + NESOB.Pbitbuffer[0];
            NESOB.a = (uint8_t)diff;
            if(NESOB.a == 0x00)
            {
                NESOB.Pbitbuffer[1] = 1;
            }
            NESOB.Pbitbuffer[0] = (diff >= 0x0100);
            NESOB.Pbitbuffer[6] = ((a^~b)&0x80) != 0 && ((a^NESOB.a)&0x80) != 0;
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            handleFlags7(NESOB.a, a);
            NESOB.pc += 2;
            NESOB.cycles += 3;
        break;

        case 0x66:
            NESOB.tempValue16 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue = NESmemRead(NESOB.tempValue16);
            NESOB.prevValue = NESOB.tempValue;
            NESOB.Abitbuffer = NESOB.tempValue;
            NESOB.Pbitbuffer = NESOB.pflag;
            NESOB.Xbitbuffer[0] = NESOB.Pbitbuffer[0];
            NESOB.Pbitbuffer[0] = NESOB.Abitbuffer[0];
            NESOB.Abitbuffer = NESOB.Abitbuffer >> 1;
            NESOB.Abitbuffer[7] = NESOB.Xbitbuffer[0];
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            NESOB.tempValue = NESOB.Abitbuffer.to_ulong();
            handleFlags7(NESOB.tempValue, NESOB.prevValue);
            handleFlags1(NESOB.tempValue, NESOB.prevValue);
            NESmemWrite(NESOB.tempValue, NESOB.tempValue16);
            NESOB.pc += 2;
            NESOB.cycles += 5;
        break;

        case 0x68:
            NESOB.sp++;
            NESOB.prevValue = NESOB.a;
            NESOB.tempValue16 = 0x01 << 8 | NESOB.sp;
            NESOB.a = NESmemRead(NESOB.tempValue16);
            handleFlags7(NESOB.a,NESOB.prevValue);
            handleFlags1(NESOB.a,NESOB.prevValue);
            NESOB.pc++;
            NESOB.cycles += 4;
        break;

        case 0x69:
            NESOB.Pbitbuffer = NESOB.pflag;
            a = NESOB.a;
            b = NESmemRead(NESOB.pc + 1);
            diff = a + b + NESOB.Pbitbuffer[0];
            NESOB.a = (uint8_t)diff;
            if(NESOB.a == 0x00)
            {
                NESOB.Pbitbuffer[1] = 1;
            }
            NESOB.Pbitbuffer[0] = (diff >= 0x0100);
            NESOB.Pbitbuffer[6] = ((a^~b)&0x80) != 0 && ((a^NESOB.a)&0x80) != 0;
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            handleFlags7(NESOB.a, a);
            NESOB.pc += 2;
            NESOB.cycles += 2;
        break;

        case 0x6A:
            NESOB.prevValue = NESOB.a;
            NESOB.Abitbuffer = NESOB.a;
            NESOB.Pbitbuffer = NESOB.pflag;
            NESOB.Xbitbuffer[0] = NESOB.Pbitbuffer[0];
            NESOB.Pbitbuffer[0] = NESOB.Abitbuffer[0];
            NESOB.Abitbuffer = NESOB.Abitbuffer >> 1;
            NESOB.Abitbuffer[7] = NESOB.Xbitbuffer[0];
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            NESOB.a = NESOB.Abitbuffer.to_ulong();
            handleFlags7(NESOB.a, NESOB.prevValue);
            handleFlags1(NESOB.a, NESOB.prevValue);
            NESOB.pc += 1;
            NESOB.cycles += 5;
        break;

        case 0x6C:
            NESOB.tempValue162 = NESmemRead(NESOB.pc + 2) << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.prevValue = NESOB.tempValue162;
            NESOB.tempValue2 = NESmemRead(NESOB.tempValue162);
            NESOB.tempValue = NESmemRead(NESOB.tempValue162 + 1);
            if(NESOB.prevValue == 0xFF)
            {
                NESOB.tempValue162++;
                NESOB.tempValue162 -= 0x100;
                NESOB.tempValue = NESmemRead(NESOB.tempValue162);
            }
            NESOB.pc = NESOB.tempValue << 8 | NESOB.tempValue2;
            NESOB.cycles += 5;
        break;

        case 0x6D:
            NESOB.tempValue16 = NESmemRead(NESOB.pc + 2) << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.Pbitbuffer = NESOB.pflag;
            a = NESOB.a;
            b = NESmemRead(NESOB.tempValue16);
            diff = a + b + NESOB.Pbitbuffer[0];
            NESOB.a = (uint8_t)diff;
            if(NESOB.a == 0x00)
            {
                NESOB.Pbitbuffer[1] = 1;
            }
            NESOB.Pbitbuffer[0] = (diff >= 0x0100);
            NESOB.Pbitbuffer[6] = ((a^~b)&0x80) != 0 && ((a^NESOB.a)&0x80) != 0;
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            handleFlags7(NESOB.a, a);
            NESOB.pc += 3;
            NESOB.cycles += 4;
        break;

        case 0x6E:
            NESOB.tempValue16 = NESmemRead(NESOB.pc + 2) << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue = NESmemRead(NESOB.tempValue16);
            NESOB.prevValue = NESOB.tempValue;
            NESOB.Abitbuffer = NESOB.tempValue;
            NESOB.Pbitbuffer = NESOB.pflag;
            NESOB.Xbitbuffer[0] = NESOB.Pbitbuffer[0];
            NESOB.Pbitbuffer[0] = NESOB.Abitbuffer[0];
            NESOB.Abitbuffer = NESOB.Abitbuffer >> 1;
            NESOB.Abitbuffer[7] = NESOB.Xbitbuffer[0];
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            NESOB.tempValue = NESOB.Abitbuffer.to_ulong();
            handleFlags7(NESOB.tempValue, NESOB.prevValue);
            handleFlags1(NESOB.tempValue, NESOB.prevValue);
            NESmemWrite(NESOB.tempValue, NESOB.tempValue16);
            NESOB.pc += 3;
            NESOB.cycles += 6;
        break;

        case 0x70:
            NESOB.Pbitbuffer = NESOB.pflag;
            if(NESOB.Pbitbuffer[6] == 1)
            {
                NESOB.Xbitbuffer = NESmemRead(NESOB.pc + 1);
                if(NESOB.Xbitbuffer[7] == 1)
                {

                    NESOB.pc = NESOB.pc + NESmemRead(NESOB.pc + 1);
                    NESOB.pc = NESOB.pc - 0x0100;
                }
                if(NESOB.Xbitbuffer[7] == 0)
                {
                    NESOB.pc = NESOB.pc + NESmemRead(NESOB.pc + 1);
                }
                NESOB.pc += 2;
                NESOB.cycles += 3;
            }
            if(NESOB.Pbitbuffer[6] == 0)
            {
                NESOB.pc += 2;
                NESOB.cycles += 2;
            }
        break;

        case 0x71:
            NESOB.prevValue = NESOB.a;
            NESOB.Pbitbuffer = NESOB.pflag;
            a = NESOB.a;
            b = NESmemRead(indirectYget());
            diff = a + b + NESOB.Pbitbuffer[0];
            NESOB.a = (uint8_t)diff;
            if(NESOB.a == 0x00)
            {
                NESOB.Pbitbuffer[1] = 1;
            }
            NESOB.Pbitbuffer[0] = (diff >= 0x0100);
            NESOB.Pbitbuffer[6] = ((a^~b)&0x80) != 0 && ((a^NESOB.a)&0x80) != 0;
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            handleFlags7(NESOB.a, a);
            NESOB.pc += 2;
            NESOB.cycles += 5;
        break;

        case 0x75:
            NESOB.prevValue = NESOB.a;
            NESOB.tempValue162 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue162 += NESOB.x;
            if(NESOB.tempValue162 >= 0x0100)
            {
                NESOB.tempValue162 -= 0x0100;
            }
            NESOB.Pbitbuffer = NESOB.pflag;
            a = NESOB.a;
            b = NESmemRead(NESOB.tempValue162);
            diff = a + b + NESOB.Pbitbuffer[0];
            NESOB.a = (uint8_t)diff;
            if(NESOB.a == 0x00)
            {
                NESOB.Pbitbuffer[1] = 1;
            }
            NESOB.Pbitbuffer[0] = (diff >= 0x0100);
            NESOB.Pbitbuffer[6] = ((a^~b)&0x80) != 0 && ((a^NESOB.a)&0x80) != 0;
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            handleFlags7(NESOB.a, a);
            NESOB.pc += 2;
            NESOB.cycles += 3;
        break;

        case 0x76:
            NESOB.tempValue16 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue16 = NESOB.tempValue16 + NESOB.x;
            if(NESOB.tempValue16 >= 0x0100)
            {
                NESOB.tempValue16 = NESOB.tempValue16 - 0x0100;
            }
            NESOB.tempValue = NESmemRead(NESOB.tempValue16);
            NESOB.prevValue = NESOB.tempValue;
            NESOB.Abitbuffer = NESOB.tempValue;
            NESOB.Pbitbuffer = NESOB.pflag;
            NESOB.Xbitbuffer[0] = NESOB.Pbitbuffer[0];
            NESOB.Pbitbuffer[0] = NESOB.Abitbuffer[0];
            NESOB.Abitbuffer = NESOB.Abitbuffer >> 1;
            NESOB.Abitbuffer[7] = NESOB.Xbitbuffer[0];
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            NESOB.tempValue = NESOB.Abitbuffer.to_ulong();
            handleFlags7(NESOB.tempValue, NESOB.prevValue);
            handleFlags1(NESOB.tempValue, NESOB.prevValue);
            NESmemWrite(NESOB.tempValue, NESOB.tempValue16);
            NESOB.pc += 2;
            NESOB.cycles += 5;
        break;

        case 0x78:
            NESOB.Pbitbuffer = NESOB.pflag;
            NESOB.Pbitbuffer[2] = 1;
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            NESOB.pc++;
            NESOB.cycles += 2;
        break;

        case 0x79:
            NESOB.tempValue16 = NESmemRead(NESOB.pc + 2) << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue16 += NESOB.y;
            NESOB.Pbitbuffer = NESOB.pflag;
            a = NESOB.a;
            b = NESmemRead(NESOB.tempValue16);
            diff = a + b + NESOB.Pbitbuffer[0];
            NESOB.a = (uint8_t)diff;
            if(NESOB.a == 0x00)
            {
                NESOB.Pbitbuffer[1] = 1;
            }
            NESOB.Pbitbuffer[0] = (diff >= 0x0100);
            NESOB.Pbitbuffer[6] = ((a^~b)&0x80) != 0 && ((a^NESOB.a)&0x80) != 0;
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            handleFlags7(NESOB.a, a);
            NESOB.pc += 3;
            NESOB.cycles += 4;
        break;

        case 0x7D:
            NESOB.tempValue16 = NESmemRead(NESOB.pc + 2) << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue16 += NESOB.x;
            NESOB.Pbitbuffer = NESOB.pflag;
            a = NESOB.a;
            b = NESmemRead(NESOB.tempValue16);
            diff = a + b + NESOB.Pbitbuffer[0];
            NESOB.a = (uint8_t)diff;
            if(NESOB.a == 0x00)
            {
                NESOB.Pbitbuffer[1] = 1;
            }
            NESOB.Pbitbuffer[0] = (diff >= 0x0100);
            NESOB.Pbitbuffer[6] = ((a^~b)&0x80) != 0 && ((a^NESOB.a)&0x80) != 0;
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            handleFlags7(NESOB.a, a);
            NESOB.pc += 3;
            NESOB.cycles += 4;
        break;

        case 0x7E:
            NESOB.tempValue16 = NESmemRead(NESOB.pc + 2) << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue16 += NESOB.x;
            NESOB.tempValue = NESmemRead(NESOB.tempValue16);
            NESOB.prevValue = NESOB.tempValue;
            NESOB.Abitbuffer = NESOB.tempValue;
            NESOB.Pbitbuffer = NESOB.pflag;
            NESOB.Xbitbuffer[0] = NESOB.Pbitbuffer[0];
            NESOB.Pbitbuffer[0] = NESOB.Abitbuffer[0];
            NESOB.Abitbuffer = NESOB.Abitbuffer >> 1;
            NESOB.Abitbuffer[7] = NESOB.Xbitbuffer[0];
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            NESOB.tempValue = NESOB.Abitbuffer.to_ulong();
            handleFlags7(NESOB.tempValue, NESOB.prevValue);
            handleFlags1(NESOB.tempValue, NESOB.prevValue);
            NESmemWrite(NESOB.tempValue, NESOB.tempValue16);
            NESOB.pc += 3;
            NESOB.cycles += 7;
        break;

        case 0x81:
            NESOB.tempValue162 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue162 += NESOB.x;
            if(NESOB.tempValue162 >= 0x0100)
            {
                NESOB.tempValue162 -= 0x0100;
            }
            NESOB.tempValue2 = NESmemRead(NESOB.tempValue162);
            NESOB.tempValue162++;
            if(NESOB.tempValue162 >= 0x0100)
            {
                NESOB.tempValue162 -= 0x0100;
            }
            NESOB.tempValue = NESmemRead(NESOB.tempValue162);
            NESOB.tempValue16 = NESOB.tempValue << 8 | NESOB.tempValue2;
            NESOB.memory[NESOB.tempValue16] = NESOB.a;
            NESOB.pc += 2;
            NESOB.cycles += 6;
        break;

        case 0x84:
            NESOB.tempValue16 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESmemWrite(NESOB.y, NESOB.tempValue16);
            NESOB.pc += 2;
            NESOB.cycles += 3;
        break;

        case 0x85:
            NESOB.tempValue16 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESmemWrite(NESOB.a, NESOB.tempValue16);
            NESOB.pc += 2;
            NESOB.cycles += 3;
        break;

        case 0x86:
            NESOB.tempValue16 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESmemWrite(NESOB.x, NESOB.tempValue16);
            NESOB.pc += 2;
            NESOB.cycles += 3;
        break;

        case 0x88:
            NESOB.prevValue = NESOB.y;
            NESOB.y--;
            handleFlags7(NESOB.y,NESOB.prevValue);
            handleFlags1(NESOB.y,NESOB.prevValue);
            NESOB.pc += 1;
            NESOB.cycles += 2;
        break;

        case 0x8A:
            NESOB.prevValue = NESOB.a;
            NESOB.a = NESOB.x;
            handleFlags7(NESOB.a, NESOB.prevValue);
            handleFlags1(NESOB.a, NESOB.prevValue);
            NESOB.pc++;
            NESOB.cycles += 2;
        break;

        case 0x8C:
            NESOB.tempValue16 = NESmemRead(NESOB.pc + 2) << 8 | NESmemRead(NESOB.pc + 1);
            NESmemWrite(NESOB.y,NESOB.tempValue16);
            NESOB.pc += 3;
            NESOB.cycles += 4;
        break;

        case 0x8D:
            NESOB.tempValue16 = NESmemRead(NESOB.pc + 2) << 8 | NESmemRead(NESOB.pc + 1);
            NESmemWrite(NESOB.a,NESOB.tempValue16);
            NESOB.pc += 3;
            NESOB.cycles += 4;
        break;

        case 0x8E:
            NESOB.tempValue16 = NESmemRead(NESOB.pc + 2) << 8 | NESmemRead(NESOB.pc + 1);
            NESmemWrite(NESOB.x,NESOB.tempValue16);
            NESOB.pc += 3;
            NESOB.cycles += 4;
        break;

        case 0x90:
            NESOB.Pbitbuffer = NESOB.pflag;
            if(NESOB.Pbitbuffer[0] == 0)
            {
                NESOB.Xbitbuffer = NESmemRead(NESOB.pc + 1);
                if(NESOB.Xbitbuffer[7] == 1)
                {

                    NESOB.pc = NESOB.pc + NESmemRead(NESOB.pc + 1);
                    NESOB.pc = NESOB.pc - 0x0100;
                }
                if(NESOB.Xbitbuffer[7] == 0)
                {
                    NESOB.pc = NESOB.pc + NESmemRead(NESOB.pc + 1);
                }
                NESOB.pc += 2;
                NESOB.cycles += 3;
            }
            if(NESOB.Pbitbuffer[0] == 1)
            {
                NESOB.pc += 2;
                NESOB.cycles += 2;
            }
        break;

        case 0x91:
            NESmemWrite(NESOB.a, indirectYget());
            NESOB.pc += 2;
            NESOB.cycles += 6;
        break;

        case 0x94:
            NESOB.tempValue = NESOB.x + NESmemRead(NESOB.pc + 1);
            NESOB.tempValue16 = 0x00 << 8 | NESOB.tempValue;
            NESmemWrite(NESOB.y, NESOB.tempValue16);
            NESOB.pc += 2;
            NESOB.cycles += 3;
        break;

        case 0x95:
            NESOB.tempValue = NESOB.x + NESmemRead(NESOB.pc + 1);
            NESOB.tempValue16 = 0x00 << 8 | NESOB.tempValue;
            NESmemWrite(NESOB.a, NESOB.tempValue16);
            NESOB.pc += 2;
            NESOB.cycles += 3;
        break;

        case 0x96:
            NESOB.tempValue = NESOB.y + NESmemRead(NESOB.pc + 1);
            NESOB.tempValue16 = 0x00 << 8 | NESOB.tempValue;
            NESmemWrite(NESOB.x, NESOB.tempValue16);
            NESOB.pc += 2;
            NESOB.cycles += 3;
        break;

        case 0x98:
            NESOB.prevValue = NESOB.a;
            NESOB.a = NESOB.y;
            handleFlags7(NESOB.a, NESOB.prevValue);
            handleFlags1(NESOB.a, NESOB.prevValue);
            NESOB.pc++;
            NESOB.cycles += 2;
        break;

        case 0x99:
            NESOB.tempValue16 = NESmemRead(NESOB.pc + 2) << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue16 = NESOB.tempValue16 + NESOB.y;
            NESmemWrite(NESOB.a,NESOB.tempValue16);
            NESOB.pc += 3;
            NESOB.cycles += 4;
        break;

        case 0x9A:
            NESOB.sp = NESOB.x;
            NESOB.pc += 1;
            NESOB.cycles += 2;
        break;

        case 0x9D:
            NESOB.tempValue16 = NESmemRead(NESOB.pc + 2) << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue16 = NESOB.tempValue16 + NESOB.x;
            NESmemWrite(NESOB.a,NESOB.tempValue16);
            NESOB.pc += 3;
            NESOB.cycles += 4;
        break;

        case 0xA0:
            NESOB.prevValue = NESOB.y;
            NESOB.y = NESmemRead(NESOB.pc + 1);
            handleFlags7(NESOB.y,NESOB.prevValue);
            handleFlags1(NESOB.y,NESOB.prevValue);
            NESOB.pc += 2;
            NESOB.cycles += 2;
        break;

        case 0xA1:
            NESOB.tempValue162 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue162 += NESOB.x;
            if(NESOB.tempValue162 >= 0x0100)
            {
                NESOB.tempValue162 -= 0x0100;
            }
            NESOB.tempValue2 = NESmemRead(NESOB.tempValue162);
            NESOB.tempValue162++;
            if(NESOB.tempValue162 >= 0x0100)
            {
                NESOB.tempValue162 -= 0x0100;
            }
            NESOB.tempValue = NESmemRead(NESOB.tempValue162);
            NESOB.tempValue16 = NESOB.tempValue << 8 | NESOB.tempValue2;
            NESOB.a = NESOB.memory[NESOB.tempValue16];
            NESOB.pc += 2;
            NESOB.cycles += 6;
        break;

        case 0xA2:
            NESOB.prevValue = NESOB.x;
            NESOB.x = NESmemRead(NESOB.pc + 1);
            handleFlags7(NESOB.x,NESOB.prevValue);
            handleFlags1(NESOB.x,NESOB.prevValue);
            NESOB.pc += 2;
            NESOB.cycles += 2;
        break;

        case 0xA4:
            NESOB.prevValue = NESOB.y;
            NESOB.tempValue16 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.y = NESmemRead(NESOB.tempValue16);
            handleFlags7(NESOB.y,NESOB.prevValue);
            handleFlags1(NESOB.y,NESOB.prevValue);
            NESOB.pc += 2;
            NESOB.cycles += 2;
        break;

        case 0xA5:
            NESOB.prevValue = NESOB.a;
            NESOB.tempValue16 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.a = NESmemRead(NESOB.tempValue16);
            handleFlags7(NESOB.a,NESOB.prevValue);
            handleFlags1(NESOB.a,NESOB.prevValue);
            NESOB.pc += 2;
            NESOB.cycles += 2;
        break;

        case 0xA6:
            NESOB.prevValue = NESOB.x;
            NESOB.tempValue16 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.x = NESmemRead(NESOB.tempValue16);
            handleFlags7(NESOB.x,NESOB.prevValue);
            handleFlags1(NESOB.x,NESOB.prevValue);
            NESOB.pc += 2;
            NESOB.cycles += 2;
        break;

        case 0xA8:
            NESOB.prevValue = NESOB.y;
            NESOB.y = NESOB.a;
            handleFlags7(NESOB.y, NESOB.prevValue);
            handleFlags1(NESOB.y, NESOB.prevValue);
            NESOB.pc++;
            NESOB.cycles += 2;
        break;

        case 0xA9:
            NESOB.prevValue = NESOB.a;
            NESOB.a = NESmemRead(NESOB.pc + 1);
            handleFlags7(NESOB.a,NESOB.prevValue);
            handleFlags1(NESOB.a,NESOB.prevValue);
            NESOB.pc += 2;
            NESOB.cycles += 2;
        break;

        case 0xAA:
            NESOB.prevValue = NESOB.x;
            NESOB.x = NESOB.a;
            handleFlags7(NESOB.x, NESOB.prevValue);
            handleFlags1(NESOB.x, NESOB.prevValue);
            NESOB.pc++;
            NESOB.cycles += 2;
        break;

        case 0xAC:
            NESOB.prevValue = NESOB.y; // Load PrevValue with A.
            NESOB.y = NESmemRead(NESOB.memory[NESOB.pc + 2] << 8 | NESOB.memory[NESOB.pc + 1]); // Load data in memory at location stored at pc+1 and pc+2.
            handleFlags7(NESOB.y,NESOB.prevValue); // Handles the flags
            handleFlags1(NESOB.y,NESOB.prevValue); // Handles the flags
            NESOB.pc += 3;
            NESOB.cycles += 4;
        break;

        case 0xAD:
            NESOB.prevValue = NESOB.a; // Load PrevValue with A.
            NESOB.tempValue16 = NESOB.memory[NESOB.pc + 2] << 8 | NESOB.memory[NESOB.pc + 1];
            NESOB.a = NESmemRead(NESOB.tempValue16); // Load data in memory at location stored at pc+1 and pc+2.
            handleFlags7(NESOB.a,NESOB.prevValue); // Handles the flags
            handleFlags1(NESOB.a,NESOB.prevValue); // Handles the flags
            NESOB.pc += 3;
            NESOB.cycles += 4;
        break;

        case 0xAE:
            NESOB.prevValue = NESOB.x; // Load PrevValue with A.
            NESOB.x = NESmemRead(NESOB.memory[NESOB.pc + 2] << 8 | NESOB.memory[NESOB.pc + 1]); // Load data in memory at location stored at pc+1 and pc+2.
            handleFlags7(NESOB.x,NESOB.prevValue); // Handles the flags
            handleFlags1(NESOB.x,NESOB.prevValue); // Handles the flags
            NESOB.pc += 3;
            NESOB.cycles += 4;
        break;

        case 0xB0:
            NESOB.Pbitbuffer = NESOB.pflag;
            if(NESOB.Pbitbuffer[0] == 1)
            {
                NESOB.Xbitbuffer = NESmemRead(NESOB.pc + 1);
                if(NESOB.Xbitbuffer[7] == 1)
                {

                    NESOB.pc = NESOB.pc + NESmemRead(NESOB.pc + 1);
                    NESOB.pc = NESOB.pc - 0x0100;
                }
                if(NESOB.Xbitbuffer[7] == 0)
                {
                    NESOB.pc = NESOB.pc + NESmemRead(NESOB.pc + 1);
                }
                NESOB.pc += 2;
                NESOB.cycles += 3;
            }
            if(NESOB.Pbitbuffer[0] == 0)
            {
                NESOB.pc += 2;
                NESOB.cycles += 2;
            }
        break;

        case 0xB1:
            NESOB.a = NESmemRead(indirectYget());
            handleFlags7(NESOB.a,NESOB.prevValue);
            handleFlags1(NESOB.a,NESOB.prevValue);
            NESOB.pc += 2;
            NESOB.cycles += 5;
        break;

        case 0xB4:
            NESOB.prevValue = NESOB.y;
            NESOB.tempValue16 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue16 = NESOB.tempValue16 + NESOB.x;
            if(NESOB.tempValue16 >= 0x0100)
            {
                NESOB.tempValue16 = NESOB.tempValue16 - 0x0100;
            }
            NESOB.y = NESmemRead(NESOB.tempValue16);
            handleFlags7(NESOB.y,NESOB.prevValue);
            handleFlags1(NESOB.y,NESOB.prevValue);
            NESOB.pc += 2;
            NESOB.cycles += 2;
        break;

        case 0xB5:
            NESOB.prevValue = NESOB.a;
            NESOB.tempValue16 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue16 = NESOB.tempValue16 + NESOB.x;
            if(NESOB.tempValue16 >= 0x0100)
            {
                NESOB.tempValue16 = NESOB.tempValue16 - 0x0100;
            }
            NESOB.a = NESmemRead(NESOB.tempValue16);
            handleFlags7(NESOB.a,NESOB.prevValue);
            handleFlags1(NESOB.a,NESOB.prevValue);
            NESOB.pc += 2;
            NESOB.cycles += 2;
        break;

        case 0xB6:
            NESOB.prevValue = NESOB.x;
            NESOB.tempValue16 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue16 = NESOB.tempValue16 + NESOB.y;
            if(NESOB.tempValue16 >= 0x0100)
            {
                NESOB.tempValue16 = NESOB.tempValue16 - 0x0100;
            }
            NESOB.x = NESmemRead(NESOB.tempValue16);
            handleFlags7(NESOB.x,NESOB.prevValue);
            handleFlags1(NESOB.x,NESOB.prevValue);
            NESOB.pc += 2;
            NESOB.cycles += 2;
        break;

        case 0xB8:
            NESOB.Pbitbuffer = NESOB.pflag;
            NESOB.Pbitbuffer[6] = 0;
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            NESOB.pc += 1;
            NESOB.cycles += 2;
        break;

        case 0xB9:
            NESOB.prevValue = NESOB.a; // Load PrevValue with A.
            NESOB.tempValue16 = NESmemRead(NESOB.pc + 2) << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue16 += NESOB.y;
            NESOB.a = NESOB.memory[NESOB.tempValue16]; // Load data in memory at location stored at pc+1 and pc+2 + x.
            handleFlags7(NESOB.a,NESOB.prevValue); // Handles the flags
            handleFlags1(NESOB.a,NESOB.prevValue); // Handles the flags
            NESOB.pc += 3;
            NESOB.cycles += 4;
        break;

        case 0xBA:
            NESOB.prevValue = NESOB.x;
            NESOB.x = NESOB.sp;
            handleFlags7(NESOB.x, NESOB.prevValue);
            handleFlags1(NESOB.x, NESOB.prevValue);
            NESOB.pc++;
            NESOB.cycles += 2;
        break;

        case 0xBC:
            NESOB.prevValue = NESOB.y; // Load PrevValue with A.
            NESOB.tempValue16 = NESOB.memory[NESOB.pc + 2] << 8 | NESOB.memory[NESOB.pc + 1];
            NESOB.tempValue16 += NESOB.x;
            NESOB.y = NESmemRead(NESOB.tempValue16); // Load data in memory at location stored at pc+1 and pc+2.
            handleFlags7(NESOB.y,NESOB.prevValue); // Handles the flags
            handleFlags1(NESOB.y,NESOB.prevValue); // Handles the flags
            NESOB.pc += 3;
            NESOB.cycles += 4;
        break;

        case 0xBD:
            NESOB.prevValue = NESOB.a; // Load PrevValue with A.
            NESOB.tempValue16 = NESOB.memory[NESOB.pc + 2] << 8 | NESOB.memory[NESOB.pc + 1];
            NESOB.tempValue16 += NESOB.x;
            NESOB.a = NESmemRead(NESOB.tempValue16); // Load data in memory at location stored at pc+1 and pc+2 + x.
            handleFlags7(NESOB.a,NESOB.prevValue); // Handles the flags
            handleFlags1(NESOB.a,NESOB.prevValue); // Handles the flags
            NESOB.pc += 3;
            NESOB.cycles += 4;
        break;

        case 0xBE:
            NESOB.prevValue = NESOB.x; // Load PrevValue with A.
            NESOB.tempValue162 = NESOB.memory[NESOB.pc + 2] << 8 | NESOB.memory[NESOB.pc + 1];
            NESOB.tempValue162 += NESOB.y;
            NESOB.x = NESmemRead(NESOB.tempValue162);
            handleFlags7(NESOB.x,NESOB.prevValue); // Handles the flags
            handleFlags1(NESOB.x,NESOB.prevValue); // Handles the flags
            NESOB.pc += 3;
            NESOB.cycles += 4;
        break;

        case 0xC0:
            NESOB.prevValue = NESOB.y;
            NESOB.tempValue = NESOB.y - NESmemRead(NESOB.pc + 1);
            handleFlags7(NESOB.tempValue, NESOB.prevValue);
            handleFlags1(NESOB.tempValue, NESOB.prevValue);
            handleFlags0(NESOB.prevValue, NESmemRead(NESOB.pc + 1));
            NESOB.pc += 2;
            NESOB.cycles += 2;
        break;

        case 0xC1:
            NESOB.prevValue = NESOB.a;
            NESOB.tempValue162 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue162 += NESOB.x;
            if(NESOB.tempValue162 >= 0x0100)
            {
                NESOB.tempValue162 -= 0x0100;
            }
            NESOB.tempValue163 = NESmemRead(NESOB.tempValue162 + 1) << 8 | NESmemRead(NESOB.tempValue162);
            NESOB.tempValue = NESOB.a - NESmemRead(NESOB.tempValue163);
            handleFlags7(NESOB.tempValue, NESOB.prevValue);
            handleFlags1(NESOB.tempValue, NESOB.prevValue);
            handleFlags0(NESOB.prevValue, NESmemRead(NESOB.tempValue163));
            NESOB.pc += 2;
            NESOB.cycles += 6;
        break;

        case 0xC4:
            NESOB.prevValue = NESOB.y;
            NESOB.tempValue16 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue = NESOB.y - NESmemRead(NESOB.tempValue16);
            handleFlags7(NESOB.tempValue, NESOB.prevValue);
            handleFlags1(NESOB.tempValue, NESOB.prevValue);
            handleFlags0(NESOB.prevValue, NESmemRead(NESOB.tempValue16));
            NESOB.pc += 2;
            NESOB.cycles += 3;
        break;

        case 0xC5:
            NESOB.prevValue = NESOB.a;
            NESOB.tempValue16 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue = NESOB.a - NESmemRead(NESOB.tempValue16);
            handleFlags7(NESOB.tempValue, NESOB.prevValue);
            handleFlags1(NESOB.tempValue, NESOB.prevValue);
            handleFlags0(NESOB.prevValue, NESmemRead(NESOB.tempValue16));
            NESOB.pc += 2;
            NESOB.cycles += 3;
        break;

        case 0xC6:
            NESOB.tempValue16 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.prevValue = NESmemRead(NESOB.tempValue16);
            NESmemDec(NESOB.tempValue16);
            handleFlags7(NESmemRead(NESOB.tempValue16),NESOB.prevValue);
            handleFlags1(NESmemRead(NESOB.tempValue16),NESOB.prevValue);
            NESOB.pc += 2;
            NESOB.cycles += 2;
        break;

        case 0xC8:
            NESOB.prevValue = NESOB.y;
            NESOB.y++;
            handleFlags7(NESOB.y,NESOB.prevValue);
            handleFlags1(NESOB.y,NESOB.prevValue);
            NESOB.pc += 1;
            NESOB.cycles += 2;
        break;

        case 0xC9:
            NESOB.prevValue = NESOB.a;
            NESOB.tempValue = NESOB.a - NESmemRead(NESOB.pc + 1);
            handleFlags7(NESOB.tempValue, NESOB.prevValue);
            handleFlags1(NESOB.tempValue, NESOB.prevValue);
            handleFlags0(NESOB.prevValue, NESmemRead(NESOB.pc + 1));
            NESOB.pc += 2;
            NESOB.cycles += 2;
        break;

        case 0xCA:
            NESOB.prevValue = NESOB.x;
            NESOB.x--;
            handleFlags7(NESOB.x,NESOB.prevValue);
            handleFlags1(NESOB.x,NESOB.prevValue);
            NESOB.pc += 1;
            NESOB.cycles += 2;
        break;

        case 0xCC:
            NESOB.prevValue = NESOB.y;
            NESOB.tempValue16 = NESmemRead(NESOB.pc + 2) << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue = NESOB.y - NESmemRead(NESOB.tempValue16);
            handleFlags7(NESOB.tempValue, NESOB.prevValue);
            handleFlags1(NESOB.tempValue, NESOB.prevValue);
            handleFlags0(NESOB.prevValue, NESmemRead(NESOB.tempValue16));
            NESOB.pc += 3;
            NESOB.cycles += 4;
        break;

        case 0xCD:
            NESOB.prevValue = NESOB.a;
            NESOB.tempValue16 = NESmemRead(NESOB.pc + 2) << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue = NESOB.a - NESmemRead(NESOB.tempValue16);
            handleFlags7(NESOB.tempValue, NESOB.prevValue);
            handleFlags1(NESOB.tempValue, NESOB.prevValue);
            handleFlags0(NESOB.prevValue, NESmemRead(NESOB.tempValue16));
            NESOB.pc += 3;
            NESOB.cycles += 4;
        break;

        case 0xCE:
            NESOB.tempValue16 = NESmemRead(NESOB.pc + 2) << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.prevValue = NESmemRead(NESOB.tempValue16);
            NESmemDec(NESOB.tempValue16);
            handleFlags7(NESmemRead(NESOB.tempValue16),NESOB.prevValue);
            handleFlags1(NESmemRead(NESOB.tempValue16),NESOB.prevValue);
            NESOB.pc += 3;
            NESOB.cycles += 6;
        break;

        case 0xD0:
            NESOB.Pbitbuffer = NESOB.pflag;
            if(NESOB.Pbitbuffer[1] == 0)
            {
                NESOB.Xbitbuffer = NESmemRead(NESOB.pc + 1);
                if(NESOB.Xbitbuffer[7] == 1)
                {

                    NESOB.pc = NESOB.pc + NESmemRead(NESOB.pc + 1);
                    NESOB.pc = NESOB.pc - 0x0100;
                }
                if(NESOB.Xbitbuffer[7] == 0)
                {
                    NESOB.pc = NESOB.pc + NESmemRead(NESOB.pc + 1);
                }
                NESOB.pc += 2;
                NESOB.cycles += 3;
            }
            if(NESOB.Pbitbuffer[1] == 1)
            {
                NESOB.pc += 2;
                NESOB.cycles += 2;
            }
        break;

        case 0xD1:
            NESOB.prevValue = NESOB.a;
            NESOB.tempValue = NESOB.a - NESmemRead(indirectYget());
            handleFlags7(NESOB.tempValue, NESOB.prevValue);
            handleFlags1(NESOB.tempValue, NESOB.prevValue);
            handleFlags0(NESOB.prevValue, NESmemRead(indirectYget()));
            NESOB.pc += 2;
            NESOB.cycles += 5;
        break;

        case 0xD5:
            NESOB.prevValue = NESOB.a;
            NESOB.tempValue16 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue16 += NESOB.x;
            if(NESOB.tempValue16 >= 0x0100)
            {
                NESOB.tempValue16 -= 0x0100;
            }
            NESOB.tempValue = NESOB.a - NESmemRead(NESOB.tempValue16);
            handleFlags7(NESOB.tempValue, NESOB.prevValue);
            handleFlags1(NESOB.tempValue, NESOB.prevValue);
            handleFlags0(NESOB.prevValue, NESmemRead(NESOB.tempValue16));
            NESOB.pc += 2;
            NESOB.cycles += 4;
        break;

        case 0xD6:
            NESOB.tempValue16 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue16 = NESOB.tempValue16 + NESOB.x;
            if(NESOB.tempValue16 >= 0x0100)
            {
                NESOB.tempValue16 = NESOB.tempValue16 - 0x0100;
            }
            NESOB.prevValue = NESmemRead(NESOB.tempValue16);
            NESmemDec(NESOB.tempValue16);
            handleFlags7(NESmemRead(NESOB.tempValue16),NESOB.prevValue);
            handleFlags1(NESmemRead(NESOB.tempValue16),NESOB.prevValue);
            NESOB.pc += 2;
            NESOB.cycles += 2;
        break;

        case 0xD8:
            NESOB.Pbitbuffer = NESOB.pflag;
            NESOB.Pbitbuffer[3] = 0;
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            NESOB.pc++;
            NESOB.cycles += 2;
        break;

        case 0xD9:
            NESOB.prevValue = NESOB.a;
            NESOB.tempValue16 = NESmemRead(NESOB.pc + 2) << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue16 += NESOB.y;
            NESOB.tempValue = NESOB.a - NESmemRead(NESOB.tempValue16);
            handleFlags7(NESOB.tempValue, NESOB.prevValue);
            handleFlags1(NESOB.tempValue, NESOB.prevValue);
            handleFlags0(NESOB.prevValue, NESmemRead(NESOB.tempValue16));
            NESOB.pc += 3;
            NESOB.cycles += 4;
        break;

        case 0xDD:
            NESOB.prevValue = NESOB.a;
            NESOB.tempValue16 = NESmemRead(NESOB.pc + 2) << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue16 += NESOB.x;
            NESOB.tempValue = NESOB.a - NESmemRead(NESOB.tempValue16);
            handleFlags7(NESOB.tempValue, NESOB.prevValue);
            handleFlags1(NESOB.tempValue, NESOB.prevValue);
            handleFlags0(NESOB.prevValue, NESmemRead(NESOB.tempValue16));
            NESOB.pc += 3;
            NESOB.cycles += 4;
        break;

        case 0xDE:
            NESOB.tempValue16 = NESmemRead(NESOB.pc + 2) << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue16 += NESOB.x;
            NESOB.prevValue = NESmemRead(NESOB.tempValue16);
            NESmemDec(NESOB.tempValue16);
            handleFlags7(NESmemRead(NESOB.tempValue16),NESOB.prevValue);
            handleFlags1(NESmemRead(NESOB.tempValue16),NESOB.prevValue);
            NESOB.pc += 3;
            NESOB.cycles += 6;
        break;

        case 0xE0:
            NESOB.prevValue = NESOB.x;
            NESOB.tempValue = NESOB.x - NESmemRead(NESOB.pc + 1);
            handleFlags7(NESOB.tempValue, NESOB.prevValue);
            handleFlags1(NESOB.tempValue, NESOB.prevValue);
            handleFlags0(NESOB.prevValue, NESmemRead(NESOB.pc + 1));
            NESOB.pc += 2;
            NESOB.cycles += 2;
        break;

        case 0xE1:
            NESOB.prevValue = NESOB.a;
            NESOB.tempValue162 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue162 += NESOB.x;
            if(NESOB.tempValue162 >= 0x0100)
            {
                NESOB.tempValue162 -= 0x0100;
            }
            NESOB.tempValue163 = NESmemRead(NESOB.tempValue162 + 1) << 8 | NESmemRead(NESOB.tempValue162);
            NESOB.Pbitbuffer = NESOB.pflag;
            a = NESOB.a;
            b = NESmemRead(NESOB.tempValue163);
            diff = a - b - !NESOB.Pbitbuffer[0];
            NESOB.a = (uint8_t)diff;
            NESOB.Pbitbuffer[1] = 0;
            if(NESOB.a == 0x00 && a != 0x00)
            {
                NESOB.Pbitbuffer[1] = 1;
            }
            NESOB.Pbitbuffer[0] = diff >= 0x00;
            NESOB.Pbitbuffer[6] = ((a^b)&0x80) != 0 && ((a^NESOB.a)&0x80) != 0;
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            handleFlags7(diff, b);
            NESOB.pc += 2;
            NESOB.cycles += 6;
        break;

        case 0xE4:
            NESOB.prevValue = NESOB.x;
            NESOB.tempValue16 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue = NESOB.x - NESmemRead(NESOB.tempValue16);
            handleFlags7(NESOB.tempValue, NESOB.prevValue);
            handleFlags1(NESOB.tempValue, NESOB.prevValue);
            handleFlags0(NESOB.prevValue, NESmemRead(NESOB.tempValue16));
            NESOB.pc += 2;
            NESOB.cycles += 2;
        break;

        case 0xE5:
            NESOB.tempValue16 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.Pbitbuffer = NESOB.pflag;
            a = NESOB.a;
            b = NESmemRead(NESOB.tempValue16);
            diff = a - b - !NESOB.Pbitbuffer[0];
            NESOB.a = (uint8_t)diff;
            NESOB.Pbitbuffer[1] = 0;
            if(NESOB.a == 0x00 && a != 0x00)
            {
                NESOB.Pbitbuffer[1] = 1;
            }
            NESOB.Pbitbuffer[0] = diff >= 0x00;
            NESOB.Pbitbuffer[6] = ((a^b)&0x80) != 0 && ((a^NESOB.a)&0x80) != 0;
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            handleFlags7(diff, b);
            NESOB.pc += 2;
            NESOB.cycles += 3;
        break;

        case 0xE6:
            NESOB.tempValue16 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.prevValue = NESmemRead(NESOB.tempValue16);
            NESmemInc(NESOB.tempValue16);
            handleFlags7(NESmemRead(NESOB.tempValue16),NESOB.prevValue);
            handleFlags1(NESmemRead(NESOB.tempValue16),NESOB.prevValue);
            NESOB.pc += 2;
            NESOB.cycles += 2;
        break;

        case 0xE8:
            NESOB.prevValue = NESOB.x;
            NESOB.x++;
            handleFlags7(NESOB.x,NESOB.prevValue);
            handleFlags1(NESOB.x,NESOB.prevValue);
            NESOB.Pbitbuffer = NESOB.pflag;
            NESOB.Pbitbuffer[0] = NESOB.Pbitbuffer[1];
            NESOB.pc += 1;
            NESOB.cycles += 2;
        break;

        case 0xE9:
            NESOB.Pbitbuffer = NESOB.pflag;
            a = NESOB.a;
            b = NESmemRead(NESOB.pc + 1);
            diff = a - b - !NESOB.Pbitbuffer[0];
            NESOB.a = (uint8_t)diff;
            NESOB.Pbitbuffer[1] = 0;
            if(NESOB.a == 0x00 && a != 0x00)
            {
                NESOB.Pbitbuffer[1] = 1;
            }
            NESOB.Pbitbuffer[0] = diff >= 0x00;
            NESOB.Pbitbuffer[6] = ((a^b)&0x80) != 0 && ((a^NESOB.a)&0x80) != 0;
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            handleFlags7(diff, b);
            NESOB.pc += 2;
            NESOB.cycles += 2;
        break;

        case 0xEA:
            NESOB.pc += 1;
            NESOB.cycles += 2;
        break;

        case 0xEC:
            NESOB.prevValue = NESOB.x;
            NESOB.tempValue16 = NESmemRead(NESOB.pc + 2) << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue = NESOB.x - NESmemRead(NESOB.tempValue16);
            handleFlags7(NESOB.tempValue, NESOB.prevValue);
            handleFlags1(NESOB.tempValue, NESOB.prevValue);
            handleFlags0(NESOB.prevValue, NESmemRead(NESOB.tempValue16));
            NESOB.pc += 3;
            NESOB.cycles += 4;
        break;

        case 0xED:
            NESOB.tempValue16 = NESmemRead(NESOB.pc + 2) << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.Pbitbuffer = NESOB.pflag;
            a = NESOB.a;
            b = NESmemRead(NESOB.tempValue16);
            diff = a - b - !NESOB.Pbitbuffer[0];
            NESOB.a = (uint8_t)diff;
            NESOB.Pbitbuffer[1] = 0;
            if(NESOB.a == 0x00 && a != 0x00)
            {
                NESOB.Pbitbuffer[1] = 1;
            }
            NESOB.Pbitbuffer[0] = diff >= 0x00;
            NESOB.Pbitbuffer[6] = ((a^b)&0x80) != 0 && ((a^NESOB.a)&0x80) != 0;
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            handleFlags7(diff, b);
            NESOB.pc += 3;
            NESOB.cycles += 4;
        break;

        case 0xEE:
            NESOB.tempValue16 = NESmemRead(NESOB.pc + 2) << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.prevValue = NESmemRead(NESOB.tempValue16);
            NESmemInc(NESOB.tempValue16);
            handleFlags7(NESmemRead(NESOB.tempValue16),NESOB.prevValue);
            handleFlags1(NESmemRead(NESOB.tempValue16),NESOB.prevValue);
            NESOB.pc += 3;
            NESOB.cycles += 6;
        break;

        case 0xF0:
            NESOB.Pbitbuffer = NESOB.pflag;
            if(NESOB.Pbitbuffer[1] == 1)
            {
                NESOB.Xbitbuffer = NESmemRead(NESOB.pc + 1);
                if(NESOB.Xbitbuffer[7] == 1)
                {

                    NESOB.pc = NESOB.pc + NESmemRead(NESOB.pc + 1);
                    NESOB.pc = NESOB.pc - 0x0100;
                }
                if(NESOB.Xbitbuffer[7] == 0)
                {
                    NESOB.pc = NESOB.pc + NESmemRead(NESOB.pc + 1);
                }
                NESOB.pc += 2;
                NESOB.cycles += 3;
            }
            if(NESOB.Pbitbuffer[1] == 0)
            {
                NESOB.pc += 2;
                NESOB.cycles += 2;
            }
        break;

        case 0xF1:
            NESOB.Pbitbuffer = NESOB.pflag;
            a = NESOB.a;
            b = NESmemRead(indirectYget());
            diff = a - b - !NESOB.Pbitbuffer[0];
            NESOB.a = (uint8_t)diff;
            NESOB.Pbitbuffer[1] = 0;
            if(NESOB.a == 0x00 && a != 0x00)
            {
                NESOB.Pbitbuffer[1] = 1;
            }
            NESOB.Pbitbuffer[0] = diff >= 0x00;
            NESOB.Pbitbuffer[6] = ((a^b)&0x80) != 0 && ((a^NESOB.a)&0x80) != 0;
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            handleFlags7(diff, b);
            NESOB.pc += 2;
            NESOB.cycles += 5;
        break;

        case 0xF5:
            NESOB.prevValue = NESOB.a;
            NESOB.tempValue162 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue162 += NESOB.x;
            if(NESOB.tempValue162 >= 0x0100)
            {
                NESOB.tempValue162 -= 0x0100;
            }
            NESOB.Pbitbuffer = NESOB.pflag;
            a = NESOB.a;
            b = NESmemRead(NESOB.tempValue162);
            diff = a - b - !NESOB.Pbitbuffer[0];
            NESOB.a = (uint8_t)diff;
            NESOB.Pbitbuffer[1] = 0;
            if(NESOB.a == 0x00 && a != 0x00)
            {
                NESOB.Pbitbuffer[1] = 1;
            }
            NESOB.Pbitbuffer[0] = diff >= 0x00;
            NESOB.Pbitbuffer[6] = ((a^b)&0x80) != 0 && ((a^NESOB.a)&0x80) != 0;
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            handleFlags7(diff, b);
            NESOB.pc += 2;
            NESOB.cycles += 4;
        break;

        case 0xF6:
            NESOB.tempValue16 = 0x00 << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue16 = NESOB.tempValue16 + NESOB.x;
            if(NESOB.tempValue16 >= 0x0100)
            {
                NESOB.tempValue16 = NESOB.tempValue16 - 0x0100;
            }
            NESOB.prevValue = NESmemRead(NESOB.tempValue16);
            NESmemInc(NESOB.tempValue16);
            handleFlags7(NESmemRead(NESOB.tempValue16),NESOB.prevValue);
            handleFlags1(NESmemRead(NESOB.tempValue16),NESOB.prevValue);
            NESOB.pc += 2;
            NESOB.cycles += 2;
        break;

        case 0xF8:
            NESOB.Pbitbuffer = NESOB.pflag;
            NESOB.Pbitbuffer[3] = 1;
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            NESOB.pc++;
            NESOB.cycles += 2;
        break;

        case 0xF9:
            NESOB.tempValue16 = NESmemRead(NESOB.pc + 2) << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue16 += NESOB.y;
            NESOB.Pbitbuffer = NESOB.pflag;
            a = NESOB.a;
            b = NESOB.memory[NESOB.tempValue16];
            diff = a - b - !NESOB.Pbitbuffer[0];
            NESOB.a = (uint8_t)diff;
            NESOB.Pbitbuffer[1] = 0;
            if(NESOB.a == 0x00 && a != 0x00)
            {
                NESOB.Pbitbuffer[1] = 1;
            }
            NESOB.Pbitbuffer[0] = diff >= 0x00;
            NESOB.Pbitbuffer[6] = ((a^b)&0x80) != 0 && ((a^NESOB.a)&0x80) != 0;
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            handleFlags7(diff, b);
            NESOB.pc += 3;
            NESOB.cycles += 4;
        break;

        case 0xFD:
            NESOB.tempValue16 = NESmemRead(NESOB.pc + 2) << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue16 += NESOB.x;
            NESOB.Pbitbuffer = NESOB.pflag;
            a = NESOB.a;
            b = NESmemRead(NESOB.tempValue16);
            diff = a - b - !NESOB.Pbitbuffer[0];
            NESOB.a = (uint8_t)diff;
            NESOB.Pbitbuffer[1] = 0;
            if(NESOB.a == 0x00 && a != 0x00)
            {
                NESOB.Pbitbuffer[1] = 1;
            }
            NESOB.Pbitbuffer[0] = diff >= 0x00;
            NESOB.Pbitbuffer[6] = ((a^b)&0x80) != 0 && ((a^NESOB.a)&0x80) != 0;
            NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
            handleFlags7(diff, b);
            NESOB.pc += 3;
            NESOB.cycles += 4;
        break;

        case 0xFE:
            NESOB.tempValue16 = NESmemRead(NESOB.pc + 2) << 8 | NESmemRead(NESOB.pc + 1);
            NESOB.tempValue16 += NESOB.x;
            NESOB.prevValue = NESmemRead(NESOB.tempValue16);
            NESmemInc(NESOB.tempValue16);
            handleFlags7(NESmemRead(NESOB.tempValue16),NESOB.prevValue);
            handleFlags1(NESmemRead(NESOB.tempValue16),NESOB.prevValue);
            NESOB.pc += 3;
            NESOB.cycles += 7;
        break;

        default:
            printf("Unknown Opcode 0x%X!\n",NESOB.opcode);
            printf("Cycles: %i\n",NESOB.cycles);
            printf("Program Counter: 0x%X\n",NESOB.pc);
            breakpoint = true;
        break;
    }
    return 0;
}
