#include <iostream>
#include <string>
#include <bitset>
#include <SDL2/SDL.h>
#ifdef __linux__
    #include <thread>
#endif // __linux__
#ifdef _WIN32
    #include "mingw.thread.h"
#endif // _WIN32
#include <unordered_map>
int cycleModulo = 113; // 134 113 90
std::bitset<8> controlBuffer;
uint64_t currentFrame;
uint64_t currentPPUFrame;
bool MMC3mirror;
bool newFrame;
bool exitLoop;
bool newOpcode;
bool beginOpLoop;
bool newOpcode2;
uint16_t nametableAddr;
class NESCPU
{
    public:
        int8_t sign8;
        int8_t sign82;
        int16_t sign16;
        int16_t sign162;
        uint8_t memory[0x10001];
        uint8_t PPUmemory[0x4000];
        uint8_t VRAMaddresslow;
        uint8_t VRAMaddresshigh;
        uint8_t PPUwritecounter;
        uint16_t VRAMaddress;
        uint16_t VRAMaddress2;
        uint8_t header[0x10];
        uint8_t a;
        uint8_t x;
        uint8_t y;
        uint16_t pc;
        uint8_t sp;
        unsigned char pflag;
        uint8_t tempValue;
        uint8_t tempValue2;
        uint16_t tempValue16;
        uint16_t tempValue162;
        uint16_t tempValue163;
        char filename[50];
        int prgsize;
        int chrsize;
        int chrlocate;
        uint8_t opcode;
        bool dontSetTrue = false;
        bool closeProgram = false;
        std::bitset<8> Abitbuffer;
        std::bitset<8> Xbitbuffer;
        std::bitset<8> Ybitbuffer;
        std::bitset<8> Sbitbuffer;
        std::bitset<8> Pbitbuffer;
        int64_t cycles;
        uint8_t prevValue;
        uint8_t lowerPC;
        uint8_t higherPC;
        uint16_t scanline;
};
uint8_t a;
uint8_t b;
int16_t diff;
NESCPU NESOB;
uint8_t OAMmem[0x100];
std::bitset<16> tempBitBuffer16;
bool showGrid;
bool showBlock;
bool showTile;
bool restartNopNES;
FILE* pal = NULL;
void memDump()
{
    FILE* mem_dump = fopen ("log/memdump", "w+");
    fwrite (NESOB.memory , sizeof(char), sizeof(NESOB.memory), mem_dump);
    fclose (mem_dump);
    FILE* mem_dump2 = fopen ("log/memdumpPPU", "w+");
    fwrite (NESOB.PPUmemory , sizeof(char), sizeof(NESOB.PPUmemory), mem_dump2);
    fclose (mem_dump2);
    FILE* mem_dump3 = fopen ("log/memdumpOAM", "w+");
    fwrite (OAMmem , sizeof(char), sizeof(OAMmem), mem_dump3);
    fclose (mem_dump3);
}
bool breakpoint;
SDL_Window* nopNESwindow = NULL;
SDL_Window* nopNESwindowDEBUG = NULL;
SDL_Renderer* renderer;
SDL_Renderer* rendererDEBUG;
SDL_Event SDL_EVENT_HANDLING;
int option2;
using namespace std;
int printRegs()
{
    printf("Opcode: 0x%X\n",NESOB.opcode);
    printf("A: 0x%X\n",NESOB.a);
    printf("X: 0x%X\n",NESOB.x);
    printf("Y: 0x%X\n",NESOB.y);
    printf("SP: 0x%X\n",NESOB.sp);
    NESOB.Pbitbuffer = NESOB.pflag;
    #ifdef __linux__
        std::cout<<"pflag: "<<NESOB.Pbitbuffer<<std::endl;
    #endif
    printf("PC: 0x%X\n",NESOB.pc);
    return true;
}
void handleFlags7(uint8_t value, uint8_t prevValue)
{
    NESOB.Abitbuffer = prevValue;
    NESOB.Pbitbuffer = NESOB.pflag;
    NESOB.Xbitbuffer = value;
    NESOB.Pbitbuffer[7] = NESOB.Xbitbuffer[7];
    NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
}
void handleFlags1(uint8_t value, uint8_t prevValue)
{
    NESOB.Abitbuffer = prevValue;
    NESOB.Pbitbuffer = NESOB.pflag;
    NESOB.Xbitbuffer = value;
    NESOB.Pbitbuffer[1] = 0;
    if(value == 0)
    {
        NESOB.Pbitbuffer[1] = 1;
    }
    NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
}
void handleFlags0(uint8_t prevValue, uint8_t subValue)
{
    NESOB.Abitbuffer = subValue;
    NESOB.Pbitbuffer = NESOB.pflag;
    NESOB.Xbitbuffer = prevValue;
    if(prevValue >= subValue)
    {
        NESOB.Pbitbuffer[0] = 1;
    }
    if(prevValue < subValue)
    {
        NESOB.Pbitbuffer[0] = 0;
    }
    NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
}
void handleFlags(uint8_t value, uint8_t prevValue)
{
    NESOB.Abitbuffer = prevValue;
    NESOB.Pbitbuffer = NESOB.pflag;
    NESOB.Xbitbuffer = value;
    if(NESOB.Xbitbuffer[7] == 1 && NESOB.Abitbuffer[7] != 1)
    {
        NESOB.Pbitbuffer[7] = 1;
    }
    if(NESOB.Xbitbuffer[7] == 0)
    {
        NESOB.Pbitbuffer[7] = 0;
    }
    if(value == 0)
    {
        NESOB.Pbitbuffer[1] = 1;
    }
    if(value != 0)
    {
        NESOB.Pbitbuffer[1] = 0;
    }
    NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
}
bool beginBenchmark;
uint64_t currentFpsonStart;
uint64_t currentFpsonEnd;
#include <unistd.h>
bool NMIthrottle;
uint8_t scanlineTimer;
uint8_t prevScanlineTimer;
char options;
bool renderThrottle;
bool NMIrequest;
bool read2002vb;
bool throttleCPU = false;
bool doneThrottle = false;
bool reach261 = false;
uint8_t tempVALUE;
uint8_t tempVALUE2;
void throttleCPUfunc()
{
    throttleCPU = false;
    SDL_Delay(16);
    while(throttleCPU == false)
    {

    }
    doneThrottle = true;


}
uint32_t switchLocation;
uint16_t helpLocation;
std::bitset<4> mapperBit;
std::bitset<4> mapperBit2;
FILE* rom = NULL;
int bankSwitch32k(uint8_t value)
{
    switchLocation = value * 0x8000;
    rom = fopen(NESOB.filename, "rb");
    fseek(rom,0x10 + switchLocation,SEEK_SET);
    fread(NESOB.memory + 0x8000,0x8000,1,rom);
    fclose(rom);
    return true;
}
int bankSwitch16k(uint8_t value)
{
    switchLocation = value * 0x4000;
    rom = fopen(NESOB.filename, "rb");
    fseek(rom,0x10 + switchLocation,SEEK_SET);
    fread(NESOB.memory + 0x8000,0x4000,1,rom); // Load first bank into 0x8000 to 0xBFFF
    fclose(rom);
    return true;
}
int bankSwitchPRG(uint8_t value, uint16_t beginlocation, uint16_t sizeval)
{
    switchLocation = value * 0x2000;
    rom = fopen(NESOB.filename, "rb");
    fseek(rom,0x10 + switchLocation,SEEK_SET);
    fread(NESOB.memory + beginlocation,sizeval,1,rom);
    fclose(rom);
    return true;
}
int bankSwitch16k2(uint8_t value)
{
    switchLocation = value * 0x4000;
    rom = fopen(NESOB.filename, "rb");
    fseek(rom,0x10 + switchLocation,SEEK_SET);
    fread(NESOB.memory + 0xC000,0x4000,1,rom); // Load first bank into 0xC000 to 0xFFFF
    fclose(rom);
    return true;
}
int fixSecondLastBankMMC3(uint16_t locationval, uint16_t sizeval)
{
    switchLocation = (NESOB.prgsize) - 0x4000;
    rom = fopen(NESOB.filename, "rb");
    fseek(rom,0x10 + switchLocation,SEEK_SET);
    fread(NESOB.memory + locationval,sizeval,1,rom); // Load first bank into 0xC000 to 0xFFFF
    fclose(rom);
    return true;
}
int fixLastBank()
{
    switchLocation = (NESOB.prgsize) - 0x4000;
    rom = fopen(NESOB.filename, "rb");
    fseek(rom,0x10 + switchLocation,SEEK_SET);
    fread(NESOB.memory + 0xC000,0x4000,1,rom); // Load first bank into 0xC000 to 0xFFFF
    fclose(rom);
    return true;
}
int CHRbankSwitch4k(uint8_t value)
{
    switchLocation = (value * 0x1000) + NESOB.prgsize;
    rom = fopen(NESOB.filename, "rb");
    fseek(rom,0x10 + switchLocation,SEEK_SET);
    fread(NESOB.PPUmemory + 0x0000,0x1000,1,rom);
    fclose(rom);
    return true;
}
int CHRbankSwitch(uint8_t value, uint16_t locationval, uint16_t sizeval)
{
    switchLocation = (value * 0x400) + NESOB.prgsize;
    rom = fopen(NESOB.filename, "rb");
    fseek(rom,0x10 + switchLocation,SEEK_SET);
    fread(NESOB.PPUmemory + locationval,sizeval,1,rom);
    fclose(rom);
    return true;
}
int CHRbankSwitch2(uint8_t value, uint16_t locationval, uint16_t sizeval, uint16_t multip)
{
    switchLocation = (value * multip) + NESOB.prgsize;
    rom = fopen(NESOB.filename, "rb");
    fseek(rom,0x10 + switchLocation,SEEK_SET);
    fread(NESOB.PPUmemory + locationval,sizeval,1,rom);
    fclose(rom);
    return true;
}
int CHRbankSwitch8k(uint8_t value)
{
    switchLocation = (value * 0x2000) + NESOB.prgsize;
    rom = fopen(NESOB.filename, "rb");
    fseek(rom,0x10 + switchLocation,SEEK_SET);
    fread(NESOB.PPUmemory + 0x0000,0x2000,1,rom);
    fclose(rom);
    return true;
}
int CHRbankSwitch4khigh(uint8_t value)
{
    switchLocation = (value * 0x1000) + NESOB.prgsize;
    rom = fopen(NESOB.filename, "rb");
    fseek(rom,0x10 + switchLocation,SEEK_SET);
    fread(NESOB.PPUmemory + 0x1000,0x1000,1,rom);
    fclose(rom);
    return true;
}
int handleScanlineStuff()
{
        if(NESOB.scanline == 241 && renderThrottle == false)
        {
            renderThrottle = true;
            //newFrame = true;
            currentFrame++;
            //printf("Current Frame: %i\n",currentFrame);
        }
        if(NESOB.scanline == 242)
        {
            renderThrottle = false;
        }
        scanlineTimer = NESOB.cycles % cycleModulo; // 134
        if(prevScanlineTimer > scanlineTimer)
        {
            NESOB.scanline++;
            newFrame = true;
            if(NESOB.scanline >= 241)
            {
                NESOB.Pbitbuffer = NESOB.memory[0x2002];
                NESOB.Pbitbuffer[7] = 1;
                NESOB.memory[0x2002] = NESOB.Pbitbuffer.to_ulong();
            }
            if(NESOB.scanline >= 241 && NMIthrottle == false)
            {
                NMIthrottle = true;
                NMIrequest = true;
            }
            if(NESOB.scanline == 261)
            {
                read2002vb = false;
                NMIthrottle = false;
                NESOB.Pbitbuffer = NESOB.memory[0x2002];
                NESOB.Pbitbuffer[7] = 0;
                NESOB.memory[0x2002] = NESOB.Pbitbuffer.to_ulong();
                NESOB.scanline = 0x00;
            }
        }
    return 0;
}
uint64_t curFPS;
uint64_t endFPS;
char windowTitle[30];
int doBenchmark()
{
    beginBenchmark = true;
    currentFpsonStart = currentFrame;
    curFPS = currentPPUFrame;
    sleep(1);
    endFPS = currentPPUFrame;
    currentFpsonEnd = currentFrame;
    endFPS = endFPS - curFPS;
    currentFpsonEnd = currentFpsonEnd - currentFpsonStart;
    //printf("Acomplished %i CPU FPS in the time it should take to do 60 CPU FPS!\n", currentFpsonEnd);
    //printf("Acomplished %i PPU FPS in the time it should take to do 60 PPU FPS!\n", endFPS);
    sprintf(windowTitle, "nopNES (Alpha) FPS: %i", currentFpsonEnd);
    SDL_SetWindowTitle(nopNESwindow, windowTitle);
    beginBenchmark = false;
    doBenchmark();
}
void fpsBenchmark()
{
        std::thread benchmark(doBenchmark);
        benchmark.detach();
}

