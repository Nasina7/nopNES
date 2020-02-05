#include "graphics.hpp"
int handleControlsr()
{
    switch( SDL_EVENT_HANDLING.key.keysym.sym )
        {

        case SDLK_3:
            cin>>blitsu;
        break;

        case SDLK_INSERT:
            printf("Change Cycle Modulo too: ");
            cin>>cycleModulo;
        break;

        case SDLK_LEFTBRACKET:
            showTile = false;
        break;

        case SDLK_RIGHTBRACKET:
            showBlock = false;
        break;

        case SDLK_BACKSLASH:
            showGrid = false;
        break;

        case SDLK_b:
            fpsBenchmark();
        break;

        case SDLK_r:
            NESOB.pc = NESOB.memory[0xFFFD] << 8 | NESOB.memory[0xFFFC]; // Sets Initial PC Value
            NESOB.pflag = 0x34;
            NESOB.sp = 0xFD;
            NESOB.a = 0x00;
            NESOB.x = 0x00;
            NESOB.y = 0x00;
            currentFrame = 0;
        break;

        case SDLK_1:
            NESOB.pc = 0xC000; // Sets Initial PC Value
            NESOB.pflag = 0x34;
            NESOB.sp = 0xFD;
            NESOB.a = 0x00;
            NESOB.x = 0x00;
            NESOB.y = 0x00;
            currentFrame = 0;
        break;

        case SDLK_0:
            if(beginning() == false)
            {
                printf("File Not Found!\n");
                return 0;
            }
            NESOB.pc = NESOB.memory[0xFFFD] << 8 | NESOB.memory[0xFFFC]; // Sets Initial PC Value
            NESOB.pflag = 0x34;
            NESOB.sp = 0xFD;
            NESOB.a = 0x00;
            NESOB.x = 0x00;
            NESOB.y = 0x00;
            NESOB.scanline = 0;
            pal = fopen("pal.pal", "rb");
            fread(pallete,192,1,pal);
            fclose(pal);
            currentFrame = 0;
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
            printf("Return!");
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
    #ifdef __linux__
    while( SDL_PollEvent( &SDL_EVENT_HANDLING)) // While Event to handle Random Stuff
        {
            //printf("event?\n");
            if (SDL_EVENT_HANDLING.type == SDL_QUIT) // If the SDL Window is Closed, close the program.
            {
                NESOB.closeProgram = true;
            }
            if (SDL_EVENT_HANDLING.type == SDL_KEYDOWN) // If a key is being pressed, handle controls.
            {   // Handle Controls
                //printf("KEYDOWN");
                handleControls();
            }
            if (SDL_EVENT_HANDLING.type == SDL_KEYUP)
            {
                handleControlsr();
            }
        }
    #endif // __linux__
    #ifdef _WIN32
        SDL_PollEvent( &SDL_EVENT_HANDLING);
        if (SDL_EVENT_HANDLING.type == SDL_QUIT) // If the SDL Window is Closed, close the program.
            {
                NESOB.closeProgram = true;
            }
            if (SDL_EVENT_HANDLING.type == SDL_KEYDOWN) // If a key is being pressed, handle controls.
            {   // Handle Controls
                printf("KEYDOWN");
                handleControls();
            }
            if (SDL_EVENT_HANDLING.type == SDL_KEYUP)
            {
                handleControlsr();
            }
    #endif
    handleSDLcontrol();
}
