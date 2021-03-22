#include "cpu.hpp"
using namespace std;
uint64_t prevCycles;
int windowH;
int windowW;
float windowH2;
float windowW2;
bool firstRun = false;
uint16_t addedCycles;
int main()
{
    rendererChoose = true;
    throttleMode = true;
    enableSound = true;
    toggleRender = true;
    playTAS = false;
    initTAS = false;
    swapRender = false;
    srand(time(NULL));
    if( SDL_Init( SDL_INIT_EVERYTHING ) < 0)
    {
        printf("SDL2 was Unable to Initialize!");
        return 1;
    }
    if(Mix_OpenAudio(48000,AUDIO_U8,2,1024)==-1)
    {
        printf("Mix_OpenAudio: %s\n",Mix_GetError());
        return 2;
    }
    nopNESwindow = SDL_CreateWindow("nopNES Beta", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 512, 480, SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(nopNESwindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    notiText = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGB888,SDL_TEXTUREACCESS_STREAMING,128, 20);
    texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_STREAMING,256, 240);
    textureScanline = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGB888,SDL_TEXTUREACCESS_STREAMING,512, 240);
    textureScanlineF = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGB888,SDL_TEXTUREACCESS_STREAMING,256, 240);
    textureScanline28 = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGB888,SDL_TEXTUREACCESS_STREAMING,256, 480);
    texture2400 = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_STREAMING,256, 240);
    restartNES:
    if(beginning() == false)
    {
        printf("ERROR!\n");
        beginning();
    }
    graphicThread = false;
    currentFrame = 0;
    nesPowerOn();
    NESOB.scanline = 0;
    NESOB.cycles = 0;
    pale = fopen("pal.pal", "rb");
    fread(pallete,192,1,pale);
    fclose(pale);
    #ifdef __linux__
    if(firstRun == false)
    {
        fpsBenchmark();
        firstRun = true;
    }
    #endif
    NESOB.opcode = NESOB.memory[NESOB.pc];
    #ifdef __linux__
    //FILE* save = fopen("save.sav", "r+");
    //if(save != NULL)
    //{
    //    fread(NESOB.memory + 0x6000,sizeof(uint8_t),0x2000,save);
    //}
    //fclose(save);
    #endif // __linux__
    displayMessagebox("Emulation Started",120);
    //NESOB.pc = 0xC000;
    //nestestLog = fopen("nestest.log", "r");
    //breakpoint = true;
    while(true) // Begin Loop
    {
        prevScanline = NESOB.scanline;
        prevScanlineTimer = NESOB.cycles % cycleModulo;
        prevCycles = NESOB.cycles;
        previousPC = NESOB.pc;
        //printf("PC 0x%X\n",NESOB.pc);
        doOpcode();
        addedCycles = NESOB.cycles - prevCycles;
        NESOB.cycles += addedCycles;
        NESOB.cycles += addedCycles;
        //nestestCompare();
        //NESOB.cycles += 4;
        handleScanlineStuff();
        handleMMC3irqREDO();
        handleNMI();
        //ifBreakpoint();
        if(prevScanline != NESOB.scanline)
        {
            if(rendererChoose == true)
            {
                if(toggleRender == false)
                {
                    handleGraphicsBASICSCAN();
                }
                if(toggleRender == true)
                {
                    handleGraphicsScan();
                }
            }
            handleOther();
        }
        if( prevScanline != NESOB.scanline && NESOB.scanline == 255 )
        {
            currentFrame++;
            if(corruptorEnable == true && currentFrame % perFrame == 0)
            {
                handleCorruptor();
            }
            if(NESOB.memory[0x4017] - 4 > NESOB.memory[0x4017])
            {
                NESOB.memory[0x4017] -= 4;
                printf("SOUND IRQ!\n");
                soundIRQ = true;
            }
            if(NESOB.closeProgram == true)
            {
                memDump();
                exitLoop = true;
                return 0;
            }
            if(enableSound == true)
            {
                handleSound();
            }
            //FILE* save = fopen("save.sav", "w+");
            //fwrite(NESOB.memory + 0x6000,sizeof(uint8_t),0x2000,save);
            //fclose(save);
            handleSoundTimers();
            SDL_GetWindowSize(nopNESwindow, &windowW, &windowH);
            windowW2 = windowW;
            windowH2 = windowH;
            SDL_RenderSetScale(renderer, windowW2 / 256, windowH2 / 240);

            if(graphicThread == false && rendererChoose == false)
            {
                handleGraphicsBASIC();
            }
            //if(playTAS == false)
            //{

            //}
            if(playTAS == true)
            {
                handleInputTas();
            }
            handleSDLcontrol();
            if(enableRewind == true && currentFrame % 3 == 0 && rewindActive == false)
            {
                handleRewindBuffer();
            }
            if(resN == true)
            {
                resN = false;
                goto restartNES;
            }
            if(initTAS == true)
            {
                tasLocation = 0;
                initTAS = false;
                NESOB.sp -= 3;
                NESOB.pflag = NESOB.pflag | 0x04;
                NESOB.memory[0x4015] = 0;
                NESOB.pc = NESOB.memory[0xFFFD] << 8 | NESOB.memory[0xFFFC];
                displayMessagebox("Reset!    ",120);
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
                playTAS = true;
            }
        }

    }
    return 0;
}

int WinMain()  // This is here to fix the Windows Build
{
    main();
    return 0;
}
