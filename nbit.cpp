#include "nbit.hpp"
#include <stdio.h>
unsigned long nBitClass::setBit(unsigned long value, unsigned char bit)
{
    return value |= (0x1 << bit);
}
unsigned long nBitClass::resetBit(unsigned long value, unsigned char bit)
{
    return value &= ~(0x1 << bit);
}
unsigned long nBitClass::setBitToVal(unsigned long value, bool bit, unsigned char bitNum)
{
    if(bit == true)
    {
        return setBit(value, bitNum);
    }
    if(bit == false)
    {
        return resetBit(value, bitNum);
    }
    return 0;
}
unsigned long nBitClass::toggleBit(unsigned long value, unsigned char bit)
{
    return value ^= (0x1 << bit);
}
bool nBitClass::testBit(unsigned long value, unsigned char bit)
{
    if(bit == 0)
    {
        return value & 0x1;
    }
    return (value >> bit) & 0x1;
}
unsigned long nBitClass::shiftBit(unsigned long value, unsigned char shiftAmount, bool direction)
{
    if(shiftAmount < 32)
    {
       switch(direction)
        {
            case false: // Logical Shift Left
                value = value << shiftAmount;
            break;

            case true: // Logical Shift Right
                value = value >> shiftAmount;
            break;
        }
        return value;
    }
    while(shiftAmount != 0)
    {
        switch(direction)
        {
            case false: // Logical Shift Left
                value = value << 1;
            break;

            case true: // Logical Shift Right
                value = value >> 1;
            break;
        }
        shiftAmount--;
    }
    return value;
}
unsigned long nBitClass::rotateBit(unsigned long valueNeed, unsigned char rotateTimes, bool goRight)
{
    backupBit = 0;
    if(goRight == true)
    {
        while(rotateTimes != 0x0)
        {
            backupBit = testBit(valueNeed, 0);
            valueNeed = shiftBit(valueNeed, 1, true);
            valueNeed = setBitToVal(valueNeed, backupBit, 31);
            rotateTimes--;
        }
    }
    if(goRight == false)
    {
        while(rotateTimes != 0x0)
        {
            backupBit = testBit(valueNeed, 31);
            valueNeed = valueNeed << 1;
            valueNeed = setBitToVal(valueNeed, backupBit, 0);
            rotateTimes--;
        }
    }
    return valueNeed;
}
unsigned long nBitClass::aShiftBit(unsigned long value, unsigned char bit)
{
    while(bit != 0)
    {
        signValue = testBit(value, 31);
        value = value >> 1;
        value = setBitToVal(value, signValue, 31);
        bit--;
    }
    return value;
}