std::bitset<2> controlerpoll;
uint8_t scrollwrite;
uint8_t scrollx;
uint8_t scrolly;
uint8_t scrolly2;
uint8_t locationMod8;
uint16_t location8;
int indirectYget()
{
    NESOB.prevValue = NESOB.a;
    NESOB.tempValue16 = 0x00 << 8 | NESOB.memory[NESOB.pc + 1];
    NESOB.tempValue = NESOB.memory[NESOB.tempValue16];
    NESOB.tempValue2 = NESOB.memory[NESOB.tempValue16 + 1];
    if(NESOB.tempValue16 == 0x00FF)
    {
        NESOB.tempValue16++;
        NESOB.tempValue16 -= 0x0100;
        NESOB.tempValue2 = NESOB.memory[NESOB.tempValue16];
    }
    NESOB.tempValue162 = NESOB.tempValue2 << 8 | NESOB.tempValue;
    NESOB.tempValue162 = NESOB.tempValue162 + NESOB.y;
    return NESOB.tempValue162;
}
bool graphicThread;
uint8_t oamAddr;
uint8_t oamDMA;
uint16_t oamCounter;
uint16_t oamDMAlocate;
uint8_t bitcountS;
uint8_t mapper;
uint8_t shiftReg;
std::bitset<8> MMC1help1;
std::bitset<8> MMC1help2;
uint8_t MMC1counter = 0xFF;
uint8_t MMC1prgmode = 3;
bool CHRbankmode = false;
uint8_t prgSwitch;
std::bitset<8> helpmapBitBuffer;
uint8_t MMC3mod;
uint8_t MMC3bankselect;
std::bitset<8> MMC3bitset;
uint8_t MMC3bankbit;
bool MMC3prgbankbit;
bool MMC3chrconvert;
uint16_t prevval;
bool handleRomWrite(uint8_t value, uint16_t location)
{
    switch(mapper)
    {
        case 0x00:
            return false;
        break;

        case 0x01:
            MMC1help1 = value;
            if(value >= 0x80)
            {
                MMC1counter = 0xFF;
                //printf("RESETCOUNT!\n");
            }
            if(value < 0x80)
            {
                MMC1counter++;
            }
            MMC1help2 = shiftReg;
            MMC1help2[MMC1counter] = MMC1help1[0];
            //printf("MMC1count: 0x%X\n",MMC1counter);
            if(MMC1counter == 4)
            {
                MMC1counter = 0xFF;
                //printf("ShiftReg: 0x%X\n",shiftReg);
                //printf("Location: 0x%X\n",location);
                if(location >= 0x8000 && location <= 0x9FFF)
                {
                    MMC1help1 = shiftReg;
                    if(MMC1help1[3] == 0 && MMC1help1[2] == 0)
                    {
                        MMC1prgmode = 0;
                    }
                    if(MMC1help1[3] == 0 && MMC1help1[2] == 1)
                    {
                        MMC1prgmode = 1;
                    }
                    if(MMC1help1[3] == 1 && MMC1help1[2] == 0)
                    {
                        MMC1prgmode = 2;
                    }
                    if(MMC1help1[3] == 1 && MMC1help1[2] == 1)
                    {
                        MMC1prgmode = 3;
                    }
                    CHRbankmode = MMC1help1[4];
                }
                if(location >= 0xA000 && location <= 0xBFFF)
                {
                    if(CHRbankmode == true)
                    {
                        CHRbankSwitch4k(shiftReg);
                    }
                    if(CHRbankmode == false)
                    {
                        MMC1help2 = shiftReg;
                        MMC1help2[0] = 0;
                        shiftReg = MMC1help2.to_ulong();
                        CHRbankSwitch8k(shiftReg);
                    }
                }
                if(location >= 0xC000 && location <= 0xDFFF)
                {
                    if(CHRbankmode == true)
                    {
                        CHRbankSwitch4khigh(shiftReg);
                    }
                    if(CHRbankmode == false)
                    {
                        //CHRbankSwitch8k(shiftReg);
                    }
                }
                if(location >= 0xE000 && location <= 0xFFFF)
                {
                    MMC1help1 = shiftReg;
                    MMC1help1[4] = 0;
                    if(MMC1prgmode == 0 || MMC1prgmode == 1)
                    {
                        MMC1help1[0] = 0;
                        MMC1help1 = MMC1help1 >> 1;
                        //printf("testing\n");
                        prgSwitch = MMC1help1.to_ulong();
                        bankSwitch32k(prgSwitch);
                    }
                    if(MMC1prgmode == 2)
                    {
                        prgSwitch = MMC1help1.to_ulong();
                        bankSwitch16k(0x00);
                        bankSwitch16k2(prgSwitch);
                    }
                    if(MMC1prgmode == 3)
                    {
                        prgSwitch = MMC1help1.to_ulong();
                        bankSwitch16k(prgSwitch);
                        fixLastBank();
                    }
                }
            }
            shiftReg = MMC1help2.to_ulong();
        break;

        case 0x02:
            bankSwitch16k(value);
        break;

        case 0x03:
            CHRbankSwitch2(value, 0x0000, 0x2000, 0x2000);
        break;

        case 0x04:
            switch(location)
            {
                case 0x8000 ... 0x9FFF:
                    MMC3mod = location % 2;
                        if(MMC3mod == 0) // If location is an even number
                        {
                            MMC3bitset = value;
                            MMC3bitset[7] = 0;
                            MMC3bitset[6] = 0;
                            MMC3bitset[5] = 0;
                            MMC3bitset[4] = 0;
                            MMC3bitset[3] = 0;
                            MMC3bankbit = MMC3bitset.to_ulong();
                            MMC3bitset = value;
                            MMC3prgbankbit = MMC3bitset[6];
                            MMC3chrconvert = MMC3bitset[7];
                        }
                        if(MMC3mod == 1) // If location is an odd number
                        {
                            //cout<<MMC3chrconvert<<endl;
                            MMC3bankselect = value;
                            if(MMC3bankbit == 0)
                            {
                                if(MMC3chrconvert == 0)
                                {
                                    CHRbankSwitch(value, 0x0000, 0x800);
                                }
                                if(MMC3chrconvert == 1)
                                {
                                    CHRbankSwitch(value, 0x1000, 0x800);
                                }
                            }
                            if(MMC3bankbit == 1)
                            {
                                if(MMC3chrconvert == 0)
                                {
                                    CHRbankSwitch(value, 0x0800, 0x800);
                                }
                                if(MMC3chrconvert == 1)
                                {
                                    CHRbankSwitch(value, 0x1800, 0x800);
                                }
                            }
                            if(MMC3bankbit == 2)
                            {
                                if(MMC3chrconvert == 0)
                                {
                                    CHRbankSwitch(value, 0x1000, 0x400);
                                }
                                if(MMC3chrconvert == 1)
                                {
                                    CHRbankSwitch(value, 0x0000, 0x400);
                                }
                            }
                            if(MMC3bankbit == 3)
                            {
                                if(MMC3chrconvert == 0)
                                {
                                    CHRbankSwitch(value, 0x1400, 0x400);
                                }
                                if(MMC3chrconvert == 1)
                                {
                                    CHRbankSwitch(value, 0x0400, 0x400);
                                }
                            }
                            if(MMC3bankbit == 4)
                            {
                                if(MMC3chrconvert == 0)
                                {
                                    CHRbankSwitch(value, 0x1800, 0x400);
                                }
                                if(MMC3chrconvert == 1)
                                {
                                    CHRbankSwitch(value, 0x0800, 0x400);
                                }
                            }
                            if(MMC3bankbit == 5)
                            {
                                if(MMC3chrconvert == 0)
                                {
                                    CHRbankSwitch(value, 0x1C00, 0x400);
                                }
                                if(MMC3chrconvert == 1)
                                {
                                    CHRbankSwitch(value, 0x0C00, 0x400);
                                }
                            }
                            if(MMC3bankbit == 6)
                            {
                                if(MMC3prgbankbit == 0)
                                {
                                    //printf("10x%X\n", value);
                                    //breakpoint = true;
                                    bankSwitchPRG(MMC3bankselect, 0x8000, 0x2000);
                                    fixSecondLastBankMMC3(0xC000, 0x2000);
                                }
                                if(MMC3prgbankbit == 1)
                                {
                                    //printf("20x%X\n", value);
                                    bankSwitchPRG(MMC3bankselect, 0xC000, 0x2000);
                                    fixSecondLastBankMMC3(0x8000, 0x2000);
                                }
                            }
                            if(MMC3bankbit == 7)
                            {
                                //printf("30x%X\n", value);
                                bankSwitchPRG(MMC3bankselect, 0xA000, 0x2000);
                            }
                        }
                break;
                case 0xA000 ... 0xBFFF:
                    if(location % 2 == 0)
                    {
                        MMC3bitset = value;
                        MMC3mirror = MMC3bitset[0];
                    }
                break;
            }
        break;

        case 0x42:
            helpmapBitBuffer = value;
            if(helpmapBitBuffer[5] == 0 && helpmapBitBuffer[4] == 0)
            {
                bankSwitch32k(0);
            }
            if(helpmapBitBuffer[5] == 0 && helpmapBitBuffer[4] == 1)
            {
                bankSwitch32k(1);
            }
            if(helpmapBitBuffer[5] == 1 && helpmapBitBuffer[4] == 0)
            {
                bankSwitch32k(2);
            }
            if(helpmapBitBuffer[5] == 1 && helpmapBitBuffer[4] == 1)
            {
                bankSwitch32k(3);
            }

            if(helpmapBitBuffer[1] == 0 && helpmapBitBuffer[0] == 0)
            {
                CHRbankSwitch8k(0);
            }
            if(helpmapBitBuffer[1] == 0 && helpmapBitBuffer[0] == 1)
            {
                CHRbankSwitch8k(1);
            }
            if(helpmapBitBuffer[1] == 1 && helpmapBitBuffer[0] == 0)
            {
                CHRbankSwitch8k(2);
            }
            if(helpmapBitBuffer[1] == 1 && helpmapBitBuffer[0] == 1)
            {
                CHRbankSwitch8k(3);
            }
        break;

        default:
            return false;
        break;
    }
}

