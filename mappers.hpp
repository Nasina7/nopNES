#include "sound.hpp"
bool handleRomLoad()
{
    switch (mapper)
    {
        case 0x00:
            NESOB.prgsize = NESOB.header[0x04] * 16384;
            printf("DEBUG: PRG ROM is 0x%X Bytes!\n",NESOB.prgsize);
            if(NESOB.prgsize > 0x8000)
            {
                //printf("ROM SIZE IS TO LARGE!\nTHIS ROM SIZE MAY NOT BE SUPPORTED.\n");
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
            fseek(rom,0x10 + NESOB.prgsize + 0x2000,SEEK_SET);
            fread(NESOB.PPUmemory, 0x1000, 1, rom);
            fseek(rom,0x10 + NESOB.prgsize + 0x2000,SEEK_SET);
            fread(NESOB.PPUmemory + 0x1000, 0x1000, 1, rom);
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

        case 0x07:
            printf("WARNING!  MAPPER 7 SUPPORT IS EXPERIMENTAL AND NOT FINISHED!\n");
            NESOB.prgsize = NESOB.header[0x04] * 16384;
            printf("DEBUG: PRG ROM is 0x%X Bytes!\n",NESOB.prgsize);
            NESOB.chrsize = NESOB.header[0x05] * 8192;
            printf("DEBUG: CHR ROM is 0x%X Bytes!\n",NESOB.chrsize);
            //printf("testing\n");
            rom = fopen(NESOB.filename, "rb");
            //printf("testing\n");
            fseek(rom,0x10,SEEK_SET);
            //printf("testing\n");
            fread(NESOB.memory + 0x8000,0x8000,1,rom);
            //printf("testing\n");
            //printf("testing\n");
            fseek(rom,NESOB.prgsize + 0x10,SEEK_SET);
            //printf("testing\n");
            fread(NESOB.PPUmemory, 0x2000, 1, rom);
            //printf("testing\n");
            fclose(rom);
        break;

        case 0x09:
            printf("WARNING!  MAPPER 9 SUPPORT IS VERY EXPERIMENTAL AND NOT FINISHED!\n");
            NESOB.prgsize = NESOB.header[0x04] * 16384;
            printf("DEBUG: PRG ROM is 0x%X Bytes!\n",NESOB.prgsize);
            NESOB.chrsize = NESOB.header[0x05] * 8192;
            printf("DEBUG: CHR ROM is 0x%X Bytes!\n",NESOB.chrsize);
            //printf("testing\n");
            rom = fopen(NESOB.filename, "rb");
            //printf("testing\n");
            fseek(rom,0x10,SEEK_SET);
            fread(NESOB.memory + 0x8000,0x2000,1,rom);
            fseek(rom,0x10 + NESOB.prgsize - 0x6000,SEEK_SET);
            fread(NESOB.memory + 0xA000,0x6000,1,rom);
            //printf("testing\n");
            //printf("testing\n");
            fseek(rom,NESOB.prgsize + 0x10,SEEK_SET);
            //printf("testing\n");
            fread(NESOB.PPUmemory, 0x2000, 1, rom);
            //printf("testing\n");
            fclose(rom);
            memDump();
        break;

        case 0x34: // GRAND DAD.  Shoutouts to Vargskelethor and Siivagunner
            printf("WARNING!  MAPPER 52 SUPPORT IS VERY EXPERIMENTAL AND VERY UNFINISHED!\n");
            printf("MAPPER 52 IS BOOTLEG MAPPER!\n");
            NESOB.prgsize = NESOB.header[0x04] * 16384;
            printf("DEBUG: PRG ROM is 0x%X Bytes!\n",NESOB.prgsize);
            NESOB.chrsize = NESOB.header[0x05] * 8192;
            printf("DEBUG: CHR ROM is 0x%X Bytes!\n",NESOB.chrsize);

            rom = fopen(NESOB.filename, "rb");

            fseek(rom,0x10,SEEK_SET);

            fread(NESOB.memory + 0x8000,0x4000,1,rom); // Load first bank into 0x8000 to 0xBFFF
            //printf("testing\n");
            rewind(rom);
            fseek(rom,(0x10 + 0x3C000),SEEK_SET);
            fread(NESOB.memory + 0xC000,0x4000,1,rom);
            //printf("testing\n");
            fseek(rom,NESOB.prgsize + 0x10,SEEK_SET);
            //printf("testing\n");
            fread(NESOB.PPUmemory, 0x2000, 1, rom);
            //printf("testing\n");
            fclose(rom);
            memDump();
        break;

        case 0x42:
            printf("WARNING!  MAPPER 66 SUPPORT IS EXPERIMENTAL!\n");
            NESOB.prgsize = NESOB.header[0x04] * 16384;
            printf("DEBUG: PRG ROM is 0x%X Bytes!\n",NESOB.prgsize);
            NESOB.chrsize = NESOB.header[0x05] * 8192;
            printf("DEBUG: CHR ROM is 0x%X Bytes!\n",NESOB.chrsize);
            //printf("testing\n");
            rom = fopen(NESOB.filename, "rb");
            //printf("testing\n");
            fseek(rom,0x10,SEEK_SET);
            //printf("testing\n");
            fread(NESOB.memory + 0x8000,0x8000,1,rom);
            //printf("testing\n");
            //printf("testing\n");
            fseek(rom,NESOB.prgsize + 0x10,SEEK_SET);
            //printf("testing\n");
            fread(NESOB.PPUmemory, 0x2000, 1, rom);
            //printf("testing\n");
            fclose(rom);
        break;

        case 90:
            printf("WARNING!  MAPPER 90 SUPPORT IS EXPERIMENTAL!\n");
            NESOB.prgsize = NESOB.header[0x04] * 16384;
            printf("DEBUG: PRG ROM is 0x%X Bytes!\n",NESOB.prgsize);
            NESOB.chrsize = NESOB.header[0x05] * 8192;
            printf("DEBUG: CHR ROM is 0x%X Bytes!\n",NESOB.chrsize);

            rom = fopen(NESOB.filename, "rb");

            fseek(rom,0x74810,SEEK_SET);
            fread(NESOB.memory + 0x8000,0x2000,1,rom);
            fseek(rom,0x7A010,SEEK_SET);
            fread(NESOB.memory + 0xA000,0x2000,1,rom);
            fseek(rom,0x7C010,SEEK_SET);
            fread(NESOB.memory + 0xC000,0x2000,1,rom);
            fseek(rom,0x7E010,SEEK_SET);
            fread(NESOB.memory + 0xE000,0x2000,1,rom); // Load first bank into 0x8000 to 0xBFFF
            fseek(rom,NESOB.prgsize + 0x10,SEEK_SET);
            fread(NESOB.PPUmemory, 0x1000, 1, rom);
            fseek(rom,NESOB.prgsize + 0x10,SEEK_SET);
            fread(NESOB.PPUmemory + 0x1000, 0x1000, 1, rom);
            //printf("testing\n");
            fclose(rom);
            memDump();
        break;

        case 113:
            NESOB.prgsize = NESOB.header[0x04] * 16384;
            printf("WARNING!  MAPPER 113 SUPPORT IS VERY EXPERIMENTAL AND VERY UNFINISHED!\n");
            printf("MAPPER 113 IS BOOTLEG? MAPPER!\n");
            printf("DEBUG: PRG ROM is 0x%X Bytes!\n",NESOB.prgsize);
            NESOB.chrsize = NESOB.header[0x05] * 8192;
            printf("DEBUG: CHR ROM is 0x%X Bytes!\n",NESOB.chrsize);
            //printf("testing\n");
            rom = fopen(NESOB.filename, "rb");
            //printf("testing\n");
            fseek(rom,0x10,SEEK_SET);
            //printf("testing\n");
            fread(NESOB.memory + 0x8000,0x8000,1,rom);
            //printf("testing\n");
            //printf("testing\n");
            fseek(rom,NESOB.prgsize + 0x10,SEEK_SET);
            //printf("testing\n");
            fread(NESOB.PPUmemory, 0x2000, 1, rom);
            //printf("testing\n");
            fclose(rom);
        break;

        case 209:
        mapper = 90;
        handleRomLoad();
        break;

        case 211:
        mapper = 90;
        handleRomLoad();
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
bool noEnter;
SDL_Rect message[4];
std::string textbArray[4];
const char *textbbuffer[4];
SDL_Texture *textb[4];
SDL_Color sdlBlack = {0, 0, 0,0};
int texWH[2];
bool beginning()
{
    TTF_Init();
    ramInitcount = 0;
    #ifdef _WIN32
        printf("WARNING!  You are using a windows build of nopNES!\nWindows Builds are experimental and thus may have bugs!\n");
    #endif // _WIN32
    //cout<<"Welcome to nopNES!"<<endl<<"Rom Name: ";
    //printf("Welcome to nopNES Beta v%s!\nMade by Nasina\nEnter Rom Name: ",currentVersion);
    TTF_Font* textboxFont = TTF_OpenFont("ubuntu.ttf", 28); //this opens a font style and sets a size
    TTF_Font* textboxFont2 = TTF_OpenFont("ubuntu.ttf", 20); //this opens a font style and sets a size
    TTF_Font* textboxFont3 = TTF_OpenFont("ubuntu.ttf", 18); //this opens a font style and sets a size
    while(noEnter == false)
    {
        SDL_SetRenderDrawColor(renderer,0xFF,0xFF,0xFF,0x00);
        SDL_RenderClear(renderer);
        if(textboxFont == NULL)
        {
            printf("TEXT ERROR!\n");
        }
        textbArray[0] = "Welcome to nopNES Beta v0.1!";
        textbArray[1] = "Made by Nasina";
        textbArray[2] = "Enter Rom Name:";
        textbArray[3] = "";
        textbbuffer[0] = textbArray[0].c_str();
        textbbuffer[1] = textbArray[1].c_str();
        textbbuffer[2] = textbArray[2].c_str();
        SDL_Surface* textboxTextSurf = TTF_RenderText_Solid(textboxFont, textbbuffer[0], sdlBlack);
        textb[0] = SDL_CreateTextureFromSurface(renderer, textboxTextSurf);

        textboxTextSurf = TTF_RenderText_Solid(textboxFont2, textbbuffer[1], sdlBlack);
        textb[1] = SDL_CreateTextureFromSurface(renderer, textboxTextSurf);

        textboxTextSurf = TTF_RenderText_Solid(textboxFont2, textbbuffer[2], sdlBlack);
        textb[2] = SDL_CreateTextureFromSurface(renderer, textboxTextSurf);

        textboxTextSurf = TTF_RenderText_Solid(textboxFont3, NESOB.filename, sdlBlack);
        textb[3] = SDL_CreateTextureFromSurface(renderer, textboxTextSurf);

        while( SDL_PollEvent( &SDL_EVENT_HANDLING)) // While Event to handle Random Stuff
        {
            switch(SDL_EVENT_HANDLING.type)
            {
                case SDL_QUIT:
                    return 1;
                break;

                case SDL_TEXTINPUT:
                    /* Add new text onto the end of our text */
                    strcat(NESOB.filename, SDL_EVENT_HANDLING.text.text);
                break;

                case SDL_KEYDOWN:
                    switch( SDL_EVENT_HANDLING.key.keysym.sym )
                    {
                        case SDLK_RETURN:
                            noEnter = true;
                        break;

                        case SDLK_BACKSPACE:
                            NESOB.filename[0] = '\0';
                        break;
                    }
                break;
            }
        }
        //printf("t: %s\n",NESOB.filename);
        SDL_QueryTexture(textb[0], NULL, NULL, &texWH[0], &texWH[1]);
        //message[0].y = 20;
        message[0].x = 256 - (texWH[0] / 2);
        message[0].y = 0;
        message[0].w = texWH[0];
        message[0].h = texWH[1];
        SDL_QueryTexture(textb[1], NULL, NULL, &texWH[0], &texWH[1]);
        message[1].x = 256 - (texWH[0] / 2);
        message[1].y = message[0].h;
        message[1].w = texWH[0];
        message[1].h = texWH[1];
        SDL_QueryTexture(textb[2], NULL, NULL, &texWH[0], &texWH[1]);
        message[2].y = 180;
        message[2].w = texWH[0];
        message[2].h = texWH[1];
        message[2].x = 256 - (texWH[0] / 2);
        SDL_QueryTexture(textb[3], NULL, NULL, &texWH[0], &texWH[1]);
        message[3].y = 200;
        message[3].w = texWH[0];
        message[3].h = texWH[1];
        message[3].x = 256 - (texWH[0] / 2);
        SDL_RenderCopy(renderer, textb[0], NULL, &message[0]);
        SDL_RenderCopy(renderer, textb[1], NULL, &message[1]);
        SDL_RenderCopy(renderer, textb[2], NULL, &message[2]);
        SDL_RenderCopy(renderer, textb[3], NULL, &message[3]);
        SDL_RenderPresent(renderer);
    }



    //cin>>NESOB.filename;
    SDL_RenderSetScale(renderer,2,2);
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
    printf("DEBUG: ROM MAPPER IS %i\n",mapper);
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
