#include "include.hpp"
#ifdef __linux
#include <SDL2/SDL_mixer.h>
#endif // __linux
#ifdef _WIN32
#include <SDL_mixer.h>
#endif // _WIN32
// TODO ( This will likely never be finished. )
Uint8 sq1raw[800];
float initialfreq;
float periods;
Mix_Chunk *sq1rawchunk;
int in;
uint16_t timesq1;
uint16_t finalfreq;
uint16_t finalPeriods;
uint16_t backupPeriods;
uint16_t periodCounter;
bool periodHigh;
uint16_t periodOther;
uint16_t otherCounter;
std::bitset<11> convert11;
int handleSound()
{
    in = 0;
    convert11 = NESOB.memory[0x4003] << 8 | NESOB.memory[0x4002];
    timesq1 = convert11.to_ulong();
    //printf("timesq1: %i\n",timesq1);
    initialfreq = 1.789773 / (16 * (timesq1 + 1));
    initialfreq * 1000000;
    initialfreq = 48000 / initialfreq;
    periods = 800 / initialfreq; // Periods
    periods = 800 / periods;
    periods = periods / 2;
    finalPeriods = periods;
    backupPeriods = periods;
    periods = 800 / initialfreq;
    periodOther = periods * 2;
    periodOther++;
    Mix_Volume(-1,0x80);
    periodCounter = 0;
    finalPeriods = timesq1;
    backupPeriods = timesq1;
    //printf("finalPeriods: %i",finalPeriods);
    if(finalPeriods < 8)
    {
        return 0;
    }
    while(true)
    {
        if(periodHigh == false)
        {
            periodHigh = true;
            goto doneCalcper;
        }
        if(periodHigh == true)
        {
            periodHigh = false;
        }
        doneCalcper:

        while(periodCounter != finalPeriods)
        {
            if(periodHigh == true)
            {
                sq1raw[periodCounter] = 0xF0;
            }
            if(periodHigh == false)
            {
                sq1raw[periodCounter] = 0x10;
            }
            if(periodCounter + 1 == finalPeriods)
            {
                sq1raw[periodCounter] = 0x80;
            }
            periodCounter++;
            if(periodCounter > 799)
            {
                goto doneCalcu; // Prevents a segfault
            }
        }
        finalPeriods += backupPeriods;
        otherCounter++;
    }
    doneCalcu:
    sq1rawchunk = Mix_QuickLoad_RAW(sq1raw,800);
    Mix_PlayChannel(-1,sq1rawchunk,1);
    return 0;
}
