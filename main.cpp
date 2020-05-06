#include "cpu.hpp"
using namespace std;
uint64_t prevCycles;
int main()
{
    rendererChoose = true;
    throttleMode = true;
    enableSound = true;
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
    renderer = SDL_CreateRenderer(nopNESwindow, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_STREAMING,256, 240);
    textureScanline = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_STREAMING,512, 240);
    textureScanline28 = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_STREAMING,256, 480);
    texture2400 = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_STREAMING,256, 240);
    SDL_RenderSetScale(renderer,2,2);

    if(beginning() == false)
    {
        printf("ERROR!\n");
        beginning();
    }
    graphicThread = false;
    currentFrame = 0;
    nesPowerOn();
    NESOB.scanline = 0;
    pale = fopen("pal.pal", "rb");
    fread(pallete,192,1,pale);
    fclose(pale);
    fpsBenchmark();
    NESOB.opcode = NESOB.memory[NESOB.pc];
    //if(enableSound == true)
    //{
        //thread sound (handleSound);
        //sound.detach();
    //}
    //breakpoint = true;
    #ifdef __linux__
    FILE* save = fopen("save.sav", "r");
    if(save != NULL)
    {
        fread(NESOB.memory + 0x6000,sizeof(uint8_t),0x2000,save);
    }
    fclose(save);
    #endif // __linux__
    while(true) // Begin Loop
    {
        prevScanline = NESOB.scanline;
        prevScanlineTimer = NESOB.cycles % cycleModulo;
        prevCycles = NESOB.cycles;
        doOpcode();
        //printf("pc: 0x%X\n",NESOB.pc);
        handleScanlineStuff();
        handleNMI();
        //handleInterrupts();
        //ifBreakpoint();
        if(prevScanline != NESOB.scanline)
        {
            handleOther();
            if(rendererChoose == true)
            {
                //blitsu = 2048;
                handleGraphicsBASICSCAN();
            }
            //drawT = false;

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
                printf("IRQ!\n");
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
                //tempBitBuffer = NESOB.memory[0x4000];
                //cout<<tempBitBuffer<<endl;
                handleSound();
            }
            FILE* save = fopen("save.sav", "w+");
            fwrite(NESOB.memory + 0x6000,sizeof(uint8_t),0x2000,save);
            fclose(save);
            handleSoundTimers();
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
