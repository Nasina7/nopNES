#include <iostream>
#include <string>
#include <bitset>

class NESCPU
{
    public:
        uint8_t memory[0x10001];
        uint8_t header[0x10];
        uint8_t a;
        uint8_t x;
        uint8_t y;
        uint16_t pc;
        uint8_t sp;
        unsigned char pflag;
        char filename[50];
        int prgsize;
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

using namespace std;
NESCPU NESOB;
int beginning()
{

    cout<<"Welcome to nopNES!"<<endl<<"Rom Name: ";
    cin>>NESOB.filename;
    FILE* headerf = fopen(NESOB.filename, "rb");
    fread(NESOB.header,0x10,1,headerf);
    fclose(headerf);
    NESOB.prgsize = NESOB.header[0x04] * 16384;
    printf("PRG ROM is 0x%X Bytes!\n",NESOB.prgsize);
    FILE* rom = fopen(NESOB.filename, "rb");
    fseek(rom,0x10,SEEK_SET);
    fread(NESOB.memory + 0x8000,NESOB.prgsize,1,rom);
    rewind(rom);
    fseek(rom,0x10,SEEK_SET);
    fread(NESOB.memory + 0xC000,NESOB.prgsize,1,rom);
    fclose(rom);
    FILE* mem_dump = fopen ("log/memdump", "w+");
    fwrite (NESOB.memory , sizeof(char), sizeof(NESOB.memory), mem_dump);
    fclose (mem_dump);
    return 0;
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
int handleFlags(uint8_t value, uint8_t prevValue)
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
