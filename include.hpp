#include <iostream>
#include <string>
#include <bitset>
#include <SDL2/SDL.h>
#include <thread>
#include <unordered_map>
int cycleModulo = 113; // 134 113 90
std::bitset<8> controlBuffer;
uint64_t currentFrame;
uint64_t currentPPUFrame;
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
SDL_Renderer* renderer;
SDL_Event SDL_EVENT_HANDLING;
int option2;
using namespace std;
bool beginning()
{

    cout<<"Welcome to nopNES!"<<endl<<"Rom Name: ";
    cin>>NESOB.filename;
    FILE* headerf = fopen(NESOB.filename, "rb");
    if(headerf == NULL)
    {
        return false;
    }
    fread(NESOB.header,0x10,1,headerf);
    fclose(headerf);
    NESOB.prgsize = NESOB.header[0x04] * 16384;
    printf("PRG ROM is 0x%X Bytes!\n",NESOB.prgsize);
    NESOB.chrsize = NESOB.header[0x05] * 8192;
    printf("CHR ROM is 0x%X Bytes!\n",NESOB.chrsize);
    FILE* rom = fopen(NESOB.filename, "rb");
    fseek(rom,0x10,SEEK_SET);
    fread(NESOB.memory + 0x8000,NESOB.prgsize,1,rom);
    if(NESOB.prgsize != 0x8000)
    {
        rewind(rom);
        fseek(rom,0x10,SEEK_SET);
        fread(NESOB.memory + 0xC000,NESOB.prgsize,1,rom);
    }
    fseek(rom,NESOB.prgsize + 0x10,SEEK_SET);
    fread(NESOB.PPUmemory, 0x2000, 1, rom);
    fclose(rom);
    FILE* mem_dump = fopen ("log/memdump", "w+");
    fwrite (NESOB.memory , sizeof(char), sizeof(NESOB.memory), mem_dump);
    fclose (mem_dump);
    cin>>option2;
    return true;
}

int printRegs()
{
    printf("Opcode: 0x%X\n",NESOB.opcode);
    printf("A: 0x%X\n",NESOB.a);
    printf("X: 0x%X\n",NESOB.x);
    printf("Y: 0x%X\n",NESOB.y);
    printf("SP: 0x%X\n",NESOB.sp);
    NESOB.Pbitbuffer = NESOB.pflag;
    std::cout<<"pflag: "<<NESOB.Pbitbuffer<<std::endl;
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
int handleScanlineStuff()
{
        if(NESOB.scanline == 241 && renderThrottle == false)
        {
            renderThrottle = true;
            newFrame = true;
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
    printf("Acomplished %i CPU FPS in the time it should take to do 60 CPU FPS!\n", currentFpsonEnd);
    printf("Acomplished %i PPU FPS in the time it should take to do 60 PPU FPS!\n", endFPS);
    beginBenchmark = false;
    return 0;
}
void fpsBenchmark()
{
    if(beginBenchmark == false)
    {
        std::thread benchmark(doBenchmark);
        benchmark.detach();
    }
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
uint8_t oamAddr;
uint8_t oamDMA;
uint16_t oamCounter;
uint16_t oamDMAlocate;
uint8_t bitcountS;
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
            /*
            if(NESOB.PPUwritecounter == 0)
            {
                NESOB.PPUwritecounter = 1;
            }
            if(NESOB.PPUwritecounter == 1)
            {
                NESOB.PPUwritecounter = 0;
            }
            */
        break;

        case 0x2008 ... 0x3FFF:
            //printf("Location: 0x%X\n", location);
            locationMod8 = location % 0x08;
            location8 = 0x20 << 8 | locationMod8;
            location = location8;
           // printf("Location After: 0x%X\n", location);
            switch(location)
            {
                case 0x2002:
                NESOB.VRAMaddress2 = 0x0000;
                NESOB.VRAMaddresslow = 0x00;
                NESOB.VRAMaddresshigh = 0x00;
                NESOB.PPUwritecounter = 0;
                read2002vb = true;
                break;

                default:

                break;
            }
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
void handleOther()
{
    tempBitBuffer = NESOB.memory[0x2000];
    tempBitBuffer2 = NESOB.memory[0x2002];
    tempBitBuffer2[4] = tempBitBuffer[4];
    tempBitBuffer2[3] = tempBitBuffer[3];
    tempBitBuffer2[2] = tempBitBuffer[2];
    tempBitBuffer2[1] = tempBitBuffer[1];
    tempBitBuffer2[0] = tempBitBuffer[0];
    dontDouble = false;
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
    dontDouble = false;
    NESOB.memory[0x2002] = tempBitBuffer2.to_ulong();
    tempBitBuffer = NESOB.memory[0x2000];
    if(tempBitBuffer[0] == 0 && tempBitBuffer[1] == 0)
    {
        nametableAddr = 0x2000;
    }
    if(tempBitBuffer[0] == 1 && tempBitBuffer[1] == 0)
    {
        nametableAddr = 0x2400;
    }
    if(tempBitBuffer[0] == 0 && tempBitBuffer[1] == 1)
    {
        nametableAddr = 0x2800;
    }
    if(tempBitBuffer[0] == 1 && tempBitBuffer[1] == 1)
    {
        nametableAddr = 0x2C00;
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
//FILE* logfile = fopen ("log/logdump", "w+");
void handleLog()
{
   // if(logfile == NULL)
    //{
        //printf("fuck\n");
        //return 1;
    //}
    //fprintf(logfile, "%X                                            A:%X X:%X Y:%X P:%X SP:%X \n", NESOB.pc, NESOB.a, NESOB.x, NESOB.y, NESOB.pflag, NESOB.sp);
}