int NESmemWrite(uint8_t value, uint16_t location)
{
    //printf("location: 0x%X\n",location);
    switch(location)
    {
        case 0x2003:
            oamAddr = value;
        break;

        case 0x2004:
            OAMmem[oamAddr] = value;
            oamAddr++;
        break;

        case 0x2005:
            if(scrollwrite == 0)
            {
                scrollwrite++;
                scrollx = value;
                goto done2005;
            }
            if(scrollwrite == 1)
            {
                scrollwrite = 0;
                scrolly = value;
            }
            done2005:
        break;

        case 0x2006:
            if(NESOB.PPUwritecounter == 0)
            {
                NESOB.VRAMaddresshigh = value;
                NESOB.PPUwritecounter = 1;
                goto done0x2006;
            }
            if(NESOB.PPUwritecounter == 1)
            {
                NESOB.VRAMaddresslow = value;
                NESOB.VRAMaddress2 = NESOB.VRAMaddresshigh << 8 | NESOB.VRAMaddresslow;
                NESOB.PPUwritecounter = 0;
            }
            done0x2006:  // Need to Remove
        break;

        case 0x2007:
            if(NESOB.VRAMaddress2 <= 0x3FFF)
            {
                if(NESOB.VRAMaddress2 >= 0x3F00 && NESOB.VRAMaddress2 < 0x3F20)
                {
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x20] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x40] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x60] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x80] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0xA0] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0xC0] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0xE0] = value;
                }
                if(NESOB.VRAMaddress2 == 0x3F10) // Adding this here as a temp patch for SMB until I rewrite this mirroring code.
                {
                    NESOB.PPUmemory[0x3F00] = value;
                }
                if(NESOB.VRAMaddress2 >= 0x3F20 && NESOB.VRAMaddress2 < 0x3F40)
                {
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x20] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x20] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x40] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x60] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x80] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0xA0] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0xC0] = value;
                }
                if(NESOB.VRAMaddress2 >= 0x3F40 && NESOB.VRAMaddress2 < 0x3F60)
                {
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x40] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x20] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x20] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x40] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x60] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x80] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0xA0] = value;
                }
                if(NESOB.VRAMaddress2 >= 0x3F60 && NESOB.VRAMaddress2 < 0x3F80)
                {
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x60] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x40] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x20] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x20] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x40] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x60] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x80] = value;
                }
                if(NESOB.VRAMaddress2 >= 0x3F80 && NESOB.VRAMaddress2 < 0x3FA0)
                {
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x80] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x60] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x40] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x20] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x20] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x40] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x60] = value;
                }
                if(NESOB.VRAMaddress2 >= 0x3FA0 && NESOB.VRAMaddress2 < 0x3FC0)
                {
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0xA0] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x80] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x60] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x40] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x20] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x20] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x40] = value;
                }
                if(NESOB.VRAMaddress2 >= 0x3FC0 && NESOB.VRAMaddress2 < 0x3FE0)
                {
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0xC0] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0xA0] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x80] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x60] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x40] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x20] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x20] = value;
                }
                if(NESOB.VRAMaddress2 >= 0x3FE0 && NESOB.VRAMaddress2 <= 0x3FFF)
                {
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0xE0] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0xC0] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0xA0] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x80] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x60] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x40] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x20] = value;
                }
                if(value == 0x55 && location == 0x23C9)
                {
                    //breakpoint = true;
                }
                NESOB.PPUmemory[NESOB.VRAMaddress2] = value;
                NESOB.Xbitbuffer = NESOB.memory[0x2000];
                if(NESOB.Xbitbuffer[2] == 0)
                {
                    NESOB.VRAMaddress2++;
                }
                if(NESOB.Xbitbuffer[2] == 1)
                {
                    NESOB.VRAMaddress2 += 0x20;
                }
            }
        break;

        case 0x2008 ... 0x3FFF:
            //printf("HMMM\n\n\n\n\n\n\n\n");
            locationMod8 = location % 0x08;
            location8 = 0x20 << 8 | locationMod8;
            location = location8;
            switch(location) // Code formatting here sucks because of copy paste.
            {
                case 0x2005:
            if(scrollwrite == 0)
            {
                scrollwrite++;
                scrollx = value;
                goto done20052;
            }
            if(scrollwrite == 1)
            {
                scrollwrite = 0;
                scrolly = value;
            }
            done20052:
        break;

        case 0x2006:
            if(NESOB.PPUwritecounter == 0)
            {
                NESOB.VRAMaddresshigh = value;
                NESOB.PPUwritecounter = 1;
                goto done0x20062;
            }
            if(NESOB.PPUwritecounter == 1)
            {
                NESOB.VRAMaddresslow = value;
                NESOB.VRAMaddress2 = NESOB.VRAMaddresshigh << 8 | NESOB.VRAMaddresslow;
                NESOB.PPUwritecounter = 0;
            }
            done0x20062:  // Need to Remove
        break;

        case 0x2007:
                NESOB.VRAMaddress2 = NESOB.VRAMaddress2 % 0x4000;
                if(NESOB.VRAMaddress2 >= 0x3F00 && NESOB.VRAMaddress2 < 0x3F20)
                {
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x20] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x40] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x60] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x80] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0xA0] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0xC0] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0xE0] = value;
                }
                if(NESOB.VRAMaddress2 >= 0x3F20 && NESOB.VRAMaddress2 < 0x3F40)
                {
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x20] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x20] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x40] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x60] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x80] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0xA0] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0xC0] = value;
                }
                if(NESOB.VRAMaddress2 >= 0x3F40 && NESOB.VRAMaddress2 < 0x3F60)
                {
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x40] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x20] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x20] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x40] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x60] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x80] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0xA0] = value;
                }
                if(NESOB.VRAMaddress2 >= 0x3F60 && NESOB.VRAMaddress2 < 0x3F80)
                {
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x60] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x40] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x20] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x20] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x40] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x60] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x80] = value;
                }
                if(NESOB.VRAMaddress2 >= 0x3F80 && NESOB.VRAMaddress2 < 0x3FA0)
                {
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x80] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x60] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x40] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x20] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x20] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x40] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x60] = value;
                }
                if(NESOB.VRAMaddress2 >= 0x3FA0 && NESOB.VRAMaddress2 < 0x3FC0)
                {
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0xA0] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x80] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x60] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x40] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x20] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x20] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x40] = value;
                }
                if(NESOB.VRAMaddress2 >= 0x3FC0 && NESOB.VRAMaddress2 < 0x3FE0)
                {
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0xC0] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0xA0] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x80] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x60] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x40] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x20] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x20] = value;
                }
                if(NESOB.VRAMaddress2 >= 0x3FE0 && NESOB.VRAMaddress2 <= 0x3FFF)
                {
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0xE0] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0xC0] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0xA0] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x80] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x60] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x40] = value;
                    NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x20] = value;
                }
                NESOB.PPUmemory[NESOB.VRAMaddress2] = value;
                NESOB.Xbitbuffer = NESOB.memory[0x2000];
                if(NESOB.Xbitbuffer[2] == 0)
                {
                    NESOB.VRAMaddress2++;
                }
                if(NESOB.Xbitbuffer[2] == 1)
                {
                    NESOB.VRAMaddress2 += 0x20;
                }
        break;
        default:

        break;
            }
        break;

        case 0x4014:
            oamDMA = value;
            //printf("oamDMA:0x%X\n",oamDMA);
            oamCounter = 0;
            oamDMAlocate;
            oamDMAlocate = oamDMA << 8 | 0x00;
            oamDMA = 0;
            while(oamCounter != 0x100)
            {
                OAMmem[oamDMA] = NESOB.memory[oamDMAlocate];
                oamDMA++;
                oamDMAlocate++;
                oamCounter++;
            }
        break;

        case 0x4016:
            //printf("Controller Wrote!\n");
            if(value == 1)
            {
                controlerpoll = controlerpoll << 1;
                controlerpoll[0] = 1;
            }
            if(value == 0)
            {
                controlerpoll = controlerpoll << 1;
                controlerpoll[0] = 0;
            }
        break;

        case 0x8000 ... 0xFFFF:
            //printf("Write to ROM!\nValue: 0x%X\n",value);
            handleRomWrite(value,location);
            return false;
        break;

        default:

        break;
    }
    if(location > 0xFFFF)
    {
        printf("SEG ERROR\n");
    }
    if(location <= 0xFFFF)
    {
        NESOB.memory[location] = value;
    }
    return 0;
}
uint8_t controllercount;
std::bitset<8> resultcontrol;
uint16_t getVal;
std::bitset<8> reset2002;
uint8_t NESmemRead(uint16_t location)
{
    switch(location)
    {
        case 0x2002:
            NESOB.VRAMaddress2 = 0x0000;
            NESOB.VRAMaddresslow = 0x00;
            NESOB.VRAMaddresshigh = 0x00;
            NESOB.PPUwritecounter = 0;
            read2002vb = true;
        break;

        case 0x2007:
                prevval = getVal;
                getVal = NESOB.VRAMaddress2 % 0x4000;
                if(getVal <= 0x3EFF)
                {
                //getVal = NESOB.VRAMaddress2 % 0x4000;
                NESOB.Xbitbuffer = NESOB.memory[0x2000];
                if(NESOB.Xbitbuffer[2] == 0)
                {
                    NESOB.VRAMaddress2++;
                }
                if(NESOB.Xbitbuffer[2] == 1)
                {
                    NESOB.VRAMaddress2 += 0x20;
                }
                NESOB.memory[0x2007] = NESOB.PPUmemory[prevval];
                //printf("getVal: 0x%X\n",NESOB.PPUmemory[getVal]);
                return NESOB.PPUmemory[prevval];
                }

                if(getVal > 0x3EFF)
                {
                    tempVALUE = NESOB.VRAMaddress2;
                    tempVALUE = tempVALUE % 0x20;
                    tempVALUE2 = NESOB.VRAMaddress2 >> 8;
                    NESOB.VRAMaddress2 = tempVALUE2 << 8 | tempVALUE;
                    NESOB.Xbitbuffer = NESOB.memory[0x2000];
                    if(NESOB.Xbitbuffer[2] == 0)
                    {
                        NESOB.VRAMaddress2++;
                        if(NESOB.VRAMaddress2 >= 0x4000)
                        {
                            NESOB.VRAMaddress2 = 0;
                        }
                    }
                    if(NESOB.Xbitbuffer[2] == 1)
                    {
                        NESOB.VRAMaddress2 += 0x20;
                        if(NESOB.VRAMaddress2 >= 0x4000)
                        {
                            NESOB.VRAMaddress2 -= 0x4000;
                        }
                    }
                    NESOB.memory[0x2007] = NESOB.PPUmemory[getVal];
                    //printf("getVal: 0x%X\n",NESOB.PPUmemory[getVal]);
                    return NESOB.PPUmemory[getVal];
                }

        break;

        case 0x2008 ... 0x3FFF:
            printf("Location: 0x%X\n", location);
            location8 = location % 8;
            location8 += 0x2000;
            location = location8;
            printf("Location After: 0x%X\n", location);
            NESmemRead(location);
            return true;
        break;

        case 0x4016:
            //printf("Controller Polled!\n");
            controllercount++;
            if(controllercount == 8)
            {
                controllercount = 0;
            }
            if(controllercount == 1)
            {
                resultcontrol[7] = 0;
                resultcontrol[6] = 1;
                resultcontrol[5] = 0;
                resultcontrol[4] = 0;
                resultcontrol[3] = 0;
                resultcontrol[2] = 0;
                resultcontrol[1] = 0;
                resultcontrol[0] = controlBuffer[0];
                return resultcontrol.to_ulong();
            }
            if(controllercount == 2)
            {
                resultcontrol[7] = 0;
                resultcontrol[6] = 1;
                resultcontrol[5] = 0;
                resultcontrol[4] = 0;
                resultcontrol[3] = 0;
                resultcontrol[2] = 0;
                resultcontrol[1] = 0;
                resultcontrol[0] = controlBuffer[1];
                return resultcontrol.to_ulong();
            }
            if(controllercount == 3)
            {
                resultcontrol[7] = 0;
                resultcontrol[6] = 1;
                resultcontrol[5] = 0;
                resultcontrol[4] = 0;
                resultcontrol[3] = 0;
                resultcontrol[2] = 0;
                resultcontrol[1] = 0;
                resultcontrol[0] = controlBuffer[2];
                return resultcontrol.to_ulong();
            }
            if(controllercount == 4)
            {
                resultcontrol[7] = 0;
                resultcontrol[6] = 1;
                resultcontrol[5] = 0;
                resultcontrol[4] = 0;
                resultcontrol[3] = 0;
                resultcontrol[2] = 0;
                resultcontrol[1] = 0;
                resultcontrol[0] = controlBuffer[3];
                return resultcontrol.to_ulong();
            }
            if(controllercount == 5)
            {
                resultcontrol[7] = 0;
                resultcontrol[6] = 1;
                resultcontrol[5] = 0;
                resultcontrol[4] = 0;
                resultcontrol[3] = 0;
                resultcontrol[2] = 0;
                resultcontrol[1] = 0;
                resultcontrol[0] = controlBuffer[4];
                return resultcontrol.to_ulong();
            }
            if(controllercount == 6)
            {
                resultcontrol[7] = 0;
                resultcontrol[6] = 1;
                resultcontrol[5] = 0;
                resultcontrol[4] = 0;
                resultcontrol[3] = 0;
                resultcontrol[2] = 0;
                resultcontrol[1] = 0;
                resultcontrol[0] = controlBuffer[5];
                return resultcontrol.to_ulong();
            }
            if(controllercount == 7)
            {
                resultcontrol[7] = 0;
                resultcontrol[6] = 1;
                resultcontrol[5] = 0;
                resultcontrol[4] = 0;
                resultcontrol[3] = 0;
                resultcontrol[2] = 0;
                resultcontrol[1] = 0;
                resultcontrol[0] = controlBuffer[6];
                return resultcontrol.to_ulong();
            }
            if(controllercount == 0)
            {
                resultcontrol[7] = 0;
                resultcontrol[6] = 1;
                resultcontrol[5] = 0;
                resultcontrol[4] = 0;
                resultcontrol[3] = 0;
                resultcontrol[2] = 0;
                resultcontrol[1] = 0;
                resultcontrol[0] = controlBuffer[7];
                return resultcontrol.to_ulong();
            }
        break;

        default:

        break;
    }
    return NESOB.memory[location];
}
void NESmemInc(uint16_t location)
{
    NESOB.memory[location]++;
}
void NESmemDec(uint16_t location)
{
    NESOB.memory[location]--;
}
std::bitset<8> tempBitBuffer;
std::bitset<8> tempBitBuffer2;
bool bgpattable;
bool dontDouble;
uint8_t nametableuse;
bool sprite1000;
bool spriteSize;
void handleOther()
{
    tempBitBuffer = NESOB.memory[0x2000];
    tempBitBuffer2 = NESOB.memory[0x2002];
    tempBitBuffer2[4] = tempBitBuffer[4];
    tempBitBuffer2[3] = tempBitBuffer[3];
    tempBitBuffer2[2] = tempBitBuffer[2];
    tempBitBuffer2[1] = tempBitBuffer[1];
    tempBitBuffer2[0] = tempBitBuffer[0];
    sprite1000 = tempBitBuffer[3];
    spriteSize = tempBitBuffer[5];
    //dontDouble = false;
    /*
    if(tempBitBuffer2[6] == 0 && dontDouble == false)
    {
        tempBitBuffer2[6] = 1;
        dontDouble = true;
    }
    if(tempBitBuffer2[6] == 1 && dontDouble == false)
    {
        tempBitBuffer2[6] = 0;
        dontDouble = true;
    }
    */
    //dontDouble = false;
    tempBitBuffer2[6] = 0;
    if(NESOB.scanline == OAMmem[0])
    {
        tempBitBuffer2[6] = 1;
    }

    NESOB.memory[0x2002] = tempBitBuffer2.to_ulong();
    tempBitBuffer = NESOB.memory[0x2000];
    if(tempBitBuffer[0] == 0 && tempBitBuffer[1] == 0)
    {
        nametableAddr = 0x2000;
        nametableuse = 0;
    }
    if(tempBitBuffer[0] == 1 && tempBitBuffer[1] == 0)
    {
        nametableAddr = 0x2400;
        nametableuse = 1;
    }
    if(tempBitBuffer[0] == 0 && tempBitBuffer[1] == 1)
    {
        nametableAddr = 0x2800;
        nametableuse = 2;
    }
    if(tempBitBuffer[0] == 1 && tempBitBuffer[1] == 1)
    {
        nametableAddr = 0x2C00;
        nametableuse = 3;
    }
    if(tempBitBuffer[4] == 0)
    {
        bgpattable = false;
    }
    if(tempBitBuffer[4] == 1)
    {
        bgpattable = true;
    }
}
FILE* logfile = fopen ("log/logdump", "w+");
int handleLog()
{
    if(logfile == NULL)
    {
        //printf("fuck\n");
        return 1;
    }
    fprintf(logfile, "%X                                            A:%X X:%X Y:%X P:%X SP:%X \n", NESOB.pc, NESOB.a, NESOB.x, NESOB.y, NESOB.pflag, NESOB.sp);
}
