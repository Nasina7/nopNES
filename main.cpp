#include "cpu.hpp"
using namespace std;
int main()
{
    if( SDL_Init( SDL_INIT_EVERYTHING ) < 0)
    {
        printf("SDL2 was Unable to Initialize!");
        return 1;
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
        return 0;
    }
    //printf("Would you like to run the graphics on a seperate thread? (This will increase speed at the expense of accuracy.)\n");
    //cin>>options;
    //if(options == 'y')
    //{
        graphicThread = false;
        //printf("THIS IS CURRENTLY BROKEN!\n SWITCHING TO DEFAULT...\n");
    //}
    //if(options == 'n')
    //{
       // graphicThread = false;
    //}
    //cout<<graphicThread<<endl;
    currentFrame = 0;
    nesPowerOn();
    NESOB.scanline = 0;
    pal = fopen("pal.pal", "rb");
    fread(pallete,192,1,pal);
    fclose(pal);
    //printf("hmm\n");
    //if(graphicThread == true)
    //{
        //thread graphic (handleGraphicsBASIC);
    //}
    //printf("hmm\n");
    #ifdef __linux__
        thread input (handleSDLcontrol);
    #endif // __linux__
    //printf("hmm\n");
    //thread throttle (throttleCPUfunc);
    //throttle.detach();
    fpsBenchmark();
    NESOB.opcode = NESOB.memory[NESOB.pc];
    //NESOB.memory[0x0067] = 0xFF;
    uint64_t prevCycles;
    uint16_t prevScanline;
    while(NESOB.dontSetTrue == false) // Begin Loop
    {
        //breakpoint = true;
        //SDLinput = true;
        //breakpoint = true;
        prevScanline = NESOB.scanline;
        throttleCPU = true;
        NESOB.opcode = NESOB.memory[NESOB.pc];
        prevScanlineTimer = NESOB.cycles % cycleModulo;
        //printf("Opcode: 0x%X\n",NESOB.opcode);
        //cout<<newOpcode2<<endl;
        //printf("PC: 0x%X\n",NESOB.pc);
        prevCycles = NESOB.cycles;
        doOpcode();
        handleOther();
        handleInterrupts();
        handleScanlineStuff();
        //breakpoint = true;
        //printRegs();
        //handleLog();
        //printRegs();
        if(NESOB.pc == 0x03A2 && NESOB.a == 0x00 && NESOB.x == 0x01 && NESOB.y == 0x02)
        {
            //printf("Scanline: 0x%i\n",NESOB.scanline);
            //breakpoint = true;
        }
        if(NESOB.memory[0x0001] == 0x3F)
        {
            //breakpoint = true;
        }
        if(breakpoint == true)
        {
            //printf("MMC1: %i\n",MMC1counter);
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
            //fclose (logfile);
            return 0;
        }
        if(prevScanline < NESOB.scanline)
        {
            //handleGraphicsBASICSCAN();
        }
        if( (prevCycles % 29780) > (NESOB.cycles % 29780) )
        {
            if(graphicThread == false)
            {
                handleGraphicsBASIC();
            }
            if(graphicThread == true)
            {
                SDL_Delay(6);
            }
            #ifdef _WIN32
                handleSDLcontrol();
            #endif // _WIN32
            //throttleCPU = true;
            //while(doneThrottle == false)
            //{

            //}
            //thread throttle (throttleCPUfunc);
            //throttle.detach();
        }

    }
    return 0;
}

int WinMain()
{
    main();
    return 0;
}
