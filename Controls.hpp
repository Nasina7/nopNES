#include "graphics.hpp"
uint8_t registerArray[15];
uint8_t rewindBuffer[72][0x1410F];
uint8_t rewindBufferNumber = 0;
uint32_t copyBufHRB;
uint16_t copyPPU;
uint8_t RBNbackup;
FILE* savestate = NULL;
FILE* saveload = NULL;
void handleRewindBuffer() // Called once every 5 frames to make a savestate
{
    copyBufHRB = 0;
    while(copyBufHRB != 0x10000)
    {
        rewindBuffer[rewindBufferNumber][copyBufHRB] = NESOB.memory[copyBufHRB];
        copyBufHRB++;
    }
    copyPPU = 0;
    while(copyPPU != 0x4000)
    {
        rewindBuffer[rewindBufferNumber][copyBufHRB] = NESOB.PPUmemory[copyPPU];
        copyBufHRB++;
        copyPPU++;
    }
    copyPPU = 0;
    while(copyPPU != 0x100)
    {
        rewindBuffer[rewindBufferNumber][copyBufHRB] = OAMmem[copyPPU];
        copyBufHRB++;
        copyPPU++;
    }
    //CopyBufHRB should be at 0x14100 here
    registerArray[0] = NESOB.pc >> 8;
    registerArray[1] = NESOB.pc;
    registerArray[2] = NESOB.sp;
    registerArray[3] = NESOB.a;
    registerArray[4] = NESOB.x;
    registerArray[5] = NESOB.y;
    registerArray[6] = NESOB.pflag;
    registerArray[7] = NESOB.cycles >> 24;
    registerArray[8] = NESOB.cycles >> 16;
    registerArray[9] = NESOB.cycles >> 8;
    registerArray[10] = NESOB.cycles;
    registerArray[11] = NESOB.scanline >> 8;
    registerArray[12] = NESOB.scanline;
    registerArray[13] = Loopy >> 8;
    registerArray[14] = Loopy;
    copyPPU = 0;
    while(copyPPU != 15)
    {
        rewindBuffer[rewindBufferNumber][copyBufHRB] = registerArray[copyPPU];
        copyBufHRB++;
        copyPPU++;
    }
    rewindBufferNumber++;
    if(rewindBufferNumber == 73)
    {
        rewindBufferNumber = 0;
    }
    RBNbackup = rewindBufferNumber;
    if(RBNbackup == 72)
    {
        RBNbackup = 71;
    }
}
bool firstPresssHandleRewind;
uint8_t isFirstAPress;
int handleRewind()
{
    if(rewindBufferNumber > 72)
    {
        printf("ERROR! RBN IS %i\n",rewindBufferNumber);
        return 2;
    }
    copyBufHRB = 0;
    while(copyBufHRB != 0x10000)
    {
        NESOB.memory[copyBufHRB] = rewindBuffer[rewindBufferNumber][copyBufHRB];
        copyBufHRB++;
    }
    copyPPU = 0;
    while(copyPPU != 0x4000)
    {
        NESOB.PPUmemory[copyPPU] = rewindBuffer[rewindBufferNumber][copyBufHRB];
        copyBufHRB++;
        copyPPU++;
    }
    copyPPU = 0;
    while(copyPPU != 0x100)
    {
        OAMmem[copyPPU] = rewindBuffer[rewindBufferNumber][copyBufHRB];
        copyBufHRB++;
        copyPPU++;
    }
    //CopyBufHRB should be at 0x14100 here
    copyPPU = 0;
    while(copyPPU != 15)
    {
        registerArray[copyPPU] = rewindBuffer[rewindBufferNumber][copyBufHRB];
        copyBufHRB++;
        copyPPU++;
    }
    NESOB.pc = registerArray[0] << 8 | registerArray[1];
    NESOB.sp = registerArray[2];
    NESOB.a = registerArray[3];
    NESOB.x = registerArray[4];
    NESOB.y = registerArray[5];
    NESOB.pflag = registerArray[6];
    NESOB.cycles = registerArray[7] << 24 | registerArray[8] << 16 | registerArray[9] << 8 | registerArray[10];
    NESOB.scanline = registerArray[11] << 8 | registerArray[12];
    Loopy = registerArray[13] << 8 | registerArray[14];
    //printf("RBN: %i   RBNB: %i\n",rewindBufferNumber,RBNbackup);
    --rewindBufferNumber;
    if(rewindBufferNumber == 0xFF)
    {
        rewindBufferNumber = 71;
    }
    if(rewindBufferNumber == RBNbackup + 1)
    {
        rewindBufferNumber++;
        if(rewindBufferNumber == 72)
        {
            rewindBufferNumber = 0;
        }
    }
    return 0;
}
void saveNESstate()
{
            if(savestatename[0] == NULL)
            {
                printf("Type name for save state.\n");
                scanf("%s",savestatename);
            }
            savestate = fopen(savestatename,"w+");
            fwrite(NESOB.memory,sizeof(uint8_t),0x10000,savestate);
            fwrite(NESOB.PPUmemory,sizeof(uint8_t),0x4000,savestate);
            fwrite(OAMmem,sizeof(uint8_t),0x100,savestate);
            registerArray[0] = NESOB.pc >> 8;
            registerArray[1] = NESOB.pc;
            registerArray[2] = NESOB.sp;
            registerArray[3] = NESOB.a;
            registerArray[4] = NESOB.x;
            registerArray[5] = NESOB.y;
            registerArray[6] = NESOB.pflag;
            registerArray[7] = NESOB.cycles >> 24;
            registerArray[8] = NESOB.cycles >> 16;
            registerArray[9] = NESOB.cycles >> 8;
            registerArray[10] = NESOB.cycles;
            registerArray[11] = NESOB.scanline >> 8;
            registerArray[12] = NESOB.scanline;
            registerArray[13] = Loopy >> 8;
            registerArray[14] = Loopy;
            fwrite(registerArray,sizeof(uint8_t),15,savestate);
            fclose(savestate);
            printf("State Saved!\n");
            displayMessagebox("State Saved",120);
            //breakpoint = true;
}
void loadNESstate()
{
            if(savestatename[0] == NULL)
            {
                printf("Type name for save state.\n");
                scanf("%s",savestatename);
            }
            saveload = fopen(savestatename,"r");
            fread(NESOB.memory,sizeof(uint8_t),0x10000,saveload);
            fread(NESOB.PPUmemory,sizeof(uint8_t),0x4000,saveload);
            fread(OAMmem,sizeof(uint8_t),0x100,saveload);
            fread(registerArray,sizeof(uint8_t),15,saveload);
            fclose(saveload);
            NESOB.pc = registerArray[0] << 8 | registerArray[1];
            NESOB.sp = registerArray[2];
            NESOB.a = registerArray[3];
            NESOB.x = registerArray[4];
            NESOB.y = registerArray[5];
            NESOB.pflag = registerArray[6];
            NESOB.cycles = registerArray[7] << 24 | registerArray[8] << 16 | registerArray[9] << 8 | registerArray[10];
            NESOB.scanline = registerArray[11] << 8 | registerArray[12];
            Loopy = registerArray[13] << 8 | registerArray[14];
            printf("State Loaded!\n");
            displayMessagebox("State Loaded",120);
}
bool resN;
bool rewind2;
int handleControlsr()
{
    switch( SDL_EVENT_HANDLING.key.keysym.sym )
        {

        case SDLK_LSHIFT:
            aPress = false;
            //rewind2 = false;
            //rewindActive = false;
            //isFirstAPress = 0;
        break;

        case SDLK_i:
            saveNESstate();
        break;

        case SDLK_o:
            loadNESstate();
        break;

        case SDLK_2:
            if(enableRewind == true)
            {
                enableRewind = false;
                printf("Rewind Disabled!\n");
                displayMessagebox("Disabled Rewind",120);
                break;
            }
            if(enableRewind == false)
            {
                enableRewind = true;
                printf("Rewind Enabled!\n");
                displayMessagebox("Enabled Rewind",120);
            }
        break;

        case SDLK_t:
            handleCorruptorKey();
        break;

        case SDLK_1:
            resN = true;
            noEnter = false;
            SDL_RenderSetScale(renderer, 1, 1);
        break;

        case SDLK_y:
            if(throttleMode == false)
            {
                throttleMode = true;
                cout<<"Throttle Mode switched to Automatic!"<<endl;
                break;
            }
            if(throttleMode == true)
            {
                throttleMode = false;
                cout<<"Throttle Mode switched to Manual!"<<endl;
                break;
            }
        break;

        case SDLK_u:
            if(rendererChoose == true)
            {
                rendererChoose = false;
                blitsu = 1024;
                cout<<"Switched to legacy renderer."<<endl;
                break;
            }
            if(rendererChoose == false)
            {
                rendererChoose = true;
                blitsu = 2048;
                cout<<"Switched to modern renderer."<<endl;
                break;
            }
        break;

        case SDLK_r:
            NESOB.sp -= 3;
            NESOB.pflag = NESOB.pflag | 0x04;
            NESOB.memory[0x4015] = 0;
            NESOB.pc = NESOB.memory[0xFFFD] << 8 | NESOB.memory[0xFFFC];
            displayMessagebox("Reset!    ",120);
        break;

        //case SDLK_b:
        //    cout<<blitsu<<endl;
        //    cin>>blitsu;
        //break;

        case SDLK_e:
            if(enableSound == true)
            {
                enableSound = false;
                displayMessagebox("Disabled Sound",120);
                cout<<"Sound is disabled!  Press E to enable."<<endl;
                break;
            }
            if(enableSound == false)
            {
                enableSound = true;
                displayMessagebox("Enabled Sound",120);
                cout<<"Sound is enabled!  Press E to disable."<<endl;
                break;
            }
        break;

        case SDLK_l:
            bit4 = toggleRender;
            bit4.flip(0);
            toggleRender = bit4.to_ulong();
        break;

        case SDLK_q:
            if(throttleMode == true)
            {
                cout<<"Throttle Mode is currently Automatic!"<<endl<<"Change it to manual by pressing Y!"<<endl<<"Current value is "<<throttleCPUval<<"."<<endl;
                break;
            }
            printf("Increase this number to slow down the Emulator.\nCurrent Value: %i\nDefault is 0\nChange throttle value to: ",throttleCPUval);
            cin>>throttleCPUval;
        break;

        case SDLK_w:
            printf("This will overclock or underclock the emulated cpu.\nDefault is 113\nCurrent Value: %i\nChange Cycle Modulo too: ",cycleModulo);
            cin>>cycleModulo;
        break;

        case SDLK_z:
            controlBuffer[1] = 0;
        break;

        case SDLK_x:
            controlBuffer[0] = 0;
        break;

        case SDLK_SPACE:
            controlBuffer[2] = 0;
        break;

        case SDLK_RETURN:
            controlBuffer[3] = 0;
        break;

        case SDLK_LCTRL:
            controlBuffer[3] = 0;
        break;

        case SDLK_UP:
            controlBuffer[4] = 0;
        break;

        case SDLK_DOWN:
            controlBuffer[5] = 0;
        break;

        case SDLK_LEFT:
            controlBuffer[6] = 0;
        break;

        case SDLK_RIGHT:
            controlBuffer[7] = 0;
        break;

        }
    return 0;
}
int handleControls()
{
    switch( SDL_EVENT_HANDLING.key.keysym.sym )
        {

        case SDLK_m:
            memDump();
            displayMessagebox("Dumped Ram",120);
        break;

        case SDLK_p:
            printRegs();
        break;
        /*
        case SDLK_o:
            tempBitBuffer = NESOB.memory[0x2000];
            #ifdef __linux__
                std::cout<<tempBitBuffer<<std::endl;
            #endif
        break;

        case SDLK_i:
            tempBitBuffer = NESOB.memory[0x2002];
            #ifdef __linux__
                std::cout<<tempBitBuffer<<std::endl;
            #endif
        break;

        case SDLK_LEFTBRACKET:
            showTile = true;
        break;

        case SDLK_RIGHTBRACKET:
            showBlock = true;
        break;
        */
        case SDLK_LSHIFT:
            aPress = true;
            //rewindActive = true;
            //handleRewind();
        break;

        case SDLK_BACKSLASH:
            showGrid = true;
        break;

        case SDLK_z:
            controlBuffer[1] = 1;
        break;

        case SDLK_x:
            controlBuffer[0] = 1;
        break;

        case SDLK_SPACE:
            controlBuffer[2] = 1;
        break;

        case SDLK_RETURN:
            controlBuffer[3] = 1;
            //printf("Return!");
        break;

        case SDLK_LCTRL:
            controlBuffer[3] = 1;
        break;

        case SDLK_UP:
            controlBuffer[4] = 1;
        break;

        case SDLK_DOWN:
            controlBuffer[5] = 1;
        break;

        case SDLK_LEFT:
            controlBuffer[6] = 1;
        break;

        case SDLK_RIGHT:
            controlBuffer[7] = 1;
        break;

        }
    return 0;
}
bool SDLinput;
int handleSDLcontrol()
{
    /*
    hidScanInput();
    uint64_t kDown = hidKeysDown(CONTROLLER_P1_AUTO);
    uint64_t kUp = hidKeysUp(CONTROLLER_P1_AUTO);
    uint64_t kHeld = hidKeysHeld(CONTROLLER_P1_AUTO);
    if(kDown & KEY_X)
    {
        NESOB.closeProgram = true;
    }
    if(kDown & KEY_A)
    {
        NESOB.sp -= 3;
        NESOB.pflag = NESOB.pflag | 0x04;
        NESOB.memory[0x4015] = 0;
        NESOB.pc = NESOB.memory[0xFFFD] << 8 | NESOB.memory[0xFFFC];
    }

    controlBuffer[0] = ((kHeld & KEY_B));
    controlBuffer[1] = ((kHeld & KEY_Y));
    controlBuffer[2] = ((kHeld & KEY_MINUS));
    controlBuffer[3] = ((kHeld & KEY_PLUS));
    controlBuffer[4] = ((kHeld & KEY_DUP));
    controlBuffer[5] = ((kHeld & KEY_DDOWN));
    controlBuffer[6] = ((kHeld & KEY_DLEFT));
    controlBuffer[7] = ((kHeld & KEY_DRIGHT));
    */
    while( SDL_PollEvent( &SDL_EVENT_HANDLING)) // While Event to handle Random Stuff
    {
        if (SDL_EVENT_HANDLING.type == SDL_QUIT) // If the SDL Window is Closed, close the program.
        {
            NESOB.closeProgram = true;
        }
    }
    return 0;
}
