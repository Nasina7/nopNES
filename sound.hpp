#include "include.hpp"
#ifdef __linux
#include <SDL2/SDL_mixer.h>
#endif // __linux
#ifdef _WIN32
#include <SDL_mixer.h>
#endif // _WIN32
// TODO
Uint8 sq1raw[96000];
Uint8 sq2raw[96000];
Uint8 triraw[96000];
Uint8 silence[96000];
float initialfreq;
float periods;
bool sq1enable = true;
bool sq2enable = true;
bool trienable = true;
float divide = 2.28;
Mix_Chunk *sq1rawchunk;
Mix_Chunk *sq2rawchunk;
Mix_Chunk *trirawchunk;
int in;
uint16_t timesq1;
uint16_t finalfreq;
uint32_t finalPeriods;
uint32_t backupPeriods;
uint32_t periodCounter;
bool periodHigh;
uint16_t periodOther;
uint16_t otherCounter;
std::bitset<11> convert11;
std::bitset<8> soundBit;
uint16_t sampleTest;
uint8_t secondCount;
bool quiet;
uint8_t runCountsound;
uint8_t sq1env;
uint8_t sq1env2;
uint8_t sq1envp;
uint8_t sq1env2p;
int generateSamplesq1()
{
    finalPeriods = finalPeriods / divide;
    backupPeriods = backupPeriods / divide;
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
                sq1raw[periodCounter] = 0x40;
            }
            if(periodHigh == false)
            {
                sq1raw[periodCounter] = 0xC0;
            }
            if(periodCounter + 1 == finalPeriods)
            {
                sq1raw[periodCounter] = 0x80;
            }
            periodCounter++;
            if(periodCounter > 96000)
            {
                //printf("test\n");
                return 0;
            }
        }
        finalPeriods += backupPeriods;
        //if(finalPeriods + backupPeriods >= 800 && finalPeriods <= 800)
        //{
        //    sampleTest = finalPeriods;
            //printf("samp: %i\n",sampleTest * 60);
            //goto doneCalcu;
        //}
        otherCounter++;
    }
}
int generateSamplesq2()
{
    finalPeriods = finalPeriods / divide;
    backupPeriods = backupPeriods / divide;
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
                sq2raw[periodCounter] = 0x40;
            }
            if(periodHigh == false)
            {
                sq2raw[periodCounter] = 0xC0;
            }
            if(periodCounter + 1 == finalPeriods)
            {
                sq2raw[periodCounter] = 0x80;
            }
            periodCounter++;
            if(periodCounter > 96000)
            {
                //printf("test\n");
                return 0;
            }
        }
        finalPeriods += backupPeriods;
        //if(finalPeriods + backupPeriods >= 800 && finalPeriods <= 800)
        //{
        //    sampleTest = finalPeriods;
            //printf("samp: %i\n",sampleTest * 60);
            //goto doneCalcu;
        //}
        otherCounter++;
    }
}
uint8_t increaseTriVal;
uint8_t writeTri;
float increaseTriVal2;
int generateSampleTri()
{
    finalPeriods = finalPeriods / divide;
    backupPeriods = backupPeriods / divide;
    //printf("%i\n",finalPeriods);
    increaseTriVal2 = finalPeriods / 0x80;
    if(increaseTriVal2 <= 1)
    {
        increaseTriVal2 = 1;
    }
    writeTri = 0x40;
    while(true)
    {
        if(periodHigh == false)
        {
            periodHigh = true;
            writeTri = 0x40;
            goto doneCalcper;
        }
        if(periodHigh == true)
        {
            periodHigh = false;
            writeTri = 0xC0;
        }
        doneCalcper:

        while(periodCounter != finalPeriods)
        {
            if(periodHigh == true)
            {
                triraw[periodCounter] = writeTri;
                writeTri += increaseTriVal2;
                if(writeTri > 0xC0)
                {
                    writeTri = 0xC0;
                }
            }
            if(periodHigh == false)
            {
                triraw[periodCounter] = writeTri;
                writeTri -= increaseTriVal2;
                if(writeTri < 0x40)
                {
                    writeTri = 0x40;
                }
            }
            periodCounter++;
            if(periodCounter > 96000)
            {
                //printf("test\n");
                return 0;
            }
        }
        finalPeriods += backupPeriods;
        //if(finalPeriods + backupPeriods >= 800 && finalPeriods <= 800)
        //{
        //    sampleTest = finalPeriods;
            //printf("samp: %i\n",sampleTest * 60);
            //goto doneCalcu;
        //}
        otherCounter++;
    }
}
uint16_t timesq1back;
uint8_t channelVolume;
int handlesq1()
{
    timesq1back = timesq1;
    //Mix_CloseAudio();
    //againTest:
    quiet = false;
    in = 0;
    convert11 = 0;
    timesq1 = 0;
    periodCounter = 0;
    convert11 = NESOB.memory[0x4003] << 8 | NESOB.memory[0x4002];
    timesq1 = convert11.to_ulong();
    //NESOB.memory[0x4003] = 0;
   // NESOB.memory[0x4002] = 0;
    //timesq1 = 0x60;
   // printf("timesq1: %x\n",timesq1);
    bit4 = NESOB.memory[0x4000];
    channelVolume = bit4.to_ulong();
    Mix_Volume(0,channelVolume * 4);
   // Mix_Volume(0,0x40);
    periodCounter = 0;
    //timesq1 = 84;
    finalPeriods = timesq1;
    backupPeriods = timesq1;
    sampleTest = 800;
    //printf("finalPeriods: %i",finalPeriods);
    if(lengthCounter1 == 0)
    {
        //sq1rawchunk = Mix_QuickLoad_RAW(silence,96000);
        //Mix_PlayChannel(0,sq1rawchunk,0);
    }
    tempBitBuffer = NESOB.memory[0x4000];
    if(finalPeriods < 8 || lengthCounter1 == 0)
    {
        //Mix_Pause(0);
        if(tempBitBuffer[5] == 0)
        {
            return 0;
        }
        //SDL_Delay(17);
        //goto againTest;
    }
    if(lengthCounter1 != 0)
    {
        tempBitBuffer = NESOB.memory[0x4015];
        tempBitBuffer[0] = 1;
        NESOB.memory[0x4015] = tempBitBuffer.to_ulong();
    }
    if(lengthCounter1 == 0)
    {
        tempBitBuffer = NESOB.memory[0x4015];
        tempBitBuffer[0] = 0;
        NESOB.memory[0x4015] = tempBitBuffer.to_ulong();
    }
        if(timesq1 != timesq1back)
        {
            generateSamplesq1();
        }
        if(timesq1 != timesq1back)
        {
            sq1rawchunk = Mix_QuickLoad_RAW(sq1raw,96000);
            Mix_PlayChannel(0,sq1rawchunk,0);
        }
    return 0;
}
uint16_t timesq2back;
uint16_t timesq2;
int handlesq2()
{
    timesq2back = timesq2;
    //Mix_CloseAudio();
    //againTest:
    quiet = false;
    in = 0;
    convert11 = 0;
    timesq2 = 0;
    periodCounter = 0;
    convert11 = NESOB.memory[0x4007] << 8 | NESOB.memory[0x4006];
    timesq2 = convert11.to_ulong();
    //NESOB.memory[0x4003] = 0;
   // NESOB.memory[0x4002] = 0;
    //timesq1 = 0x60;
   // printf("timesq1: %x\n",timesq1);
    bit4 = NESOB.memory[0x4004];
    channelVolume = bit4.to_ulong();
    Mix_Volume(1,channelVolume * 4);

    periodCounter = 0;
    //timesq1 = 84;
    finalPeriods = timesq2;
    backupPeriods = timesq2;
    sampleTest = 800;
    //printf("finalPeriods: %i",finalPeriods);
    if(lengthCounter2 == 0)
    {
        //sq2rawchunk = Mix_QuickLoad_RAW(silence,96000);
        //Mix_PlayChannel(1,sq2rawchunk,0);
    }
    tempBitBuffer = NESOB.memory[0x4004];
    if(finalPeriods < 8)
    {
        //Mix_Pause(0);
        return 0;
        //SDL_Delay(17);
        //goto againTest;
    }
    if(lengthCounter2 == 0 && tempBitBuffer[5] == 0)
    {
        return 0;
    }
    if(lengthCounter2 != 0)
    {
        tempBitBuffer = NESOB.memory[0x4015];
        tempBitBuffer[1] = 1;
        NESOB.memory[0x4015] = tempBitBuffer.to_ulong();
    }
    if(lengthCounter2 == 0)
    {
        tempBitBuffer = NESOB.memory[0x4015];
        tempBitBuffer[1] = 0;
        NESOB.memory[0x4015] = tempBitBuffer.to_ulong();
    }
        if(timesq2 != timesq2back)
        {
            generateSamplesq2();
        }
        if(timesq2 != timesq2back)
        {
            sq2rawchunk = Mix_QuickLoad_RAW(sq2raw,96000);
            Mix_PlayChannel(1,sq2rawchunk,0);
        }
    return 0;
}
uint16_t timetriback;
uint16_t timetri;
int handleTri()
{
    timetriback = timetri;
    //Mix_CloseAudio();
    //againTest:
    quiet = false;
    in = 0;
    convert11 = 0;
    periodCounter = 0;
    convert11 = NESOB.memory[0x400B] << 8 | NESOB.memory[0x400A];
    timetri = convert11.to_ulong();
    //NESOB.memory[0x4003] = 0;
   // NESOB.memory[0x4002] = 0;
    //timesq1 = 0x60;
   // printf("timesq1: %x\n",timesq1);
    Mix_Volume(2,0x30);

    periodCounter = 0;
    //timesq1 = 84;
    finalPeriods = timetri;
    backupPeriods = timetri;
    sampleTest = 800;
    //printf("finalPeriods: %i",finalPeriods);
    //if(lengthCounter2 == 0)
    //{
        //sq2rawchunk = Mix_QuickLoad_RAW(silence,96000);
        //Mix_PlayChannel(1,sq2rawchunk,0);
    //}
    if(finalPeriods < 8)
    {
        //Mix_Pause(0);
        return 0;
        //SDL_Delay(17);
        //goto againTest;
    }
    tempBitBuffer = NESOB.memory[0x4008];
    if(lengthCounter3 == 0 && tempBitBuffer[7] == 0)
    {
        return 0;
    }
    if(lengthCounter3 != 0)
    {
        tempBitBuffer = NESOB.memory[0x4015];
        tempBitBuffer[1] = 1;
        NESOB.memory[0x4015] = tempBitBuffer.to_ulong();
    }
    if(lengthCounter3 == 0)
    {
        tempBitBuffer = NESOB.memory[0x4015];
        tempBitBuffer[1] = 0;
        NESOB.memory[0x4015] = tempBitBuffer.to_ulong();
    }
        if(timetri != timetriback)
        {
            generateSampleTri();
        }
        if(timetri != timetriback)
        {
            trirawchunk = Mix_QuickLoad_RAW(triraw,96000);
            Mix_PlayChannel(2,trirawchunk,0);
        }
    return 0;
}
int handleSound()
{
    if(sq1enable == true)
    {
        handlesq1();
    }
    if(sq2enable == true)
    {
        handlesq2();
    }
    if(trienable == true)
    {
        handleTri();
    }
    //SDL_Delay(17);
    //handleSound();
    return 0;
}
int handleSoundTimers()
{
    std::bitset<4> bit42;
    uint8_t le;
    uint8_t le2;
    uint8_t le3;
    uint8_t env1;
    tempBitBuffer = NESOB.memory[0x4000];
    tempBitBuffer2 = NESOB.memory[0x4004];
    tempBitBuffer3 = NESOB.memory[0x4008];
    le = lengthCounter1;
    le2 = lengthCounter2;
    le3 = lengthCounter3;
    if(lengthCounter1 == 1 && tempBitBuffer[5] == 0 && volumeConstsq1 == true)
    {
        lengthCounter1 = 2;
    }
    if(lengthCounter1 != 0 && tempBitBuffer[5] == 0 && volumeConstsq1 == true)
    {
        lengthCounter1 -= 2;
    }
    if(tempBitBuffer[5] == 1)
    {
        lengthCounter1 = le;
    }
    if(lengthCounter2 == 1 && tempBitBuffer2[5] == 0 && volumeConstsq2 == true)
    {
        lengthCounter2 = 2;
    }
    if(lengthCounter2 != 0 && tempBitBuffer2[5] == 0 && volumeConstsq2 == true)
    {
        lengthCounter2 -= 2;
    }
    if(tempBitBuffer2[5] == 1)
    {
        lengthCounter2 = le2;
    }
    if(lengthCounter3 == 1 && tempBitBuffer3[7] == 0)
    {
        lengthCounter3 = 2;
    }
    if(lengthCounter3 != 0 && tempBitBuffer3[7] == 0)
    {
        lengthCounter3 -= 2;
    }
    if(tempBitBuffer3[5] == 1)
    {
        lengthCounter3 = le3;
    }
    return 0;
    if(tempBitBuffer[4] == 0) // If SQ1 Envelope is enabled
    {
        bit42 = NESOB.memory[0x4000];
        if(bit42.to_ulong() == 0)
        {
            if(tempBitBuffer[5] == 1)
            {
                bit42 = 15;
            }
            if(tempBitBuffer[5] == 0)
            {
                bit42 = 0;
            }
            tempBitBuffer = NESOB.memory[0x4000];
            tempBitBuffer[0] = bit42[0];
            tempBitBuffer[1] = bit42[1];
            tempBitBuffer[2] = bit42[2];
            tempBitBuffer[3] = bit42[3];
            NESOB.memory[0x4000] = tempBitBuffer.to_ulong();
            goto doneEnv1;
        }
        if(bit42.to_ulong() != 0)
        {
            env1 = bit42.to_ulong();
            if(env1 < env1 - 4)
            {
                if(tempBitBuffer[5] == 1)
                {
                    bit42 = 15;
                }
                if(tempBitBuffer[5] == 0)
                {
                    bit42 = 0;
                }
                tempBitBuffer = NESOB.memory[0x4000];
                tempBitBuffer[0] = bit42[0];
                tempBitBuffer[1] = bit42[1];
                tempBitBuffer[2] = bit42[2];
                tempBitBuffer[3] = bit42[3];
                NESOB.memory[0x4000] = tempBitBuffer.to_ulong();
                goto doneEnv1;
            }
            env1 -= 4;
            bit42 = env1;
            tempBitBuffer = NESOB.memory[0x4000];
            tempBitBuffer[0] = bit42[0];
            tempBitBuffer[1] = bit42[1];
            tempBitBuffer[2] = bit42[2];
            tempBitBuffer[3] = bit42[3];
            NESOB.memory[0x4000] = tempBitBuffer.to_ulong();
        }
    }
    doneEnv1:
    //cout<<"s1: "<<sq1env<<endl;
    //cout<<"s2: "<<sq1env2<<endl;
    dummyval++;
}
