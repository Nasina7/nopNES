#include "cpu.hpp"

using namespace std;

int main()
{
    uint8_t scanlineTimer;
    uint8_t prevScanlineTimer;
    beginning();
    nesPowerOn();
    NESOB.scanline = 0;
    while(NESOB.dontSetTrue == false) // Begin Loop
    {
        NESOB.opcode = NESOB.memory[NESOB.pc];
        prevScanlineTimer = NESOB.cycles % 134;
        doOpcode();
        scanlineTimer = NESOB.cycles % 134;
        if(prevScanlineTimer > scanlineTimer)
        {
            NESOB.scanline++;
            if(NESOB.scanline >= 241)
            {
                NESOB.Pbitbuffer = NESOB.memory[0x2002];
                NESOB.Pbitbuffer[7] = 1;
                NESOB.memory[0x2002] = NESOB.Pbitbuffer.to_ulong();
            }
            if(NESOB.scanline == 261)
            {
                NESOB.scanline = 0x00;
            }
        }
        printRegs();
        if(NESOB.closeProgram == true)
        {
            return 0;
        }
    }
    return 0;
}
