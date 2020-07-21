#include <iostream>
#include <string>
#include <bitset>
#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#endif // __linux__
#ifdef _WIN32
#include <SDL.h>
#include <SDL_ttf.h>
#endif // _WIN32
#ifdef __linux__
    #include <thread>
#endif // __linux__
#ifdef _WIN32
    #include "mingw.thread.h"
#endif // _WIN32
#include <unordered_map>
#include <time.h>
#include <stdlib.h>
bool rewindActive;
bool enableRewind;
bool aPress;
uint64_t opcodeTest[0x100];
int cycleModulo = 113; // 134 113 90
const char currentVersion[4] = "0.1";
bool breakpoint;
bool dummybool;
bool enableSound;
bool threadedRenderer;
bool isRenderFinish;
std::bitset<8> controlBuffer;
uint64_t currentFrame;
bool soundIRQ;
uint64_t currentPPUFrame;
bool MMC3mirror;
bool newFrame;
bool exitLoop;
bool newOpcode;
uint16_t Loopy;
bool beginOpLoop;
bool newOpcode2;
uint16_t nametableAddr;
bool rendererChoose;
uint8_t dummyval;
bool volumeConstsq2;
std::bitset<8> tempBitBuffer;
std::bitset<8> tempBitBuffer2;
std::bitset<8> tempBitBuffer3;
std::bitset<8> tempBitBuffer4;
char savestatename[50];
uint8_t lengthCounter1;
uint8_t lengthCounter2;
uint8_t lengthCounter3;
uint8_t lengthCounter4;
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
        uint64_t cycles;
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
uint8_t accurateSP0eval;
FILE* pale = NULL;
void breakpointfunc(uint16_t thepc)
{
    if(NESOB.pc == thepc)
    {
        //breakpoint = true;
    }
}
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
    printf("Cycles: %i\n",NESOB.cycles);
    printf("Scanline: %i\n",NESOB.scanline);
    return true;
}
void handleFlags7(uint8_t value, uint8_t prevValue)
{
    NESOB.Pbitbuffer = NESOB.pflag;
    //NESOB.Xbitbuffer = value;
    NESOB.Pbitbuffer[7] = value >> 7;
    //NESOB.Pbitbuffer[7] = NESOB.Xbitbuffer[7];
    NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
}
void handleFlags1(uint8_t value, uint8_t prevValue)
{
    NESOB.Pbitbuffer = NESOB.pflag;
    NESOB.Pbitbuffer[1] = (value == 0);
    NESOB.pflag = NESOB.Pbitbuffer.to_ulong();
}
void handleFlags0(uint8_t prevValue, uint8_t subValue)
{
    NESOB.Pbitbuffer = NESOB.pflag;
    NESOB.Pbitbuffer[0] = (prevValue >= subValue);
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
uint16_t location2;
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
uint8_t dummy8;
bool read2002vb;
bool throttleCPU = false;
bool doneThrottle = false;
bool reach261 = false;
uint8_t tempVALUE;
uint8_t tempVALUE2;
uint32_t switchLocation;
uint16_t helpLocation;
uint16_t prevScanline;
std::bitset<4> mapperBit;
std::bitset<4> mapperBit2;
FILE* rom = NULL;
bool MMC3irqstate;

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
int bankSwitchPRG52(uint8_t value, uint16_t beginlocation, uint16_t sizeval, uint8_t m52b, bool prgsize52)
{
    switchLocation = (value * 0x2000) + (m52b * (0x10000 * (prgsize52 + 1)));
    rom = fopen(NESOB.filename, "rb");
    fseek(rom,0x10 + switchLocation,SEEK_SET);
    fread(NESOB.memory + beginlocation,sizeval,1,rom);
    fclose(rom);
    return true;
}
int fixSecondLastBank52(uint16_t locationval, uint16_t sizeval,uint8_t m52b, bool prgsize52)
{
    switchLocation = (m52b * (0x10000 * (prgsize52 + 1))) - 0x4000;
    rom = fopen(NESOB.filename, "rb");
    fseek(rom,0x10 + switchLocation,SEEK_SET);
    fread(NESOB.memory + locationval,sizeval,1,rom); // Load first bank into 0xC000 to 0xFFFF
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
int CHRbankSwitch16val(uint16_t value, uint16_t locationval, uint16_t sizeval, uint16_t multip)
{
    switchLocation = (value * multip) + NESOB.prgsize;
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
void handleScanlineStuff()
{
        scanlineTimer = NESOB.cycles % cycleModulo; // 134
        if(prevScanlineTimer > scanlineTimer)
        {
            NESOB.scanline++;
            newFrame = true;
            NESOB.Pbitbuffer = NESOB.memory[0x2002];
            if(NESOB.scanline >= 241)
            {
                //printf("test\n");
                NESOB.Pbitbuffer[7] = 1;
                NESOB.memory[0x2002] = NESOB.Pbitbuffer.to_ulong();
            }
            NESOB.Xbitbuffer = NESOB.memory[0x2000];
            if(NESOB.scanline >= 241 && NMIthrottle == false && NESOB.Pbitbuffer[7] == 1 && NESOB.Xbitbuffer[7] == 1)
            {
                NMIthrottle = true;
                NMIrequest = true;
            }
            if(NESOB.scanline == 261)
            {
                read2002vb = false;
                NMIthrottle = false;
                //NMIrequest = false;
                NESOB.Pbitbuffer = NESOB.memory[0x2002];
                NESOB.Pbitbuffer[7] = 0;
                NESOB.memory[0x2002] = NESOB.Pbitbuffer.to_ulong();
                NESOB.scanline = 0x00;
            }
        }
}
bool throttleMode;
uint64_t curFPS;
uint64_t endFPS;
int throttleCPUval = 0;
char windowTitle[80];
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
    sprintf(windowTitle, "nopNES (Beta v%s) FPS: %i",currentVersion,currentFpsonEnd);
    SDL_SetWindowTitle(nopNESwindow, windowTitle);
    beginBenchmark = false;
    doBenchmark();
    return 0;
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
uint8_t irqCounterReloadMMC3;
uint8_t irqCounterMMC3;
bool irqMMC3reloadstore;
bool irqEnable;
bool beginIRQcounter;
void triggerMMC3irq()
{
        tempBitBuffer = NESOB.pflag;
        tempBitBuffer[2] = 1;
        tempBitBuffer[5] = 1;
        tempBitBuffer[4] = 0;
        NESOB.pflag = tempBitBuffer.to_ulong();
        NESOB.tempValue16 = 0x01 << 8 | NESOB.sp;
        NESOB.higherPC = NESOB.pc >> 8;
        NESOB.lowerPC = NESOB.pc;
        NESOB.memory[NESOB.tempValue16] = NESOB.higherPC;
        NESOB.tempValue16--;
        NESOB.memory[NESOB.tempValue16] = NESOB.lowerPC;
        NESOB.tempValue16--;
        NESOB.memory[NESOB.tempValue16] = NESOB.pflag;
        NESOB.tempValue16--;
        NESOB.sp -= 3;
        NESOB.pc = NESOB.memory[0xFFFF] << 8 | NESOB.memory[0xFFFE];
        //printf("%X\n",NESOB.pc);
}
bool storeIRQ;
uint8_t irqCount;
uint8_t itest;
bool irqThrottle;
bool drawT;
int handleMMC3irq()
{
    return 0;
    if(mapper == 4) // MMC3 IRQ timer
    {
        tempBitBuffer = NESOB.pflag;
        //irqCount--;
        if(irqMMC3reloadstore == true)
        {
            irqMMC3reloadstore = false;
            irqCount = irqCounterReloadMMC3;
        }
        if(irqCount - 1 == NESOB.scanline && irqEnable == true)
        {
            triggerMMC3irq();
            drawT = true;
            printf("IRQ!\n");
            printf("Scanline: %i\n",NESOB.scanline);
            printf("JUMP TO: 0x%X%X\n",NESOB.memory[0xFFFF],NESOB.memory[0xFFFE]);
        }
        //if(irqCount == 0)
        //{
        //    irqCount = irqCounterReloadMMC3;
        //}
        //printf("0x%X\n",irqCounterMMC3);
    }
    return 0;
}
void handleMMC3(uint16_t location, uint8_t value)
{
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
                                    tempBitBuffer = MMC3bankselect;
                                    tempBitBuffer[7] = 0;
                                    tempBitBuffer[6] = 0;
                                    MMC3bankselect = tempBitBuffer.to_ulong();
                                    bankSwitchPRG(MMC3bankselect, 0xC000, 0x2000);
                                    fixSecondLastBankMMC3(0x8000, 0x2000);
                                }
                            }
                            if(MMC3bankbit == 7)
                            {
                                //printf("0x%X\n", value);
                                tempBitBuffer = MMC3bankselect;
                                tempBitBuffer[7] = 0;
                                tempBitBuffer[6] = 0;
                                MMC3bankselect = tempBitBuffer.to_ulong();
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
                case 0xC000 ... 0xDFFF:
                    if(location % 2 == 0)
                    {
                        irqCounterReloadMMC3 = value;
                        //printf("%i on SCANLINE %i\n",value,NESOB.scanline);
                    }
                    if(location % 2 == 1)
                    {
                        irqMMC3reloadstore = true;
                    }
                break;
                case 0xE000 ... 0xFFFF:
                    if(location % 2 == 0)
                    {
                        irqEnable = false;
                        irqCounterMMC3 = irqCounterReloadMMC3;
                    }
                    if(location % 2 == 1)
                    {
                        irqEnable = true;
                    }
                break;
            }
}
bool map52reg;
bool map52prgsize;
uint8_t map52prgreg;
void handlemap52(uint16_t location, uint8_t value)
{
    printf("locate: 0x%X\n",location);
    printf("value: 0x%X\n",value);
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
                                    bankSwitchPRG52(MMC3bankselect, 0x8000,0x2000, map52prgreg, map52prgsize);

                                    //bankSwitchPRG(MMC3bankselect, 0x8000, 0x2000);

                                    fixSecondLastBank52(0xC000, 0x2000, map52prgreg, map52prgsize);
                                }
                                if(MMC3prgbankbit == 1)
                                {
                                    //printf("20x%X\n", value);
                                    bankSwitchPRG52(MMC3bankselect, 0xC000, 0x2000, map52prgreg, map52prgsize);
                                    fixSecondLastBank52(0x8000, 0x2000, map52prgreg, map52prgsize);
                                }
                            }
                            if(MMC3bankbit == 7)
                            {
                                //printf("30x%X\n", value);
                                bankSwitchPRG52(MMC3bankselect, 0xA000, 0x2000, map52prgreg, map52prgsize);
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
                case 0xC000 ... 0xDFFF:
                    if(location % 2 == 0)
                    {
                        irqCounterReloadMMC3 = value;
                    }
                    if(location % 2 == 1)
                    {
                        irqMMC3reloadstore = true;
                    }
                break;
                case 0xE000 ... 0xFFFF:
                    if(location % 2 == 0)
                    {
                        irqEnable = false;
                    }
                    if(location % 2 == 1)
                    {
                        irqEnable = true;
                    }
                break;
            }
}
uint8_t mapper90mode;
uint16_t map90chr;
uint8_t map90chrhigh;
uint8_t map90chrlow;
uint8_t helpFlip;
std::bitset<4> bit4;
std::bitset<5> bit5;
uint8_t dummymap;
void cpyVMEM(uint16_t from, uint16_t to, uint16_t datasize)
{
    while(datasize != 0xFFFF)
    {
        NESOB.PPUmemory[to] = NESOB.PPUmemory[from];
        from++;
        to++;
        datasize--;
    }
}
uint8_t chr113;
uint8_t prg113;
uint8_t dummy113;
bool handleRomWrite(uint8_t value, uint16_t location)
{
    switch(mapper)
    {
        case 0x00:
            return false;
        break;

        case 0x01:
            MMC1help1 = value;
            //MMC3mirror = MMC1help1[1];
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
                    //cout<<MMC1help1<<endl;
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
                    if(MMC1help1[1] == 1 && MMC1help1[0] == 1)
                    {

                        MMC3mirror = true;
                        cpyVMEM(0x2400, 0x2800, 0x400);
                        cpyVMEM(0x2000, 0x2400, 0x400);
                    }
                    else
                    {
                        MMC3mirror = false;
                    }
                }
                if(location >= 0xA000 && location <= 0xBFFF)
                {
                    //printf("shift: 0x%X\n",shiftReg);
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
                    //printf("shift: 0x%X\n",shiftReg);
                    if(CHRbankmode == true)
                    {
                        CHRbankSwitch4khigh(shiftReg);
                    }
                    if(CHRbankmode == false)
                    {
                        //printf("shift: 0x%X\n",shiftReg);
                        CHRbankSwitch4khigh(shiftReg);
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
            handleMMC3(location, value);
        break;

        case 0x07:
            tempBitBuffer = value;
            tempBitBuffer[7] = 0;
            tempBitBuffer[6] = 0;
            tempBitBuffer[5] = 0;
            tempBitBuffer[4] = 0;
            tempBitBuffer[3] = 0;
            bankSwitch32k(tempBitBuffer.to_ulong());
        break;

        case 0x09:
            switch(location)
            {
                case 0xA000 ... 0xAFFF:
                    bit4 = value;
                    dummymap = bit4.to_ulong();
                    bankSwitchPRG(dummymap,0x8000,0x2000);
                break;

                case 0xB000 ... 0xCFFF:
                    bit5 = value;
                    dummymap = bit5.to_ulong();
                    CHRbankSwitch2(dummymap, 0x0000,0x1000,0x1000);
                break;

                case 0xD000 ... 0xEFFF:
                    bit5 = value;
                    dummymap = bit5.to_ulong();
                    CHRbankSwitch2(dummymap, 0x1000,0x1000,0x1000);
                break;

                case 0xF000 ... 0xFFFF:
                    bit5 = value;
                    bit5.flip(0);
                    MMC3mirror = bit5[0];
                break;
            }
        break;

        case 0x34:
            //printf("test\n");
            switch(location)
            {
                case 0x6000 ... 0x7FFF:
                    tempBitBuffer = value;
                    map52reg = tempBitBuffer[7];
                    tempBitBuffer[7] = 0;
                    tempBitBuffer[6] = 0;
                    tempBitBuffer[5] = 0;
                    tempBitBuffer[4] = 0;
                    tempBitBuffer[3] = 0;
                    map52prgreg = tempBitBuffer.to_ulong();
                    map52prgreg = map52prgreg << 4;
                    tempBitBuffer = value;
                    map52prgsize = tempBitBuffer[3];
                    printf("blBank: 0x%X\n",map52prgreg);
                    cout<<"PRGSIZE: 0x"<<tempBitBuffer[3]<<endl;
                    //cout<<tempBitBuffer<<endl;
                    //printf("testing2\n\naaaa\n");
                break;

                case 0x8000 ... 0xFFFF:
                    handlemap52(location, value);
                    //printf("test\n");
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

        case 0x5A:

            switch(location)
            {
                case 0x8000 ... 0x8003:
                    tempBitBuffer = mapper90mode;
                    //printf("0x%X\n",value);
                    if(tempBitBuffer[1] == 0 && tempBitBuffer[0] == 0)
                    {
                        if(tempBitBuffer[2] == 1)
                        {
                            bankSwitch32k(value);
                        }

                    }
                    if(tempBitBuffer[1] == 0 && tempBitBuffer[0] == 1)
                    {
                        if(location == 0x8000 || location == 0x8001)
                        {
                            bankSwitch16k(value);
                        }
                        if(location == 0x8002 || location == 0x8003)
                        {
                            if(tempBitBuffer[2] == 1)
                            {
                                bankSwitch16k2(value);
                            }

                        }
                    }
                    if(tempBitBuffer[1] == 1 && tempBitBuffer[0] == 0)
                    {
                        if(location == 0x8000)
                        {
                            bankSwitchPRG(value, 0x8000,0x2000);
                        }
                        if(location == 0x8001)
                        {
                            bankSwitchPRG(value, 0xA000,0x2000);
                        }
                        if(location == 0x8002)
                        {
                            bankSwitchPRG(value, 0xC000,0x2000);
                        }
                        if(location == 0x8003)
                        {
                            if(tempBitBuffer[2] == 1)
                            {
                                bankSwitchPRG(value, 0xE000,0x2000);
                            }
                        }
                    }
                    if(tempBitBuffer[1] == 1 && tempBitBuffer[0] == 1)
                    {
                        tempBitBuffer2 = value;
                        tempBitBuffer2.flip(0);
                        tempBitBuffer2.flip(1);
                        tempBitBuffer2.flip(2);
                        tempBitBuffer2.flip(3);
                        tempBitBuffer2.flip(4);
                        tempBitBuffer2.flip(5);
                        tempBitBuffer2.flip(6);
                        value = tempBitBuffer2.to_ulong();
                        if(location == 0x8000)
                        {
                            bankSwitchPRG(value, 0x8000,0x2000);
                        }
                        if(location == 0x8001)
                        {
                            bankSwitchPRG(value, 0xA000,0x2000);
                        }
                        if(location == 0x8002)
                        {
                            bankSwitchPRG(value, 0xC000,0x2000);
                        }
                        if(location == 0x8003)
                        {
                            if(tempBitBuffer[2] == 1)
                            {
                                bankSwitchPRG(value, 0xE000,0x2000);
                            }
                        }
                    }
                break;
                case 0x9000 ... 0xAFFF:
                    tempBitBuffer = mapper90mode;
                    if(tempBitBuffer[3] == 0 && tempBitBuffer[4] == 0)
                    {
                        if(location >= 0x9000 && location <= 0x9007)
                        {
                            map90chrlow = value;
                            map90chr = map90chrhigh << 8 | map90chrlow;
                            CHRbankSwitch16val(map90chr,0x0000,0x2000,0x400);
                        }
                        if(location >= 0xA000 && location <= 0xA007)
                        {
                            map90chrhigh = value;
                            map90chr = map90chrhigh << 8 | map90chrlow;
                            CHRbankSwitch16val(map90chr,0x0000,0x2000,0x400);
                        }
                    }
                    if(tempBitBuffer[3] == 1 && tempBitBuffer[4] == 0)
                    {
                        if(location >= 0x9000 && location <= 0x9003)
                        {
                            map90chrlow = value;
                            map90chr = map90chrhigh << 8 | map90chrlow;
                            CHRbankSwitch16val(map90chr,0x0000,0x1000,0x400);
                        }
                        if(location >= 0x9004 && location <= 0x9007)
                        {
                            map90chrlow = value;
                            map90chr = map90chrhigh << 8 | map90chrlow;
                            CHRbankSwitch16val(map90chr,0x1000,0x1000,0x400);
                        }
                        if(location >= 0xA000 && location <= 0xA003)
                        {
                            map90chrhigh = value;
                            map90chr = map90chrhigh << 8 | map90chrlow;
                            CHRbankSwitch16val(map90chr,0x0000,0x1000,0x400);
                        }
                        if(location >= 0xA004 && location <= 0xA007)
                        {
                            map90chrhigh = value;
                            map90chr = map90chrhigh << 8 | map90chrlow;
                            CHRbankSwitch16val(map90chr,0x1000,0x1000,0x400);
                        }
                    }
                    if(tempBitBuffer[3] == 0 && tempBitBuffer[4] == 1)
                    {
                        if(location >= 0x9000 && location <= 0x9001)
                        {
                            map90chrlow = value;
                            map90chr = map90chrhigh << 8 | map90chrlow;
                            CHRbankSwitch16val(map90chr,0x0000,0x0800,0x400);
                        }
                        if(location >= 0x9002 && location <= 0x9003)
                        {
                            map90chrlow = value;
                            map90chr = map90chrhigh << 8 | map90chrlow;
                            CHRbankSwitch16val(map90chr,0x0800,0x0800,0x400);
                        }
                        if(location >= 0x9004 && location <= 0x9005)
                        {
                            map90chrlow = value;
                            map90chr = map90chrhigh << 8 | map90chrlow;
                            CHRbankSwitch16val(map90chr,0x1000,0x0800,0x400);
                        }
                        if(location >= 0x9006 && location <= 0x9007)
                        {
                            map90chrlow = value;
                            map90chr = map90chrhigh << 8 | map90chrlow;
                            CHRbankSwitch16val(map90chr,0x1800,0x0800,0x400);
                        }
                        if(location >= 0xA000 && location <= 0xA001)
                        {
                            map90chrhigh = value;
                            map90chr = map90chrhigh << 8 | map90chrlow;
                            CHRbankSwitch16val(map90chr,0x0000,0x0800,0x400);
                        }
                        if(location >= 0xA002 && location <= 0xA003)
                        {
                            map90chrhigh = value;
                            map90chr = map90chrhigh << 8 | map90chrlow;
                            CHRbankSwitch16val(map90chr,0x0800,0x0800,0x400);
                        }
                        if(location >= 0xA004 && location <= 0xA005)
                        {
                            map90chrhigh = value;
                            map90chr = map90chrhigh << 8 | map90chrlow;
                            CHRbankSwitch16val(map90chr,0x1000,0x0800,0x400);
                        }
                        if(location >= 0xA006 && location <= 0xA007)
                        {
                            map90chrhigh = value;
                            map90chr = map90chrhigh << 8 | map90chrlow;
                            CHRbankSwitch16val(map90chr,0x1800,0x0800,0x400);
                        }
                    }
                    if(tempBitBuffer[3] == 1 && tempBitBuffer[4] == 1)
                    {
                        switch(location)
                        {
                            case 0x9000:
                                map90chrlow = value;
                                map90chr = map90chrhigh << 8 | map90chrlow;
                                CHRbankSwitch16val(map90chr,0x0000,0x0400,0x400);
                            break;
                            case 0x9001:
                                map90chrlow = value;
                                map90chr = map90chrhigh << 8 | map90chrlow;
                                CHRbankSwitch16val(map90chr,0x0400,0x0400,0x400);
                            break;
                            case 0x9002:
                                map90chrlow = value;
                                map90chr = map90chrhigh << 8 | map90chrlow;
                                CHRbankSwitch16val(map90chr,0x0800,0x0400,0x400);
                            break;
                            case 0x9003:
                                map90chrlow = value;
                                map90chr = map90chrhigh << 8 | map90chrlow;
                                CHRbankSwitch16val(map90chr,0x0C00,0x0400,0x400);
                            break;
                            case 0x9004:
                                map90chrlow = value;
                                map90chr = map90chrhigh << 8 | map90chrlow;
                                CHRbankSwitch16val(map90chr,0x1000,0x0400,0x400);
                            break;
                            case 0x9005:
                                map90chrlow = value;
                                map90chr = map90chrhigh << 8 | map90chrlow;
                                CHRbankSwitch16val(map90chr,0x1400,0x0400,0x400);
                            break;
                            case 0x9006:
                                map90chrlow = value;
                                map90chr = map90chrhigh << 8 | map90chrlow;
                                CHRbankSwitch16val(map90chr,0x1800,0x0400,0x400);
                            break;
                            case 0x9007:
                                map90chrlow = value;
                                map90chr = map90chrhigh << 8 | map90chrlow;
                                CHRbankSwitch16val(map90chr,0x1C00,0x0400,0x400);
                            break;
                            case 0xA000:
                                map90chrhigh = value;
                                map90chr = map90chrhigh << 8 | map90chrlow;
                                CHRbankSwitch16val(map90chr,0x0000,0x0400,0x400);
                            break;
                            case 0xA001:
                                map90chrhigh = value;
                                map90chr = map90chrhigh << 8 | map90chrlow;
                                CHRbankSwitch16val(map90chr,0x0400,0x0400,0x400);
                            break;
                            case 0xA002:
                                map90chrhigh = value;
                                map90chr = map90chrhigh << 8 | map90chrlow;
                                CHRbankSwitch16val(map90chr,0x0800,0x0400,0x400);
                            break;
                            case 0xA003:
                                map90chrhigh = value;
                                map90chr = map90chrhigh << 8 | map90chrlow;
                                CHRbankSwitch16val(map90chr,0x0C00,0x0400,0x400);
                            break;
                            case 0xA004:
                                map90chrhigh = value;
                                map90chr = map90chrhigh << 8 | map90chrlow;
                                CHRbankSwitch16val(map90chr,0x1000,0x0400,0x400);
                            break;
                            case 0xA005:
                                map90chrhigh = value;
                                map90chr = map90chrhigh << 8 | map90chrlow;
                                CHRbankSwitch16val(map90chr,0x1400,0x0400,0x400);
                            break;
                            case 0xA006:
                                map90chrhigh = value;
                                map90chr = map90chrhigh << 8 | map90chrlow;
                                CHRbankSwitch16val(map90chr,0x1800,0x0400,0x400);
                            break;
                            case 0xA007:
                                map90chrhigh = value;
                                map90chr = map90chrhigh << 8 | map90chrlow;
                                CHRbankSwitch16val(map90chr,0x1C00,0x0400,0x400);
                            break;
                        }
                    }
                break;
                case 0xD000:
                    mapper90mode = value;
                break;
                case 0xD001:
                    tempBitBuffer = value;
                    //cout<<tempBitBuffer<<endl;
                    MMC3mirror = tempBitBuffer[0];
                break;
            }
        break;

        case 113:
            printf("Value 0x%X\n",value);
            dummy113 = location >> 8;
            printf("dummy: 0x%X\n",dummy113);
            if(dummy113 % 2 == 1)
            {
                tempBitBuffer = value;
                tempBitBuffer[7] = 0;
                tempBitBuffer[6] = 0;
                tempBitBuffer[2] = 0;
                tempBitBuffer[1] = 0;
                tempBitBuffer[0] = 0;
                tempBitBuffer = tempBitBuffer >> 3;
                prg113 = tempBitBuffer.to_ulong();
                bankSwitch32k(prg113);
                tempBitBuffer = value;
                tempBitBuffer.flip(7);
                MMC3mirror = tempBitBuffer[7];
                tempBitBuffer[3] = 0;
                tempBitBuffer[4] = 0;
                tempBitBuffer[5] = 0;
                tempBitBuffer[7] = 0;
                tempBitBuffer[3] = tempBitBuffer[6];
                tempBitBuffer[6] = 0;
                chr113 = tempBitBuffer.to_ulong();
                CHRbankSwitch8k(chr113);
            }
        break;

        default:
            return false;
        break;
    }
    return 0;
}
bool three000;
bool volumeConstsq1;
uint8_t coarseYscroll;
uint8_t fineXscroll;
uint8_t coarseXscroll;
uint8_t controllercount;
bool maskProperties[8];
int NESmemWrite(uint8_t value, uint16_t location)
{
    //printf("location: 0x%X\n",location);
    switch(location)
    {

        case 0x0800 ... 0x1FFF:
            location = location % 0x7FF;
        break;

        case 0x2000:
            tempBitBuffer = value;
            tempBitBuffer16 = Loopy;
            tempBitBuffer16[11] = tempBitBuffer[1];
            tempBitBuffer16[10] = tempBitBuffer[0];
            Loopy = tempBitBuffer16.to_ulong();
        break;

        case 0x2001:
            tempBitBuffer = value;
            maskProperties[0] = tempBitBuffer[0];
            maskProperties[1] = tempBitBuffer[1];
            maskProperties[2] = tempBitBuffer[2];
            maskProperties[3] = tempBitBuffer[3];
            maskProperties[4] = tempBitBuffer[4];
            maskProperties[5] = tempBitBuffer[5];
            maskProperties[6] = tempBitBuffer[6];
            maskProperties[7] = tempBitBuffer[7];
        break;

        case 0x2002:
            return 0;
        break;

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
                tempBitBuffer = scrollx / 8;
                //printf("scrollx %x\n",scrollx / 8);
                tempBitBuffer16 = Loopy;
                tempBitBuffer16[4] = tempBitBuffer[4];
                tempBitBuffer16[3] = tempBitBuffer[3];
                tempBitBuffer16[2] = tempBitBuffer[2];
                tempBitBuffer16[1] = tempBitBuffer[1];
                tempBitBuffer16[0] = tempBitBuffer[0];
                Loopy = tempBitBuffer16.to_ulong();
                fineXscroll = scrollx % 8;
                goto done2005;
            }
            if(scrollwrite == 1)
            {
                scrollwrite = 0;
                if(NESOB.scanline >= 240)
                {
                    scrolly = value;
                    tempBitBuffer = scrolly / 8;
                    tempBitBuffer16 = Loopy;
                    tempBitBuffer16[9] = tempBitBuffer[4];
                    tempBitBuffer16[8] = tempBitBuffer[3];
                    tempBitBuffer16[7] = tempBitBuffer[2];
                    tempBitBuffer16[6] = tempBitBuffer[1];
                    tempBitBuffer16[5] = tempBitBuffer[0];
                    tempBitBuffer = scrolly % 8;
                    tempBitBuffer16[14] = tempBitBuffer[2];
                    tempBitBuffer16[13] = tempBitBuffer[1];
                    tempBitBuffer16[12] = tempBitBuffer[0];
                    Loopy = tempBitBuffer16.to_ulong();
                }
                //printf("SCY: 0x%X\n",scrolly);
                //printf("Scanline: %i\n",NESOB.scanline);
            }
            done2005:
        break;

        case 0x2006:
            if(NESOB.PPUwritecounter == 0)
            {
                NESOB.VRAMaddresshigh = value;
                NESOB.PPUwritecounter = 1;
                //handleMMC3irq();
                goto done0x2006;
            }
            if(NESOB.PPUwritecounter == 1)
            {
                NESOB.VRAMaddresslow = value;
                NESOB.VRAMaddress2 = NESOB.VRAMaddresshigh << 8 | NESOB.VRAMaddresslow;
                Loopy = NESOB.VRAMaddress2;
                NESOB.PPUwritecounter = 0;
                //printf("NESPPUWRITE: 0x%X\n",NESOB.VRAMaddress2);
                //handleMMC3irq();
            }
            done0x2006:  // Need to Remove
            if(NESOB.VRAMaddress2 >= 0x4000)
            {
                NESOB.VRAMaddress2 = NESOB.VRAMaddress2 % 0x4000;
            }
        break;

        case 0x2007:
            three000 = false;
            if(NESOB.VRAMaddress2 >= 0x3000  && NESOB.VRAMaddress2 <= 0x3EFF)
            {
                three000 = true;
                NESOB.VRAMaddress2 -= 0x1000;
            }
            if(NESOB.VRAMaddress2 >= 0x2000 && NESOB.VRAMaddress2 <= 0x27FF && MMC3mirror == false)
            {
                NESOB.PPUmemory[NESOB.VRAMaddress2] = value;
                NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x800] = value;
            }
            if(NESOB.VRAMaddress2 >= 0x2800 && NESOB.VRAMaddress2 <= 0x2FFF && MMC3mirror == false)
            {
                NESOB.PPUmemory[NESOB.VRAMaddress2] = value;
                NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x800] = value;
            }
            if(NESOB.VRAMaddress2 >= 0x2000 && NESOB.VRAMaddress2 <= 0x23FF && MMC3mirror == true)
            {
                NESOB.PPUmemory[NESOB.VRAMaddress2] = value;
                NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x400] = value;
            }
            if(NESOB.VRAMaddress2 >= 0x2400 && NESOB.VRAMaddress2 <= 0x27FF && MMC3mirror == true)
            {
                NESOB.PPUmemory[NESOB.VRAMaddress2] = value;
                NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x400] = value;
            }
            if(NESOB.VRAMaddress2 >= 0x2800 && NESOB.VRAMaddress2 <= 0x2BFF && MMC3mirror == true)
            {
                NESOB.PPUmemory[NESOB.VRAMaddress2] = value;
                NESOB.PPUmemory[NESOB.VRAMaddress2 + 0x400] = value;
            }
            if(NESOB.VRAMaddress2 >= 0x2C00 && NESOB.VRAMaddress2 <= 0x2FFF && MMC3mirror == true)
            {
                NESOB.PPUmemory[NESOB.VRAMaddress2] = value;
                NESOB.PPUmemory[NESOB.VRAMaddress2 - 0x400] = value;
            }
            if(NESOB.VRAMaddress2 <= 0x2FFF || NESOB.VRAMaddress2 >= 0x3F00)
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
                    NESOB.PPUmemory[0x3F10] = value;
                }
                if(NESOB.VRAMaddress2 == 0x3F00)
                {
                    NESOB.PPUmemory[0x3F00] = value;
                    NESOB.PPUmemory[0x3F10] = value;
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
                if(three000 == true)
                {
                    NESOB.VRAMaddress2 += 0x1000;
                }
                if(NESOB.VRAMaddress2 > 0x3FFF)
                {
                    NESOB.VRAMaddress2 -= 0x4000;
                }
            }
        break;

        case 0x2008 ... 0x3FFF:
            location2 = location % 8;
            location2 += 0x2000;
            NESmemWrite(value,location2);
            return 0;
        break;

        case 0x4000:
            tempBitBuffer = value;
            volumeConstsq1 = tempBitBuffer[4];
        break;

        case 0x4003:
            tempBitBuffer = value;
            tempBitBuffer = tempBitBuffer >> 3;
            lengthCounter1 = tempBitBuffer.to_ulong();
        break;

        case 0x4004:
            tempBitBuffer = value;
            volumeConstsq2 = tempBitBuffer[4];
        break;

        case 0x4007:
            tempBitBuffer = value;
            tempBitBuffer = tempBitBuffer >> 3;
            lengthCounter2 = tempBitBuffer.to_ulong();
        break;

        case 0x400B:
            tempBitBuffer = value;
            tempBitBuffer = tempBitBuffer >> 3;
            lengthCounter3 = tempBitBuffer.to_ulong();
        break;

        case 0x400F:
            tempBitBuffer = value;
            tempBitBuffer = tempBitBuffer >> 3;
            lengthCounter4 = tempBitBuffer.to_ulong();
        break;

        case 0x4014:
            oamDMA = value;
            //printf("oamDMA:0x%X\n",oamDMA);
            oamCounter = 0;
            oamDMAlocate = oamDMA << 8 | 0x00;
            oamDMA = oamAddr;
            while(oamCounter != 0x100)
            {
                OAMmem[oamDMA] = NESOB.memory[oamDMAlocate];
                oamDMA++;
                oamDMAlocate++;
                oamCounter++;
            }
        break;

        case 0x4016:
            //printf("Controller Wrote VAL 0x%0!\n",value);
            controlerpoll = value;
            controllercount = 0x0;
        break;

        case 0x4100 ... 0x7FFF:
            NESOB.memory[location] = value;
            handleRomWrite(value, location);
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
        return 0xFF;
    }
    if(location <= 0xFFFF)
    {
        NESOB.memory[location] = value;
    }
    return 0;
}
std::bitset<8> resultcontrol;
uint16_t getVal;
std::bitset<8> reset2002;
uint8_t NESmemRead(uint16_t location)
{
    switch(location)
    {
        case 0x0800 ... 0x1FFF:
        location = location % 0x07FF;
        break;

        case 0x2002:
            NESOB.VRAMaddress2 = 0x0000;
            NESOB.VRAMaddresslow = 0x00;
            NESOB.VRAMaddresshigh = 0x00;
            NESOB.PPUwritecounter = 0;
            read2002vb = true;
        break;

        case 0x2004:
            return OAMmem[oamAddr];
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
                    if(NESOB.VRAMaddress2 == 0x3FFF)
                    {
                        printf("hey\n");
                    }
                    if(NESOB.Xbitbuffer[2] == 0)
                    {
                        NESOB.VRAMaddress2++;
                    }
                    if(NESOB.Xbitbuffer[2] == 1)
                    {
                        NESOB.VRAMaddress2 += 0x20;
                    }
                    if(NESOB.VRAMaddress2 >= 0x4000)
                    {
                        NESOB.VRAMaddress2 -= 0x4000;
                    }
                    NESOB.memory[0x2007] = NESOB.PPUmemory[getVal];
                    //printf("getVal: 0x%X\n",NESOB.PPUmemory[getVal]);
                    return NESOB.PPUmemory[getVal];
                }

        break;

        case 0x2008 ... 0x3FFF:
            //breakpoint = true;
            printf("Location: 0x%X\n", location);
            location2 = location % 8;
            location2 += 0x2000;
            printf("Location After: 0x%X\n", location2);
            return NESOB.memory[location2];
        break;

        case 0x4016:
            //printf("Controller Polled! PC: 0x%X\n",NESOB.pc);
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
    if(read2002vb == true)
    {
        read2002vb = false;
        //tempBitBuffer2 = NESOB.memory[0x2002];
        //tempBitBuffer = NESOB.memory[0x2002];
        //tempBitBuffer[7] = 0;
        //NESOB.memory[0x2002] = tempBitBuffer.to_ulong();
        //return tempBitBuffer2.to_ulong();
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
bool bgpattable;
bool dontDouble;
uint8_t nametableuse;
bool sprite1000;
bool spriteSize;
uint16_t nametableLookup[4] = {0x2000, 0x2400, 0x2800, 0x2C00};
uint8_t nametableUseLook[4] = {0,1,2,3};
/*
case 0xC000 ... 0xDFFF:
                    if(location % 2 == 0)
                    {
                        irqCounterReloadMMC3 = value;
                    }
                    if(location % 2 == 1)
                    {
                        irqMMC3reloadstore = true;
                    }
                break;
                case 0xE000 ... 0xFFFF:
                    if(location % 2 == 0)
                    {
                        irqEnable = false;
                    }
                    if(location % 2 == 1)
                    {
                        irqEnable = true;
                    }
                break;
                */
uint8_t sp0scan;
bool sprite0;
void handleOther()
{
    handleMMC3irq();
    //doneIRQ:
    tempBitBuffer = NESOB.memory[0x2000];
    tempBitBuffer2 = NESOB.memory[0x2002];
    tempBitBuffer2[4] = tempBitBuffer[4];
    tempBitBuffer2[3] = tempBitBuffer[3];
    tempBitBuffer2[2] = tempBitBuffer[2];
    tempBitBuffer2[1] = tempBitBuffer[1];
    tempBitBuffer2[0] = tempBitBuffer[0];
    sprite1000 = tempBitBuffer[3];
    spriteSize = tempBitBuffer[5];
    //OAMmem[0] + 5
    if(NESOB.scanline == sp0scan)
    {
        //printf("Scan %i\n",NESOB.scanline);
        //printf("2 %i\n",OAMmem[0]);
        tempBitBuffer2[6] = 1;
    }
    if(NESOB.scanline == 260)
    {
        tempBitBuffer2[6] = 0;
    }
    NESOB.memory[0x2002] = tempBitBuffer2.to_ulong();
    tempBitBuffer = NESOB.memory[0x2000];
    nametableAddr = nametableLookup[(tempBitBuffer[1] * 2) + tempBitBuffer[0]];
    nametableuse = tempBitBuffer[1] << 1 | tempBitBuffer[0];
    bgpattable = tempBitBuffer[4];
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
    return 0;
}
bool corruptorEnable;
uint16_t numberCor;
uint16_t perFrame;
int handleCorruptorKey()
{
    if(corruptorEnable == false)
    {
        cout<<"Would you like to enable the corruptor?"<<endl<<"WARNING, THIS MAY CAUSE SEIZURES!"<<endl;
        cin>>options;
        if(options == 'y')
        {
            corruptorEnable = true;
        }
        if(options == 'n')
        {
            return 0;
        }
    }
    if(corruptorEnable == true)
    {
        cout<<"How many bytes would you like to corrupt? Up to 65535"<<endl;
        cin>>numberCor;
        cout<<"Corrupt "<<numberCor<<" bytes per how many frames?"<<endl;
        cin>>perFrame;
        cout<<"Beginning Corruption."<<endl;
    }
    return 0;
}
uint16_t bytecount;
uint16_t corAddr;
void handleCorruptor()
{
    bytecount = numberCor;
    while(bytecount != 0)
    {
        if(rand() % 5 == 0)
        {
            corAddr = rand() % 0x8000;
            if(corAddr >= 0x2000 && corAddr <= 0x3FFF)
            {
                corAddr -= 0x2000;
            }
            NESmemWrite(rand() % 0x100,corAddr);
            goto donecor1;
        }
        NESOB.PPUmemory[rand() % 0x4000] = rand() % 0x100;
        donecor1:
        bytecount--;
    }
}
void ifBreakpoint()
{
    if(breakpoint == true)
        {
            printRegs();
            printf("Continue?\n");
            cin>>options;
            if(options == 's')
            {
                //saveNESstate();
            }
            if(options == 'l')
            {
                //loadNESstate();
            }
            if(options == 'm')
            {
                memDump();
            }
            if(options == 'o')
            {
                breakpoint = false;
            }
            if(options == 'b')
            {
                breakpoint = true;
            }
            if(options == 'r')
            {
                NESOB.closeProgram = true;
            }
            if(options == 'i')
            {
                NESOB.pc++;
            }
        }
}
