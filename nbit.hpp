/*
Nasina's Bit Manipulation Library.
v1.0.1
*/
class nBitClass
{
    public:
        unsigned long setBit(unsigned long value, unsigned char bit);
        unsigned long resetBit(unsigned long value, unsigned char bit);
        unsigned long setBitToVal(unsigned long value, bool bit, unsigned char bitNum);
        unsigned long toggleBit(unsigned long value, unsigned char bit);
        bool testBit(unsigned long value, unsigned char bit);
        unsigned long shiftBit(unsigned long value, unsigned char shiftAmount, bool direction);
        unsigned long rotateBit(unsigned long valueNeed, unsigned char rotateTimes, bool goRight);
        unsigned long aShiftBit(unsigned long value, unsigned char bit);
    private:
        bool backupBit;
        bool signValue;
        unsigned long testBitLookupTable[32];
};
