#include "cpu.hpp"
using namespace std;
int main()
{
    if( SDL_Init( SDL_INIT_EVERYTHING ) < 0)
    {
        printf("SDL2 was Unable to Initialize!");
        return 1;
    }
    if(Mix_OpenAudio(48000,AUDIO_S8,1,800)==-1)
    {
        printf("Mix_OpenAudio: %s\n",Mix_GetError());
        return 2;
    }
    nopNESwindow = SDL_CreateWindow("nopNES Alpha", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 512, 480, SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(nopNESwindow, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTexture
    (
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        256, 240
    );
    textureScanline = SDL_CreateTexture
    (
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        512, 256
    );
    texture2400 = SDL_CreateTexture
    (
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        256, 240
    );
    SDL_RenderSetScale(renderer,2,2);
    if(beginning() == false)
    {
        printf("ERROR!\n");
        beginning();
    }
    printf("Would you like to use the scanline renderer?\nThis will improve accuracy of some games but it is not finished. (y or n)\n");
    cin>>options;
    if(options == 'y')
    {
        rendererChoose = true;
    }
    if(options == 'n')
    {
        rendererChoose = false;
    }
    printf("Would you like to enable sound?\nVERY WIP!  WILL HURT YOUR EARS! (y or n)\n");
    cin>>options;
    if(options == 'y')
    {
        enableSound = true;
    }
    if(options == 'n')
    {
        enableSound = false;
    }
    graphicThread = false;
    currentFrame = 0;
    nesPowerOn();
    NESOB.scanline = 0;
    pal = fopen("pal.pal", "rb");
    fread(pallete,192,1,pal);
    fclose(pal);
    fpsBenchmark();
    NESOB.opcode = NESOB.memory[NESOB.pc];
    uint64_t prevCycles;
    uint16_t prevScanline;
    while(NESOB.dontSetTrue == false) // Begin Loop
    {
        prevScanline = NESOB.scanline;
        throttleCPU = true;
        NESOB.opcode = NESOB.memory[NESOB.pc];
        prevScanlineTimer = NESOB.cycles % cycleModulo;
        //printf("Opcode: 0x%X\n",NESOB.opcode);
        //printf("PC: 0x%X\n",NESOB.pc);
        prevCycles = NESOB.cycles;
        doOpcode();
        NESOB.Pbitbuffer = NESOB.pflag;
        //cout<<NESOB.Pbitbuffer<<endl;
        handleOther();
        handleInterrupts();
        handleScanlineStuff();
        //breakpoint = true;
        //printRegs();
        //handleLog();
        if(breakpoint == true)
        {
            printRegs();
            printf("Continue?\n");
            cin>>options;
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
        if(restartNopNES == true)
        {
            main();
        }
        if(NESOB.closeProgram == true)
        {
            memDump();
            exitLoop = true;
            return 0;
        }
        if(prevScanline < NESOB.scanline || prevScanline > NESOB.scanline)
        {
            if(rendererChoose == true)
            {
                blitsu = 2048;
                handleGraphicsBASICSCAN();
            }
        }
        if( (prevCycles % 29780) > (NESOB.cycles % 29780) )
        {
            if(enableSound == true)
            {
                handleSound();
            }
            if(graphicThread == false && rendererChoose == false)
            {
                handleGraphicsBASIC();
            }
            handleSDLcontrol();
            SDL_Delay(throttleCPUval);
        }

    }
    return 0;
}

int WinMain()  // This is here to fix the Windows Build
{
    main();
    return 0;
}
