#include "graphics.hpp"
uint8_t registerArray[13];
FILE* savestate = NULL;
FILE* saveload = NULL;
void saveNESstate()
{
            if(savestatename[0] == NULL)
            {
                cout<<"Type name for save state."<<endl;
                cin>>savestatename;
            }
            savestate = fopen(savestatename,"w+");
            fwrite(NESOB.memory,sizeof(uint8_t),0xFFFF,savestate);
            //fwrite(NESOB.PPUmemory,sizeof(uint8_t),0x4000,savestate);
            //fwrite(OAMmem,sizeof(uint8_t),0x100,savestate);
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
            fwrite(registerArray,sizeof(uint8_t),13,savestate);
            fclose(savestate);
            cout<<"State Saved!"<<endl;
            //breakpoint = true;
}
void loadNESstate()
{
            if(savestatename[0] == NULL)
            {
                cout<<"Type name for save state."<<endl;
                cin>>savestatename;
            }
            saveload = fopen(savestatename,"w+");
            fread(NESOB.memory,sizeof(uint8_t),0xFFFF,saveload);
            //fread(NESOB.PPUmemory,sizeof(uint8_t),0x4000,saveload);
            //fread(OAMmem,sizeof(uint8_t),0x100,saveload);
            fread(registerArray,sizeof(uint8_t),13,saveload);
            fclose(saveload);
            NESOB.pc = registerArray[0] << 8 | registerArray[1];
            NESOB.sp = registerArray[2];
            NESOB.a = registerArray[3];
            NESOB.x = registerArray[4];
            NESOB.y = registerArray[5];
            NESOB.pflag = registerArray[6];
            NESOB.cycles = registerArray[7] << 24 | registerArray[8] << 16 | registerArray[9] << 8 | registerArray[10];
            NESOB.scanline = registerArray[11] << 8 | registerArray[12];
            cout<<"State Loaded!"<<endl;
            fclose(saveload);
}
int handleControlsr()
{
    switch( SDL_EVENT_HANDLING.key.keysym.sym )
        {

        case SDLK_t:
            handleCorruptorKey();
        break;

        case SDLK_8:
            Mix_Pause(0);
            bit4 = sq1enable;
            bit4.flip(0);
            sq1enable = bit4.to_ulong();
        break;

        case SDLK_9:
            Mix_Pause(1);
            bit4 = sq2enable;
            bit4.flip(0);
            sq2enable = bit4.to_ulong();
        break;

        case SDLK_0:
            Mix_Pause(2);
            bit4 = trienable;
            bit4.flip(0);
            trienable = bit4.to_ulong();
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
        break;

        case SDLK_b:
            cout<<blitsu<<endl;
            cin>>blitsu;
        break;

        case SDLK_e:
            if(enableSound == true)
            {
                enableSound = false;
                cout<<"Sound is disabled!  Press E to enable."<<endl;
                break;
            }
            if(enableSound == false)
            {
                enableSound = true;
                cout<<"Sound is enabled!  Press E to disable."<<endl;
                break;
            }
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
    while( SDL_PollEvent( &SDL_EVENT_HANDLING)) // While Event to handle Random Stuff
        {
            if (SDL_EVENT_HANDLING.type == SDL_QUIT) // If the SDL Window is Closed, close the program.
            {
                NESOB.closeProgram = true;
            }
            if (SDL_EVENT_HANDLING.type == SDL_KEYDOWN) // If a key is being pressed, handle controls.
            {   // Handle Controls
                handleControls();
            }
            if (SDL_EVENT_HANDLING.type == SDL_KEYUP)
            {
                handleControlsr();
            }
        }
    return 0;
}
