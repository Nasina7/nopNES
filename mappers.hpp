#include "sound.hpp"
bool handleRomLoad()
{
    switch (mapper)
    {
        case 0x00:
            NESOB.prgsize = NESOB.header[0x04] * 16384;
            printf("DEBUG: The PRG ROM is 0x%X Bytes!\n",NESOB.prgsize);
            if(NESOB.prgsize > 0x8000)
            {
                //printf("ROM SIZE IS TO LARGE!\nTHIS ROM SIZE MAY NOT BE SUPPORTED.\n");
                //return false;
                NESOB.prgsize = 0x8000;
            }
            NESOB.chrsize = NESOB.header[0x05] * 8192;
            printf("DEBUG: The CHR ROM is 0x%X Bytes!\n",NESOB.chrsize);
            //printf("testing\n");
            rom = fopen(NESOB.filename, "rb");
            //printf("testing\n");
            fseek(rom,0x10,SEEK_SET);
            //printf("testing\n");
            fread(NESOB.memory + 0x8000,NESOB.prgsize,1,rom);
            //printf("testing\n");
            if(NESOB.prgsize != 0x8000)
            {
                rewind(rom);
                fseek(rom,0x10,SEEK_SET);
                fread(NESOB.memory + 0xC000,NESOB.prgsize,1,rom);
            }
            //printf("testing\n");
            fseek(rom,NESOB.prgsize + 0x10,SEEK_SET);
            //printf("testing\n");
            fread(NESOB.PPUmemory, 0x2000, 1, rom);
            //printf("testing\n");
            fclose(rom);
        break;

        case 0x01:
            printf("WARNING!  MAPPER 1 SUPPORT IS EXPERIMENTAL!\n");
            NESOB.prgsize = NESOB.header[0x04] * 16384;
            printf("DEBUG: PRG ROM is 0x%X Bytes!\n",NESOB.prgsize);
            NESOB.chrsize = NESOB.header[0x05] * 8192;
            printf("DEBUG: CHR ROM is 0x%X Bytes!\n",NESOB.chrsize);

            rom = fopen(NESOB.filename, "rb");

            fseek(rom,0x10,SEEK_SET);

            fread(NESOB.memory + 0x8000,0x4000,1,rom); // Load first bank into 0x8000 to 0xBFFF
            //printf("testing\n");
            rewind(rom);
            fseek(rom,(0x10 + NESOB.prgsize - 0x4000),SEEK_SET);
            fread(NESOB.memory + 0xC000,0x4000,1,rom);
            //printf("testing\n");
            fseek(rom,NESOB.prgsize + 0x10,SEEK_SET);
            //printf("testing\n");
            fread(NESOB.PPUmemory, 0x2000, 1, rom);
            //printf("testing\n");
            fclose(rom);
        break;

        case 0x02:
            printf("WARNING!  MAPPER 2 SUPPORT IS EXPERIMENTAL!\n");
            NESOB.prgsize = NESOB.header[0x04] * 16384;
            printf("DEBUG: PRG ROM is 0x%X Bytes!\n",NESOB.prgsize);
            NESOB.chrsize = NESOB.header[0x05] * 8192;
            printf("DEBUG: CHR ROM is 0x%X Bytes!\n",NESOB.chrsize);

            rom = fopen(NESOB.filename, "rb");

            fseek(rom,0x10,SEEK_SET);

            fread(NESOB.memory + 0x8000,0x4000,1,rom); // Load first bank into 0x8000 to 0xBFFF
            //printf("testing\n");
            rewind(rom);
            fseek(rom,(0x10 + NESOB.prgsize - 0x4000),SEEK_SET);
            fread(NESOB.memory + 0xC000,0x4000,1,rom);
            //printf("testing\n");
            fseek(rom,NESOB.prgsize + 0x10,SEEK_SET);
            //printf("testing\n");
            fread(NESOB.PPUmemory, 0x2000, 1, rom);
            //printf("testing\n");
            fclose(rom);
        break;

        case 0x03:
            printf("WARNING!  MAPPER 3 SUPPORT IS EXPERIMENTAL!\n");
            NESOB.prgsize = NESOB.header[0x04] * 16384;
            printf("DEBUG: PRG ROM is 0x%X Bytes!\n",NESOB.prgsize);
            NESOB.chrsize = NESOB.header[0x05] * 8192;
            printf("DEBUG: CHR ROM is 0x%X Bytes!\n",NESOB.chrsize);

            rom = fopen(NESOB.filename, "rb");

            fseek(rom,0x10,SEEK_SET);

            fread(NESOB.memory + 0x8000,0x4000,1,rom); // Load first bank into 0x8000 to 0xBFFF
            //printf("testing\n");
            rewind(rom);
            fseek(rom,(0x10 + NESOB.prgsize - 0x4000),SEEK_SET);
            fread(NESOB.memory + 0xC000,0x4000,1,rom);
            //printf("testing\n");
            fseek(rom,NESOB.prgsize + 0x6010,SEEK_SET);
            //printf("testing\n");
            fread(NESOB.PPUmemory, 0x2000, 1, rom);
            //printf("testing\n");
            fclose(rom);
        break;

        case 0x04:
            printf("WARNING!  MAPPER 4 SUPPORT IS VERY EXPERIMENTAL AND NOT FINISHED!\n");
            NESOB.prgsize = NESOB.header[0x04] * 16384;
            printf("DEBUG: PRG ROM is 0x%X Bytes!\n",NESOB.prgsize);
            NESOB.chrsize = NESOB.header[0x05] * 8192;
            printf("DEBUG: CHR ROM is 0x%X Bytes!\n",NESOB.chrsize);

            rom = fopen(NESOB.filename, "rb");

            fseek(rom,0x10,SEEK_SET);

            fread(NESOB.memory + 0x8000,0x4000,1,rom); // Load first bank into 0x8000 to 0xBFFF
            //printf("testing\n");
            rewind(rom);
            fseek(rom,(0x10 + NESOB.prgsize - 0x4000),SEEK_SET);
            fread(NESOB.memory + 0xC000,0x4000,1,rom);
            //printf("testing\n");
            fseek(rom,NESOB.prgsize + 0x10,SEEK_SET);
            //printf("testing\n");
            fread(NESOB.PPUmemory, 0x2000, 1, rom);
            //printf("testing\n");
            fclose(rom);
        break;

        case 0x42:
            printf("WARNING!  MAPPER 66 SUPPORT IS EXPERIMENTAL!\n");
            NESOB.prgsize = NESOB.header[0x04] * 16384;
            printf("DEBUG: PRG ROM is 0x%X Bytes!\n",NESOB.prgsize);
            if(NESOB.prgsize > 0x8000)
            {
                printf("DEBUG: ROM SIZE IS TO LARGE!\nTHIS ROM SIZE MAY NOT BE SUPPORTED.\n");
                //return false;
                NESOB.prgsize = 0x8000;
            }
            NESOB.chrsize = NESOB.header[0x05] * 8192;
            printf("DEBUG: CHR ROM is 0x%X Bytes!\n",NESOB.chrsize);
            //printf("testing\n");
            rom = fopen(NESOB.filename, "rb");
            //printf("testing\n");
            fseek(rom,0x10,SEEK_SET);
            //printf("testing\n");
            fread(NESOB.memory + 0x8000,NESOB.prgsize,1,rom);
            //printf("testing\n");
            if(NESOB.prgsize != 0x8000)
            {
                rewind(rom);
                fseek(rom,0x10,SEEK_SET);
                fread(NESOB.memory + 0xC000,NESOB.prgsize,1,rom);
            }
            //printf("testing\n");
            fseek(rom,NESOB.prgsize + 0x10,SEEK_SET);
            //printf("testing\n");
            fread(NESOB.PPUmemory, 0x2000, 1, rom);
            //printf("testing\n");
            fclose(rom);
        break;

        default:
            printf("ERROR: MAPPER %i IS NOT SUPPORTED!\n",mapper);
            printf("Would you like to use a different mapper?\n");
            cin>>options;
            if(options == 'y')
            {
                printf("Please enter a different mapper: ");
                cin>>mapper;
                mapper = mapper - 48; // IDK why this needs to be here.
                handleRomLoad();
            }
            if(options == 'n')
            {
                return false;
            }
        break;

    }
    return true;
}
uint16_t ramInitcount;
bool beginning()
{
    ramInitcount = 0;
    #ifdef _WIN32
        printf("WARNING!  You are using a windows build of nopNES!\nWindows Builds are experimental and thus may have bugs!\n");
    #endif // _WIN32
    //cout<<"Welcome to nopNES!"<<endl<<"Rom Name: ";
    printf("Welcome to nopNES!\nMade by Tails2600\nEnter Rom Name: ");
    cin>>NESOB.filename;
    FILE* headerf = fopen(NESOB.filename, "rb");
    if(headerf == NULL)
    {
        return false;
    }
    fread(NESOB.header,0x10,1,headerf);
    fclose(headerf);
    mapperBit = NESOB.header[6] >> 4;
    mapper = mapperBit.to_ulong();
    mapperBit2 = NESOB.header[7] >> 4;
    mapper = (mapperBit2.to_ulong()) << 4 | (mapperBit.to_ulong());
    printf("This rom's mapper is %i.\n",mapper);
    while(ramInitcount != 0x2000)
    {
        if((ramInitcount % 8) <= 3)
        {
            NESOB.memory[ramInitcount] = 0x00;
        }
        if((ramInitcount % 8) > 3)
        {
            NESOB.memory[ramInitcount] = 0xFF;
        }
        ramInitcount++;
    }
    if(handleRomLoad() == false)
    {
        return false;
    }
    return true;
}
