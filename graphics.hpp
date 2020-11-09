#include "mappers.hpp"
uint16_t notificationTimer;
SDL_Rect notification[2];
SDL_Texture *notiText;
uint8_t bitcount;
SDL_Texture* texture = NULL;
SDL_Texture* texture2400 = NULL;
SDL_Texture* textureScanline = NULL;
SDL_Texture* textureScanlineF = NULL;
SDL_Texture* textureScanline28 = NULL;
SDL_Rect mainScreen;
std::bitset<8> graphicline;
std::bitset<8> graphicline2;
std::bitset<8> graphiclineS;
std::bitset<8> graphicline2S;
std::bitset<8> spriteAtt;
int blitsu = 2048;
uint16_t nametableAddr2;
uint8_t currentnametable;
uint8_t tilecounte;
uint8_t currentblockx;
uint8_t currentblocky;
uint8_t currentgridx;
uint8_t currentgridy;
uint8_t currentBlock;
uint8_t currentGrid;
uint16_t tilecount;
uint8_t xpixtile;
uint8_t ypixtile;
uint8_t xpixtile2;
uint8_t ypixtile2;
uint16_t xpix;
uint16_t ypix;
uint8_t tilelocation;
uint16_t tiledatalocation1;
uint8_t pal00[4] = {0xFF,0xFF,0xFF,0x00};
uint8_t pal01[4] = {0xC2,0xC2,0xC2,0x00};
uint8_t pal10[4] = {0x76,0x76,0x76,0x00};
uint8_t pal11[4] = {0x00,0x00,0x00,0x00};
uint8_t pal[16];
uint8_t pal00S[4] = {0xFF,0xFF,0xFF,0x00};
uint8_t pal01S[4] = {0xC2,0xC2,0xC2,0x00};
uint8_t pal10S[4] = {0x76,0x76,0x76,0x00};
uint8_t pal11S[4] = {0x00,0x00,0x00,0x00};
SDL_Color color_00;
SDL_Color color_01;
SDL_Color color_02;
SDL_Color color_03;
uint8_t pallete[192];
uint8_t bgcol;
uint8_t bgcolS;
uint8_t tilecounteback;
uint8_t Xcounter;
uint8_t Ycounter;
SDL_Color greyM = {0x80,0x80,0x80,0x80};
const char *messageBuf;
void displayMessagebox(std::string line1, uint16_t amountOfFrameDisplay)
{

}
void handleGrid()
{
    currentgridx = xpixtile / 4;
    currentgridy = ypixtile / 4;
    currentGrid = (currentgridx * 1) + (currentgridy * 8);
    //printf("curgri: %i\n",currentGrid);
}
uint8_t handleblockx;
uint8_t handleblocky;
uint8_t previblock;
void handleBlock()
{
    //printf("xpixtile: %i\n",xpixtile);
    previblock = currentBlock;
    handleblockx = xpixtile / 2;
    handleblocky = ypixtile / 2;
    currentblockx = (xpixtile) % 2;
    currentblocky = (ypixtile) % 2;
    //currentblockx = currentblockx % 2;
    //currentblocky = currentblocky % 2;
    if(currentblockx == 0 && currentblocky == 0)
    {
        currentBlock = 0;
    }
    if(currentblockx == 1 && currentblocky == 0)
    {
        currentBlock = 2;
    }
    if(currentblockx == 0 && currentblocky == 1)
    {
        currentBlock = 4;
    }
    if(currentblockx == 1 && currentblocky == 1)
    {
        currentBlock = 6;
    }
    if(currentBlock != previblock)
    {
        //printf("curblo: %i\n",currentBlock);
    }

}
uint16_t palLocate;
std::bitset<8> currentGridat;
std::bitset<2> currentBlockat;
void palDetermine()
{
    if(nametableuse == 0)
    {
        currentGridat = NESOB.PPUmemory[(0x23C0) + currentGrid];
    }
    if(nametableuse == 1)
    {
        currentGridat = NESOB.PPUmemory[(0x27C0) + currentGrid];
    }
    currentBlockat[0] = currentGridat[currentBlock + 1];
    currentBlockat[1] = currentGridat[currentBlock];
    if(currentBlockat[1] == 0 && currentBlockat[0] == 0)
    {
        palLocate = 0x3F01;
    }
    if(currentBlockat[1] == 0 && currentBlockat[0] == 1)
    {
        palLocate = 0x3F05;
    }
    if(currentBlockat[1] == 1 && currentBlockat[0] == 0)
    {
        palLocate = 0x3F09;
    }
    if(currentBlockat[1] == 1 && currentBlockat[0] == 1)
    {
        palLocate = 0x3F0D;
    }
    bgcol = NESOB.PPUmemory[0x3F00];
    bgcol = bgcol * 3;
    pal00[0] = pallete[bgcol];
    pal00[1] = pallete[bgcol + 1];
    pal00[2] = pallete[bgcol + 2];
    pal00[3] = 0xFF;
    bgcol = NESOB.PPUmemory[palLocate];
    bgcol = bgcol * 3;
    pal01[0] = pallete[bgcol];
    pal01[1] = pallete[bgcol + 1];
    pal01[2] = pallete[bgcol + 2];
    pal01[3] = 0xFF;
    bgcol = NESOB.PPUmemory[palLocate + 1];
    bgcol = bgcol * 3;
    pal10[0] = pallete[bgcol];
    pal10[1] = pallete[bgcol + 1];
    pal10[2] = pallete[bgcol + 2];
    pal10[3] = 0xFF;
    bgcol = NESOB.PPUmemory[palLocate + 2];
    bgcol = bgcol * 3;
    pal11[0] = pallete[bgcol];
    pal11[1] = pallete[bgcol + 1];
    pal11[2] = pallete[bgcol + 2];
    pal11[3] = 0xFF;
    color_00 = {pal00[0],pal00[1],pal00[2],pal00[3]};
    color_01 = {pal01[0],pal01[1],pal01[2],pal01[3]};
    color_02 = {pal10[0],pal10[1],pal10[2],pal10[3]};
    color_03 = {pal11[0],pal11[1],pal11[2],pal11[3]};
}
uint8_t prevBlock;
uint16_t palLocLookup[4] = {0x3F01, 0x3F05, 0x3F09, 0x3F0D};
uint8_t testFix;
void palDetermineREWRITE()
{
    testFix = NESOB.PPUmemory[(nametableAddr2 + 0x3C0) + currentGrid];

    //palLocate = palLocLookup[(currentGridat[currentBlock + 1] * 2) + currentGridat[currentBlock]];

    palLocate = palLocLookup[(nBit.testBit(
                                           testFix,
                                           currentBlock + 1) * 2) +
                              nBit.testBit(
                                           testFix,
                                           currentBlock)];

    bgcol = NESOB.PPUmemory[0x3F00] * 3;
    pal[0] = pallete[bgcol];
    pal[1] = pallete[bgcol + 1];
    pal[2] = pallete[bgcol + 2];
    bgcol = NESOB.PPUmemory[palLocate] * 3;
    pal[4] = pallete[bgcol];
    pal[5] = pallete[bgcol + 1];
    pal[6] = pallete[bgcol + 2];
    bgcol = NESOB.PPUmemory[palLocate + 1] * 3;
    pal[8] = pallete[bgcol];
    pal[9] = pallete[bgcol + 1];
    pal[10] = pallete[bgcol + 2];
    bgcol = NESOB.PPUmemory[palLocate + 2] * 3;
    pal[12] = pallete[bgcol];
    pal[13] = pallete[bgcol + 1];
    pal[14] = pallete[bgcol + 2];
}
uint8_t palResult;
void chooseRenderColor()
{
    prevBlock = currentBlock;
    handleGrid();
    handleBlock();
    if(prevBlock != currentBlock)
    {
        palDetermineREWRITE();
    }
    if(graphicline[bitcount] == 0 && graphicline2[bitcount] == 0)
    {
        palResult = 0;
    }
    if(graphicline[bitcount] == 1 && graphicline2[bitcount] == 0)
    {
        palResult = 1;
    }
    if(graphicline[bitcount] == 0 && graphicline2[bitcount] == 1)
    {
        palResult = 2;
    }
    if(graphicline[bitcount] == 1 && graphicline2[bitcount] == 1)
    {
        palResult = 3;
    }
}
uint8_t xpixti;
uint8_t ypixti;
uint8_t curBlolookup[4] = {0,2,4,6};
uint8_t palBlolookup[4] = {0,1,2,3};
uint8_t currentblockx2;
uint8_t currentblocky2;
void chooseRenderColorREWRITE()
{
    //handleBlock();
    prevBlock = currentBlock;
    xpixti = xpix / 8;
    ypixti = ypix / 8;
    currentblockx = xpix / 16;
    currentblocky = ypix / 16;
    currentgridx = xpix / 32;
    currentgridy = ypix / 32;
    currentGrid = currentgridx + currentgridy * 8;
    currentblockx2 = currentblockx % 2;
    currentblocky2 = currentblocky % 2;
    currentBlock = curBlolookup[(currentblocky2 * 2) + currentblockx2];
    if(prevBlock != currentBlock)
    {
        palDetermineREWRITE();
    }
    //palResult = palBlolookup[(graphicline2[bitcount] * 2) + graphicline[bitcount]];
}
bool tempfixrender;
uint8_t ypix2;
uint16_t palLocateS;
std::bitset<8> OAMbitbuffer;
bool dontRenderSprite;
uint16_t palLocateLook[4] = {0x3F11, 0x3F15, 0x3F19, 0x3F1D};
uint8_t bgSto;
uint32_t bgPal;
void getBGcolor()
{
    bgSto = NESOB.PPUmemory[0x3F00];
    bgPal = pallete[bgSto * 3] << 16 | pallete[(bgSto * 3) + 1] << 8 | pallete[(bgSto * 3) + 2];
    //printf("bgpal 0x%X\n",bgPal);
    //printf("bgSto 0x%X\n",bgSto);
}
uint8_t sprResult;
uint8_t graphicLiness;
uint8_t graphicLiness2;
void chooseRenderColorSprite(uint8_t spritePal)
{
    palLocateS = palLocateLook[
                            (nBit.testBit(spritePal, 1) * 2) +
                             nBit.testBit(spritePal, 0)];
    bgcolS = NESOB.PPUmemory[0x3F10];
    bgcolS = bgcolS * 3;
    pal00S[0] = pallete[bgcolS];
    pal00S[1] = pallete[bgcolS + 1];
    pal00S[2] = pallete[bgcolS + 2];
    pal00S[3] = 0xFF;
    bgcolS = NESOB.PPUmemory[palLocateS];
    bgcolS = bgcolS * 3;
    pal01S[0] = pallete[bgcolS];
    pal01S[1] = pallete[bgcolS + 1];
    pal01S[2] = pallete[bgcolS + 2];
    pal01S[3] = 0xFF;
    bgcolS = NESOB.PPUmemory[palLocateS + 1];
    bgcolS = bgcolS * 3;
    pal10S[0] = pallete[bgcolS];
    pal10S[1] = pallete[bgcolS + 1];
    pal10S[2] = pallete[bgcolS + 2];
    pal10S[3] = 0xFF;
    bgcolS = NESOB.PPUmemory[palLocateS + 2];
    bgcolS = bgcolS * 3;
    pal11S[0] = pallete[bgcolS];
    pal11S[1] = pallete[bgcolS + 1];
    pal11S[2] = pallete[bgcolS + 2];
    pal11S[3] = 0xFF;
    color_00 = {pal00S[0],pal00S[1],pal00S[2],pal00S[3]};
    color_01 = {pal01S[0],pal01S[1],pal01S[2],pal01S[3]};
    color_02 = {pal10S[0],pal10S[1],pal10S[2],pal10S[3]};
    color_03 = {pal11S[0],pal11S[1],pal11S[2],pal11S[3]};
    if(nBit.testBit(graphicLiness, bitcountS) == 0 && nBit.testBit(graphicLiness2, bitcountS) == 0)
    {
        SDL_SetRenderDrawColor(renderer,color_00.r,color_00.g,color_00.b,color_00.a);
        dontRenderSprite = true;
    }
    if(nBit.testBit(graphicLiness, bitcountS) == 1 && nBit.testBit(graphicLiness2, bitcountS) == 0)
    {
        SDL_SetRenderDrawColor(renderer,color_01.r,color_01.g,color_01.b,color_01.a);
    }
    if(nBit.testBit(graphicLiness, bitcountS) == 0 && nBit.testBit(graphicLiness2, bitcountS) == 1)
    {
        SDL_SetRenderDrawColor(renderer,color_02.r,color_02.g,color_02.b,color_02.a);
    }
    if(nBit.testBit(graphicLiness, bitcountS) == 1 && nBit.testBit(graphicLiness2, bitcountS) == 1)
    {
        SDL_SetRenderDrawColor(renderer,color_03.r,color_03.g,color_03.b,color_03.a);
    }
    sprResult = (nBit.testBit(graphicLiness2, bitcountS) * 2) + nBit.testBit(graphicLiness, bitcountS);
}
uint32_t curPixx;
uint32_t curPixy;
uint32_t pixels[512 * 240];
uint32_t pixelsOptimize[240][512];
uint32_t pixelsFixTest[240][512];
uint32_t pixelsScanF[240][256];
uint32_t pixelsRe[240][512];
uint32_t pixelsRe28[480][256];
uint32_t pixelsScan28[256 * 480];
uint32_t pixels2400[256 * 256];
std::bitset<8> flipBitBuffer;
int8_t helpXflip;
uint8_t extramanipX;
uint8_t help1;
uint8_t help2;
uint8_t spriteXflip;
uint8_t spriteAttributes;
uint8_t spriteXpos;
uint8_t ypixS = 0;
uint8_t xpixS = 0;
uint16_t currentSprite = 0;
uint8_t spriteYpos;
uint16_t spritePatIndex;
uint8_t minus16val;
bool dontbreaksp0;
int renderPixS()
{
    if(dontRenderSprite == true)
    {
        return 0;
    }
    if(pixels[ (ypixS * 512) + xpixS ] != bgPal || pixelsScan28[ (ypixS * 256) + xpixS ] != bgPal)
    {
        //SDL_RenderDrawPoint(renderer,xpixS,ypixS);
        if(sprite0 == true)
        {
            sp0scan = ypixS;
            sprite0 = false;
        }
        //sprite0 = false;
        //return 0;
    }
    if(spriteAtt[5] == 0)
    {
        //printf("tesst\n");
        SDL_RenderDrawPoint(renderer,xpixS,ypixS); // Middle
        return 0;
    }
    if(pixels[ (ypixS * 512) + xpixS ] == bgPal)
    {
        //printf("tesst\n");
        //printf("BGPAL %X\n",bgPal);
        //printf("pixels %X\n",pixels[ (ypixS * 512) + xpixS ]);
        SDL_RenderDrawPoint(renderer,xpixS,ypixS);
        //sprite0 = false;
        return 0;
    }
    //SDL_RenderDrawPoint(renderer,xpixS,ypixS);
    return 0;
}
bool have16;
bool dontTwi;

int renderSprite()
{
        while(ypixS != help2)
        {
            //graphiclineS = NESOB.PPUmemory[spritePatIndex];
            //graphicline2S = NESOB.PPUmemory[spritePatIndex + 8];
            graphicLiness = NESOB.PPUmemory[spritePatIndex];
            graphicLiness2 = NESOB.PPUmemory[spritePatIndex + 8];

            while(xpixS != help1)
            {
                chooseRenderColorSprite(spriteAttributes);
                //SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0x00);
                renderPixS();
                xpixS++;
                bitcountS--;
                dontRenderSprite = false;
                //printf("xpix:0x%X\n", xpixS);
                //printf("sprXpos:0x%X\n", spriteXpos + 8);
            }
            helpXflip = 8;
            ypixS += 1;
            xpixS = spriteXpos;
            //spriteXflip = xpixS + 7;
            bitcountS = 7;
            spritePatIndex++;
            //printf("curSpr:0x%X\n", currentSprite);
        }
        if(spriteSize == 1 && dontTwi == false)
        {
            spritePatIndex += 8;
            help2 += 8;
            help1 = spriteXpos + 8;
            dontTwi = true;
            renderSprite();
        }
        return 0;
}
int renderHFLIPSprite()
{
        help1 = spriteXpos - 1;
        xpixS = spriteXpos;
        xpixS += 7;
        while(ypixS != help2)
        {
            graphicLiness = NESOB.PPUmemory[spritePatIndex];
            graphicLiness2 = NESOB.PPUmemory[spritePatIndex + 8];

            while(xpixS != help1)
            {
                chooseRenderColorSprite(spriteAttributes);
                //SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0x00);
                renderPixS();
                xpixS--;
                bitcountS--;
                dontRenderSprite = false;
                //printf("xpix:0x%X\n", xpixS);
                //printf("sprXpos:0x%X\n", spriteXpos + 8);
            }
            helpXflip = 8;
            ypixS += 1;
            xpixS = spriteXpos;
            xpixS += 7;
            //spriteXflip = xpixS + 7;
            bitcountS = 7;
            spritePatIndex++;
            //printf("curSpr:0x%X\n", currentSprite);
        }
        if(spriteSize == 1 && dontTwi == false)
        {
            dontTwi = true;
            spritePatIndex += 8;
            help2 += 8;
            xpixS = spriteXpos + 8;
            help1 = spriteXpos;
            renderHFLIPSprite();
        }
        return 0;
}
int renderVFLIPSprite()
{
        spritePatIndex += 7;
        while(ypixS != help2)
        {
            graphicLiness = NESOB.PPUmemory[spritePatIndex];
            graphicLiness2 = NESOB.PPUmemory[spritePatIndex + 8];

            while(xpixS != help1)
            {
                chooseRenderColorSprite(spriteAttributes);
                //SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0xFF, 0x00);
                renderPixS();
                xpixS++;
                bitcountS--;
                dontRenderSprite = false;
                //printf("xpix:0x%X\n", xpixS);
                //printf("sprXpos:0x%X\n", spriteXpos + 8);
            }
            helpXflip = 8;
            ypixS++;
            xpixS = spriteXpos;
            //spriteXflip = xpixS + 7;
            bitcountS = 7;
            spritePatIndex--;
            //printf("curSpr:0x%X\n", currentSprite);
        }
        return 0;
}
int renderVSprite16()
{
        while(ypixS != help2)
        {
            graphicLiness = NESOB.PPUmemory[spritePatIndex];
            graphicLiness2 = NESOB.PPUmemory[spritePatIndex + 8];

            while(xpixS != help1)
            {
                chooseRenderColorSprite(spriteAttributes);
                //SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0x00);
                renderPixS();
                xpixS++;
                bitcountS--;
                dontRenderSprite = false;
                //printf("xpix:0x%X\n", xpixS);
                //printf("sprXpos:0x%X\n", spriteXpos + 8);
            }
            helpXflip = 8;
            ypixS -= 1;
            xpixS = spriteXpos;
            //spriteXflip = xpixS + 7;
            bitcountS = 7;
            spritePatIndex++;
            //printf("curSpr:0x%X\n", currentSprite);
        }
        if(spriteSize == 1 && dontTwi == false)
        {
            spritePatIndex += 8;
            help2 -= 8;
            help1 = spriteXpos + 8;
            dontTwi = true;
            renderVSprite16();
        }
        return 0;
}
int renderHVFLIPSprite()
{
        help1 = spriteXpos - 1;
        xpixS = spriteXpos;
        xpixS += 7;
        spritePatIndex += 7;
        while(ypixS != help2)
        {
            graphicLiness = NESOB.PPUmemory[spritePatIndex];
            graphicLiness2 = NESOB.PPUmemory[spritePatIndex + 8];

            while(xpixS != help1)
            {
                chooseRenderColorSprite(spriteAttributes);
                //SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);
                renderPixS();
                xpixS--;
                bitcountS--;
                dontRenderSprite = false;
                //printf("xpix:0x%X\n", xpixS);
                //printf("sprXpos:0x%X\n", spriteXpos + 8);
            }
            helpXflip = 8;
            ypixS++;
            xpixS = spriteXpos;
            xpixS += 7;
            //spriteXflip = xpixS + 7;
            bitcountS = 7;
            spritePatIndex--;
            //printf("curSpr:0x%X\n", currentSprite);
        }
        return 0;
}
int renderHVFLIPSprite16()
{
        help1 = spriteXpos - 1;
        xpixS = spriteXpos;
        xpixS += 7;
        while(ypixS != help2)
        {
            graphicLiness = NESOB.PPUmemory[spritePatIndex];
            graphicLiness2 = NESOB.PPUmemory[spritePatIndex + 8];

            while(xpixS != help1)
            {
                chooseRenderColorSprite(spriteAttributes);
                //SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0x00);
                renderPixS();
                xpixS--;
                bitcountS--;
                dontRenderSprite = false;
                //printf("xpix:0x%X\n", xpixS);
                //printf("sprXpos:0x%X\n", spriteXpos + 8);
            }
            helpXflip = 8;
            ypixS -= 1;
            xpixS = spriteXpos;
            xpixS += 7;
            //spriteXflip = xpixS + 7;
            bitcountS = 7;
            spritePatIndex++;
            //printf("curSpr:0x%X\n", currentSprite);
        }
        if(spriteSize == 1 && dontTwi == false)
        {
            dontTwi = true;
            spritePatIndex += 8;
            help2 -= 8;
            xpixS = spriteXpos + 8;
            help1 = spriteXpos;
            renderHVFLIPSprite16();
        }
        return 0;
}
void horizontalmirror()
{
    if(nametableuse == 0 || nametableuse == 2)
    {
        mainScreen.w = 256;
        mainScreen.h = 240;
        mainScreen.x = 0 - scrollx;
        mainScreen.y = 0 - scrolly;
        SDL_RenderSetViewport(renderer, &mainScreen);
        SDL_UpdateTexture(texture, NULL, pixels, blitsu);
        SDL_RenderCopy(renderer, texture, NULL, NULL);

        mainScreen.x = 256 - scrollx;
        mainScreen.y = 0 - scrolly;
        SDL_RenderSetViewport(renderer, &mainScreen);
        SDL_UpdateTexture(texture2400, NULL, pixels2400, blitsu);
        SDL_RenderCopy(renderer, texture2400, NULL, NULL);

        mainScreen.x = 0 - scrollx;
        mainScreen.y = 240 - scrolly;
        SDL_RenderSetViewport(renderer, &mainScreen);
        SDL_UpdateTexture(texture, NULL, pixels, blitsu);
        SDL_RenderCopy(renderer, texture, NULL, NULL);

        mainScreen.x = 256 - scrollx;
        mainScreen.y = 240 - scrolly;
        SDL_RenderSetViewport(renderer, &mainScreen);
        SDL_UpdateTexture(texture2400, NULL, pixels2400, blitsu);
        SDL_RenderCopy(renderer, texture2400, NULL, NULL);

    }
    //printf("Scrollx: 0x%X\n",scrollx);
    //printf("Scrolly: 0x%X\n",scrolly);
    //printf("nameuse: 0x%X\n",nametableuse);
    if(nametableuse == 1 || nametableuse == 3)
    {
        mainScreen.w = 256;
        mainScreen.h = 240;
        mainScreen.x = 256 - scrollx;
        mainScreen.y = 0 - scrolly;
        SDL_RenderSetViewport(renderer, &mainScreen);
        SDL_UpdateTexture(texture, NULL, pixels, blitsu);
        SDL_RenderCopy(renderer, texture, NULL, NULL);

        mainScreen.x = 0 - scrollx;
        mainScreen.y = 0 - scrolly;
        SDL_RenderSetViewport(renderer, &mainScreen);
        SDL_UpdateTexture(texture2400, NULL, pixels2400, blitsu);
        SDL_RenderCopy(renderer, texture2400, NULL, NULL);

        mainScreen.x = 256 - scrollx;
        mainScreen.y = 240 - scrolly;
        SDL_RenderSetViewport(renderer, &mainScreen);
        SDL_UpdateTexture(texture, NULL, pixels, blitsu);
        SDL_RenderCopy(renderer, texture, NULL, NULL);

        mainScreen.x = 0 - scrollx;
        mainScreen.y = 240 - scrolly;
        SDL_RenderSetViewport(renderer, &mainScreen);
        SDL_UpdateTexture(texture2400, NULL, pixels2400, blitsu);
        SDL_RenderCopy(renderer, texture2400, NULL, NULL);
    }
}
void verticalmirror()
{
    if(nametableuse == 0 || nametableuse == 1)
    {
        mainScreen.w = 256;
        mainScreen.h = 240;
        mainScreen.x = 0 - scrollx;
        mainScreen.y = 0 - scrolly;
        SDL_RenderSetViewport(renderer, &mainScreen);
        SDL_UpdateTexture(texture, NULL, pixels, blitsu);
        SDL_RenderCopy(renderer, texture, NULL, NULL);

        mainScreen.x = 0 - scrollx;
        mainScreen.y = 240 - scrolly;
        SDL_RenderSetViewport(renderer, &mainScreen);
        SDL_UpdateTexture(texture2400, NULL, pixels2400, blitsu);
        SDL_RenderCopy(renderer, texture2400, NULL, NULL);

        mainScreen.x = 256 - scrollx;
        mainScreen.y = 0 - scrolly;
        SDL_RenderSetViewport(renderer, &mainScreen);
        SDL_UpdateTexture(texture, NULL, pixels, blitsu);
        SDL_RenderCopy(renderer, texture, NULL, NULL);

        mainScreen.x = 256 - scrollx;
        mainScreen.y = 240 - scrolly;
        SDL_RenderSetViewport(renderer, &mainScreen);
        SDL_UpdateTexture(texture2400, NULL, pixels2400, blitsu);
        SDL_RenderCopy(renderer, texture2400, NULL, NULL);

    }
    //printf("Scrollx: 0x%X\n",scrollx);
    //printf("Scrolly: 0x%X\n",scrolly);
    //printf("nameuse: 0x%X\n",nametableuse);
    if(nametableuse == 2 || nametableuse == 3)
    {
        mainScreen.w = 256;
        mainScreen.h = 240;
        mainScreen.x = 0 - scrollx;
        mainScreen.y = 240 - scrolly;
        SDL_RenderSetViewport(renderer, &mainScreen);
        SDL_UpdateTexture(texture, NULL, pixels, blitsu);
        SDL_RenderCopy(renderer, texture, NULL, NULL);

        mainScreen.x = 0 - scrollx;
        mainScreen.y = 0 - scrolly;
        SDL_RenderSetViewport(renderer, &mainScreen);
        SDL_UpdateTexture(texture2400, NULL, pixels2400, blitsu);
        SDL_RenderCopy(renderer, texture2400, NULL, NULL);

        mainScreen.x = 256 - scrollx;
        mainScreen.y = 240 - scrolly;
        SDL_RenderSetViewport(renderer, &mainScreen);
        SDL_UpdateTexture(texture, NULL, pixels, blitsu);
        SDL_RenderCopy(renderer, texture, NULL, NULL);

        mainScreen.x = 256 - scrollx;
        mainScreen.y = 0 - scrolly;
        SDL_RenderSetViewport(renderer, &mainScreen);
        SDL_UpdateTexture(texture2400, NULL, pixels2400, blitsu);
        SDL_RenderCopy(renderer, texture2400, NULL, NULL);
    }
}
int handleSprites()
{
    if(maskProperties[4] == false)
    {
        return 0;
    }
    // Begin Sprite Code
    ypixS = 0;
    xpixS = 0;
    sprite0 = false;
    currentSprite = 0xFC;
    spriteYpos = OAMmem[0xFC];
    if(spriteSize == 1)
    {
        OAMmem[0xFD] = nBit.resetBit(OAMmem[0xFD], 0);
    }
    spritePatIndex = OAMmem[0xFD] << 4;
    //tempBitBuffer = OAMmem[0xFD];
    if(sprite1000 == 1 && spriteSize == 0) // If the bank 1000 bit is set in sprite, use 0x1000 sprites instead.
    {
        spritePatIndex += 0x1000;
    }
    if((OAMmem[0xFD] & 0x1) == 1 && spriteSize == 1)
    {
        spritePatIndex += 0x1000;
    }
    flipBitBuffer = OAMmem[0xFE];
    spriteAttributes = OAMmem[0xFE];
    spriteAtt = OAMmem[0xFE];
    spriteXpos = OAMmem[0xFF];
    help1 = spriteXpos + 8;
    help2 = spriteYpos + 8;
    //uint8_t temp1;
    //uint8_t temp2;
    ypixS = spriteYpos;
    xpixS = spriteXpos;
    spriteYpos += 8;
    helpXflip = 8;
    spriteXflip = xpixS + 7;
    //printf("spriteY: 0x%X\n",spriteYpos);
    //printf("spriteX: 0x%X\n",spriteXpos);
    bitcountS = 7;
    getBGcolor();
    while(currentSprite != 0xFFFC)
    {
        if(spriteYpos > 240)
        {
            goto noDrawSprite;
        }
        if(spriteYpos <= 7 && maskProperties[2] == false)
        {
            goto noDrawSprite;
        }
        if(flipBitBuffer[6] == 0 && flipBitBuffer[7] == 0)
        {
            dontTwi = false;
            renderSprite();
        }
        if(flipBitBuffer[6] == 1 && flipBitBuffer[7] == 0)
        {
            have16 = false;
            dontTwi = false;
            renderHFLIPSprite();
        }
        if(flipBitBuffer[6] == 0 && flipBitBuffer[7] == 1)
        {
            dontTwi = false;
            if(spriteSize == 0)
            {
                renderVFLIPSprite();
            }
            if(spriteSize == 1)
            {
                dontTwi = false;
                spriteXpos = OAMmem[currentSprite + 3];
                spriteYpos = OAMmem[currentSprite];
                ypixS = spriteYpos + 16;
                help2 = spriteYpos + 8;
                renderVSprite16();
            }
        }
        if(flipBitBuffer[6] == 1 && flipBitBuffer[7] == 1)
        {
            dontTwi = false;
            if(spriteSize == 0)
            {
                renderHVFLIPSprite();
            }
            if(spriteSize == 1)
            {
                dontTwi = false;
                spriteXpos = OAMmem[currentSprite + 3];
                spriteYpos = OAMmem[currentSprite];
                ypixS = spriteYpos + 16;
                help2 = spriteYpos + 8;
                renderHVFLIPSprite16();
            }
        }
        noDrawSprite:
        currentSprite -= 4;
        if(currentSprite == 0xFFFC)
        {
            //printf("wowie\n");
            return 0;
        }
        sprite0 = (currentSprite == 0);
        spriteYpos = OAMmem[currentSprite];
        tempBitBuffer = OAMmem[currentSprite + 1];
        //spritePatIndex = OAMmem[currentSprite + 1] << 4;
        tempBitBuffer2 = OAMmem[currentSprite + 1];
        if(spriteSize == 1)
        {
            tempBitBuffer2[0] = 0;
        }
        spritePatIndex = tempBitBuffer2.to_ulong() << 4;
        if(sprite1000 == 1 && spriteSize == 0) // If the bank 1000 bit is set in sprite, use 0x1000 sprites instead.
        {
            spritePatIndex += 0x1000;
        }
        if(tempBitBuffer[0] == 1 && spriteSize == 1)
        {
            spritePatIndex += 0x1000;
        }
        flipBitBuffer = OAMmem[currentSprite + 2];
        spriteAttributes = OAMmem[currentSprite + 2];
        spriteAtt = OAMmem[currentSprite + 2];
        spriteXpos = OAMmem[currentSprite + 3];
        dontbreaksp0 = false;
        help1 = spriteXpos + 8;
        help2 = spriteYpos + 8;
        ypixS = spriteYpos;
        xpixS = spriteXpos;
        //spriteYpos += 8;
        bitcountS = 7;

    }
    return 0;
}
uint8_t xpixplus;
uint8_t ypixplus;
uint16_t xpixsc;
uint8_t xpixsc2;
uint8_t ypixsc;
uint8_t ppurendercount;
uint16_t plusAmount;
int handleGraphicsBASIC()
{
    //cout<<graphicThread<<endl;
    //printf("Frame\n");
    //handleSprites();
    //currentPPUFrame++;
    ppurendercount = 2;
    plusAmount = 0;
    nametableAddr2 = 0x2000;
    while(ppurendercount != 0)
    {
    Xcounter = 32;
    Ycounter = 32;
    //return 0;
    bitcount = 7;
    tiledatalocation1 = nametableAddr2;
    tilelocation = NESOB.PPUmemory[tiledatalocation1];
    //tilelocation++;
    tilecount = tilelocation << 4;
    xpix = 0;
    ypix = 0;
    xpixtile = 0;
    ypixtile = 0;
    xpixtile2 = 0;
    ypixtile2 = 0;
    currentnametable = 0;
    SDL_SetRenderDrawColor(renderer,0x00,0x00,0xFF,0xFF);
    SDL_RenderClear(renderer);
    while(Ycounter != 0)
    {
        while(Xcounter != 0)
        {
            while(ypixtile2 != 8)
            {
                graphicline = NESOB.PPUmemory[tilecount];
                graphicline2 = NESOB.PPUmemory[tilecount + 8];
                while(xpixtile2 != 8)
                {
                    chooseRenderColorREWRITE();

                    ypix2 = ypix;
                    xpixsc = xpix - scrollx;
                    ypixsc = ypix - scrolly;
                    if(scrolly > 240)
                    {
                        ypixsc = ypix;
                    }
                    curPixx = (xpix);
                    curPixy = ((ypix) * 256 );
                    curPixx = curPixx + curPixy;
                    curPixx = curPixx;
                    if(nametableAddr2 == 0x2000)
                    {
                        if (palResult == 0)
                        {
                            pixels[curPixx] = pal00[0] << 24 | pal00[1] << 16 | pal00[2] << 8 | pal00[3];
                        }
                        if (palResult == 1)
                        {
                            pixels[curPixx] = pal01[0] << 24 | pal01[1] << 16 | pal01[2] << 8 | pal01[3];
                        }
                        if (palResult == 2)
                        {
                            pixels[curPixx] = pal10[0] << 24 | pal10[1] << 16 | pal10[2] << 8 | pal10[3];
                        }
                        if (palResult == 3)
                        {
                            pixels[curPixx] = pal11[0] << 24 | pal11[1] << 16 | pal11[2] << 8 | pal11[3];
                        }

                        if (showGrid == true)
                        {
                                pixels[curPixx] = pal10[0] << 24 | pal10[1] << 16 | pal10[2] << 8 | pal10[3];
                                if(xpix % 16 == 0 || ypix % 16 == 0)
                                {
                                    pixels[curPixx] = 0xFF << 24 | 0x00 << 16 | 0xFF << 8 | 0xFF;
                                }
                                if(currentBlock == 0)
                                {
                                    pixels[curPixx] = 0 << 24 | 0xFF << 16 | 0 << 8 | 0xFF;
                                }
                                if(currentBlock == 2)
                                {
                                    pixels[curPixx] = 0xFF << 24 | 0x00 << 16 | 0 << 8 | 0xFF;
                                }
                                if(currentBlock == 4)
                                {
                                    pixels[curPixx] = 0xFF << 24 | 0xFF << 16 | 0 << 8 | 0xFF;
                                }
                                if(currentBlock == 6)
                                {
                                    pixels[curPixx] = 0xFF << 24 | 0xFF << 16 | 0x50 << 8 | 0xFF;
                                }
                        }
                    }
                    if(nametableAddr2 == 0x2400 || 0x2800)
                    {
                        if (palResult == 0)
                        {
                            pixels2400[curPixx] = pal00[0] << 24 | pal00[1] << 16 | pal00[2] << 8 | pal00[3];
                        }
                        if (palResult == 1)
                        {
                            pixels2400[curPixx] = pal01[0] << 24 | pal01[1] << 16 | pal01[2] << 8 | pal01[3];
                        }
                        if (palResult == 2)
                        {
                            pixels2400[curPixx] = pal10[0] << 24 | pal10[1] << 16 | pal10[2] << 8 | pal10[3];
                        }
                        if (palResult == 3)
                        {
                            pixels2400[curPixx] = pal11[0] << 24 | pal11[1] << 16 | pal11[2] << 8 | pal11[3];
                        }
                        if (showGrid == true)
                        {
                            if(xpix % 32 == 0 || ypix % 32 == 0)
                            {
                                pixels2400[curPixx] = 0 << 24 | 0xFF << 16 | 0 << 8 | 0xFF;
                            }
                        }
                    }
                    //SDL_RenderDrawPoint(renderer,(xpix - scrollx) + plusAmount,ypix2 - scrolly); // Middle
                    /*
                    SDL_RenderDrawPoint(renderer,(xpix - scrollx) + 0x100,(ypix2 - scrolly)); // Right
                    SDL_RenderDrawPoint(renderer,(xpix - scrollx) + 0x100,(ypix2 - scrolly) + 0x100); // Bottom-Right
                    SDL_RenderDrawPoint(renderer,(xpix - scrollx),(ypix2 - scrolly) + 0x100); // Bottom
                    SDL_RenderDrawPoint(renderer,(xpix - scrollx) - 0x100,(ypix2 - scrolly) + 0x100); // Bottom Left
                    SDL_RenderDrawPoint(renderer,(xpix - scrollx) - 0x100,(ypix2 - scrolly)); // Left
                    SDL_RenderDrawPoint(renderer,(xpix - scrollx) - 0x100,(ypix2 - scrolly) - 0x100); // Top Left
                    SDL_RenderDrawPoint(renderer,(xpix - scrollx),(ypix2 - scrolly) - 0x100); // Top
                    SDL_RenderDrawPoint(renderer,(xpix - scrollx) + 0x100,(ypix2 - scrolly) - 0x100); // Top Right
                    */
                    xpix++;
                    bitcount--;
                    xpixtile2++;
                }
                ypix += 1;
                xpix = (xpixtile * 8);
                bitcount = 7;
                tilecount++;
                xpixtile2 = 0;
                ypixtile2++;
            }
            //printf("testing\n");
            xpixtile++;
            Xcounter--;
            ypix = (ypixtile * 8);
            xpix = (xpixtile * 8);
            xpixtile2 = 0;
            ypixtile2 = 0;
            tiledatalocation1++;
            tilelocation = NESOB.PPUmemory[tiledatalocation1];
            //tilelocation += 1;
            //printf("tilelocation: 0x%X\n",tilelocation);
            tilecount = tilelocation << 4;
            if(bgpattable == true)
            {
                tilecount += 0x1000;
            }
        }
        xpixtile = 0x00;
        ypixtile++;
        Xcounter = 32;
        Ycounter--;
        ypix = (ypixtile * 8);
        xpix = (xpixtile * 8);
    }
        Xcounter = 32;
        Ycounter = 32;
        currentnametable = 1;
        ppurendercount--;
        nametableAddr2 = 0x2800;
        if(MMC3mirror == false)
        {
            nametableAddr2 = 0x2400;
        }
        //plusAmount = 65536;
    }

    if(MMC3mirror == false)
    {
        horizontalmirror();
    }
    if(MMC3mirror == true)
    {
        verticalmirror();
    }
    mainScreen.x = 0;
    mainScreen.y = 0;
    SDL_RenderSetViewport(renderer, &mainScreen);
    //SDL_RenderFillRect(renderer, &mainScreen);
    handleSprites();
    mainScreen.w = 256;
    mainScreen.h = 256;
    SDL_RenderSetViewport(renderer, &mainScreen);
    SDL_RenderPresent(renderer);
    //renderVRAM();

    //printf("ScrollX: 0x%X\n",scrollx);
    //printf("ScrollY: 0x%X\n",scrolly);
    currentPPUFrame++;
    //printf("Current PPU Frame: %i\n",currentPPUFrame);
        while(graphicThread == true)
        {
            if(newFrame == true)
            {
                newFrame = false;
                handleGraphicsBASIC();
            }
            if(exitLoop == true)
            {
                return 0;
            }
        }
    return 0;
}
std::bitset<9> bit916convert;
uint16_t vTran;
uint8_t yscro8;
uint16_t xS9bit;
bool dontRenderline;
uint8_t fineYscroll;
uint16_t yscro16;
uint16_t ypixX;
uint16_t xpixX;
uint16_t xadd;
uint16_t yadd;
uint16_t lookupNametable[4] = {0x2000,0x2400,0x2800,0x2C00};
uint16_t invertedlook1[4] = {0x2400,0x2000,0x2400,0x2000};
uint16_t invertedlook2[4] = {0x2800,0x2800,0x2000,0x2000};
uint8_t palResult4;
uint16_t modScan;
void testThread()
{
    SDL_UpdateTexture(textureScanline, NULL, pixels, blitsu);
    SDL_RenderCopy(renderer, textureScanline, NULL, &mainScreen);
}
uint8_t ppuRenderC;
uint16_t testFixPix;
uint16_t loopTemp;
int handleGraphicsBASICSCAN()
{
    if(maskProperties[3] == 0)
    {
        if(NESOB.scanline != 260)
        {
            return 0;
        }
        goto doneRenderFrame;
    }
    if(NESOB.scanline >= 240 && NESOB.scanline != 260)
    {
        return 0;
    }
    if(NESOB.scanline == 260)
    {
        goto doneRenderFrame;
    }
    ppurendercount = 2;
    loopTemp = Loopy;
    nametableuse = (nBit.testBit(loopTemp, 11) << 1) + nBit.testBit(loopTemp, 10);
    loopTemp = Loopy;
    loopTemp = nBit.resetBit(loopTemp, 7);
    loopTemp = nBit.resetBit(loopTemp, 6);
    loopTemp = nBit.resetBit(loopTemp, 5);
    scrollx = loopTemp;
    scrollx = scrollx << 3;
    scrollx += fineXscroll;
    //printf("scrollx: %i\n",scrollx);
    loopTemp = Loopy >> 5;
    loopTemp = nBit.resetBit(loopTemp, 7);
    loopTemp = nBit.resetBit(loopTemp, 6);
    loopTemp = nBit.resetBit(loopTemp, 5);
    scrolly = loopTemp;
    scrolly = scrolly << 3;
    loopTemp = Loopy >> 12;
    loopTemp = nBit.resetBit(loopTemp, 7);
    loopTemp = nBit.resetBit(loopTemp, 6);
    loopTemp = nBit.resetBit(loopTemp, 5);
    loopTemp = nBit.resetBit(loopTemp, 4);
    loopTemp = nBit.resetBit(loopTemp, 3);
    fineYscroll = loopTemp;
    scrolly += fineYscroll;
    //printf("Scrolly: %X\n",scrolly);
    nametableAddr2 = nametableLookup[nametableuse];
    //printf("Nametable1: 0x%X\n",nametableAddr2);
    modScan = NESOB.scanline & 7;
    switch(scrollx)
    {
        case 0:
            ppuRenderC = 1;
        break;

        default:
            ppuRenderC = 0;
        break;
    }
    while(ppurendercount != ppuRenderC)
    {
    Xcounter = 32;
    bitcount = 7;
    tiledatalocation1 = nametableAddr2 + ((NESOB.scanline) >> 3) * 32;
    tilelocation = NESOB.PPUmemory[tiledatalocation1];
    tilecount = tilelocation << 4;
    tilecount += modScan;
    xpix = 0;
    ypix = NESOB.scanline;
    xpixtile = 0;
    ypixtile = 0;
    xpixtile2 = 0;
    ypixtile2 = 0;
    //printf("ys: %i\n",scrolly);
    tilecount += (bgpattable << 0xC);
        while(Xcounter != 0)
        {
                //graphicline = NESOB.PPUmemory[tilecount];
                //graphicline2 = NESOB.PPUmemory[tilecount + 8];
                if(Xcounter % 2 == 0)
                {
                    chooseRenderColorREWRITE();
                }
                while(xpixtile2 != 8)
                {

                    //palResult = palBlolookup[(graphicline2[bitcount] * 2) + graphicline[bitcount]];
                    palResult = palBlolookup[(nBit.testBit(NESOB.PPUmemory[tilecount + 8],
                                                           bitcount) << 1) +
                                             (nBit.testBit(NESOB.PPUmemory[tilecount],
                                                           bitcount)    )];
                    xS9bit = xpix - scrollx;
                    xS9bit = xS9bit & 511;
                    curPixx = xS9bit;

                    switch(MMC3mirror)
                    {
                        case false:
                            if(ppurendercount == 1)
                            {
                                curPixx += 256;
                                xS9bit = curPixx;
                            }

                            curPixx = curPixx & 511;
                            xS9bit = xS9bit & 511;

                            if(xS9bit > 255)
                            {
                                xpix -= xpixtile2;
                                xpix += 8;
                                goto dontRenderTile; // Render Optimization
                            }

                            //if(NESOB.scanline < 240)
                            //{
                                yscro8 = NESOB.scanline - scrolly;
                                if(yscro8 >= 240 - (scrolly - 16))
                                {
                                    yscro8 -= 16;
                                }
                                curPixx += yscro8 * 512;
                            //}
                            palResult4 = palResult << 2;
                            pixels[curPixx] = pal[palResult4] << 16 | pal[1 + palResult4] << 8 | pal[2 + palResult4];
                            //pixelsOptimize[yscro8][xS9bit] = pal[0 + palResult4] << 16 | pal[1 + palResult4] << 8 | pal[2 + palResult4];
                            if(xS9bit <= 7 && maskProperties[1] == false)
                            {
                                pixels[curPixx] = pal[0] << 16 | pal[1] << 8 | pal[2];
                                //pixelsOptimize[yscro8][xS9bit] = pal[0] << 16 | pal[1] << 8 | pal[2];
                            }
                        break;

                        case true:
                            if(NESOB.scanline < 240)
                            {
                                yscro16 = NESOB.scanline;
                                yscro16 -= scrolly;
                                if(yscro16 > 480)
                                {
                                    yscro16 -= 65055;
                                }
                                //curPixx = yscro16 * 256; // Sets Current Y line
                                if(ppurendercount == 1)
                                {
                                    yscro16 += 240;
                                }
                                if(yscro16 > 480)
                                {
                                    yscro16 -= 480;
                                }
                                curPixx = yscro16 * 256; // Sets Current Y line
                            }
                            dummy8 = xpix;
                            dummy8 -= scrollx;
                            curPixx += dummy8;
                            palResult4 = palResult << 2;
                            pixelsScan28[curPixx] = pal[0 + palResult4] << 16 | pal[1 + palResult4] << 8 | pal[2 + palResult4];
                            if(dummy8 <= 7 && maskProperties[1] == false)
                            {
                                pixelsScan28[curPixx] = pal[0] << 16 | pal[1] << 8 | pal[2];
                            }
                        break;
                    }
                    dontRenderPix:
                    ++xpix;
                    --bitcount;
                    ++xpixtile2;
                }
            dontRenderTile:
            ++xpixtile;
            --Xcounter;
            xpixtile2 = 0;
            ++tiledatalocation1;
            bitcount = 7;
            tilelocation = NESOB.PPUmemory[tiledatalocation1];
            tilecount = ((tilelocation << 4) + (modScan));
            //if(bgpattable == true)
           // {
            //    tilecount += 0x1000;
            //}
            tilecount += bgpattable * 0x1000;
        }
        currentnametable = 0;
        --ppurendercount;
        if(MMC3mirror == false)
        {
            nametableAddr2 = invertedlook1[nametableuse];
        }
        if(MMC3mirror == true)
        {
            nametableAddr2 = invertedlook2[nametableuse];
        }
        //printf("Nametable2: 0x%X\n",nametableAddr2);
    }
    return 0;
    //printf("Nametableuse: %i\n",nametableuse);
    //printf("Coarse Y Scroll: 0x%X\n",coarseYscroll);
    //printf("Coarse X Scroll: 0x%X\n",coarseXscroll);
    doneRenderFrame:
    if(MMC3mirror == false)
    {

    blitsu = 2048;
    mainScreen.w = 512;
    mainScreen.h = 240;
    mainScreen.x = 0;
    mainScreen.y = 0;
    //printf("Coarse Y Scroll: 0x%X\n",coarseYscroll);
    //printf("Coarse X Scroll: 0x%X\n",coarseXscroll);
    //printf("Scrollx: 0x%X\n",scrollx);
    //printf("Scrolly: 0x%X\n",scrolly);
    //printf("Nametableuse: %i\n",nametableuse);
    SDL_UpdateTexture(textureScanline, NULL, pixels, blitsu);
    SDL_RenderCopy(renderer, textureScanline, NULL, &mainScreen);
    //mainScreen.y = 240;
    //SDL_UpdateTexture(textureScanline, NULL, pixels, blitsu);
    //SDL_RenderCopy(renderer, textureScanline, NULL, &mainScreen);
    handleSprites();
    //SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF,0,0);
    //SDL_RenderDrawLine(renderer, 0,175,512,175);
    notification[0].w = 128;
    notification[0].h = 20;
    notification[0].x = 0;
    notification[0].y = 220;
    notification[1].w = 128;
    notification[1].h = 20;
    notification[1].x = 0;
    notification[1].y = 220;
    //SDL_RenderCopy(renderer, textureScanline, NULL, &notification[0]);
    if(notificationTimer == 0)
    {
        notification[1].w = 0;
        notification[1].h = 0;
        notification[0].w = 0;
        notification[0].h = 0;
    }
    if(notificationTimer != 0)
    {
        SDL_SetRenderDrawColor(renderer, 0xA0,0xA0,0xA0,0x00);
        SDL_RenderFillRect(renderer, &notification[0]);
        SDL_RenderCopy(renderer, notiText, NULL, &notification[1]);
        notificationTimer--;
    }
    SDL_RenderPresent(renderer);
    currentPPUFrame++;
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00,0,0);
    SDL_RenderClear(renderer);
    }
    if(MMC3mirror == true)
    {
    blitsu = 1024;
    //printf("Coarse Y Scroll: 0x%X\n",coarseYscroll);
    //printf("Coarse X Scroll: 0x%X\n",coarseXscroll);
    //printf("Scrollx: 0x%X\n",scrollx);
    //printf("Scrolly: 0x%X\n",scrolly);
    //printf("Nametableuse: %i\n",nametableuse);
    mainScreen.w = 256;
    mainScreen.h = 480;
    mainScreen.x = 0;
    mainScreen.y = 0;
    SDL_UpdateTexture(textureScanline28, NULL, pixelsScan28, blitsu);
    SDL_RenderCopy(renderer, textureScanline28, NULL, &mainScreen);
    handleSprites();
    //SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF,0,0);
    //SDL_RenderDrawLine(renderer, 0,175,512,175);
    notification[0].w = 128;
    notification[0].h = 20;
    notification[0].x = 0;
    notification[0].y = 220;
    notification[1].w = 128;
    notification[1].h = 20;
    notification[1].x = 0;
    notification[1].y = 220;
    //SDL_RenderCopy(renderer, textureScanline, NULL, &notification[0]);
    if(notificationTimer == 0)
    {
        notification[1].w = 0;
        notification[1].h = 0;
        notification[0].w = 0;
        notification[0].h = 0;
    }
    if(notificationTimer != 0)
    {
        SDL_SetRenderDrawColor(renderer, 0xA0,0xA0,0xA0,0x00);
        SDL_RenderFillRect(renderer, &notification[0]);
        SDL_RenderCopy(renderer, notiText, NULL, &notification[1]);
        notificationTimer--;
    }
    SDL_RenderPresent(renderer);
    currentPPUFrame++;
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00,0,0);
    SDL_RenderClear(renderer);
    }
    return 0;
}




uint16_t testValF;
uint16_t testValF2;
uint16_t scrollYF;
int handleGraphicsScan()
{
    if(maskProperties[3] == 0)
    {
        if(NESOB.scanline != 260)
        {
            return 0;
        }
        goto doneRenderFrame;
    }
    if(NESOB.scanline >= 240 && NESOB.scanline != 260)
    {
        return 0;
    }
    if(NESOB.scanline == 260)
    {
        goto doneRenderFrame;
    }
    ppurendercount = 2;
    tempBitBuffer16 = Loopy;
    nametableuse = (tempBitBuffer16[11] * 2) + tempBitBuffer16[10];
    tempBitBuffer = Loopy;
    tempBitBuffer[7] = 0;
    tempBitBuffer[6] = 0;
    tempBitBuffer[5] = 0;
    scrollx = tempBitBuffer.to_ulong();
    scrollx = scrollx * 8;
    scrollx += fineXscroll;
    //printf("scrollx: %i\n",scrollx);
    tempBitBuffer = Loopy >> 5;
    tempBitBuffer[7] = 0;
    tempBitBuffer[6] = 0;
    tempBitBuffer[5] = 0;
    scrolly = tempBitBuffer.to_ulong();
    scrolly = scrolly * 8;
    tempBitBuffer = Loopy >> 12;
    tempBitBuffer[7] = 0;
    tempBitBuffer[6] = 0;
    tempBitBuffer[5] = 0;
    tempBitBuffer[4] = 0;
    tempBitBuffer[3] = 0;
    fineYscroll = tempBitBuffer.to_ulong();
    scrolly += fineYscroll;
    //printf("Scrolly: %X\n",scrolly);
    nametableAddr2 = nametableLookup[nametableuse];
    //printf("Nametable1: 0x%X\n",nametableAddr2);
    if(NESOB.scanline == 135)
    {
        printf("Scrolly: 0x%X\n",scrolly);
        printf("Scrollx: 0x%X\n",scrollx);
        printf("nameuse: 0x%X\n",nametableAddr2);
    }
    while(ppurendercount != 1)
    {
    Xcounter = 32;
    bitcount = 7;
    if(MMC3mirror == false)
    {
        testValF2 = NESOB.scanline + scrolly;
        if(testValF2 >= 260)
        {
            testValF2 -= 260;
        }
        testValF = nametableAddr2 + ((scrolly) / 8) * 32;
    }
    if(MMC3mirror == true)
    {
        //testValF2 = NESOB.scanline + scrolly;
        if(testValF2 < 260)
        {
            testValF = nametableAddr2 + ((scrolly) / 8) * 32;
        }
        if(testValF2 >= 260)
        {
            testValF2 -= 260;
            if(nametableAddr2 == 0x2000)
            {
                testValF = 0x2800;
            }
            if(nametableAddr2 == 0x2800)
            {
                testValF = 0x2000;
            }
            testValF += ((testValF2) / 8) * 32;
        }
    }
    testValF = nametableAddr2 + ((scrolly) / 8) * 32;
    tiledatalocation1 = testValF;
    tilelocation = NESOB.PPUmemory[tiledatalocation1];
    tilecount = tilelocation << 4;
    tilecount += NESOB.scanline % 8;
    xpix = 0;
    ypix = NESOB.scanline;
    xpixtile = 0;
    ypixtile = 0;
    xpixtile2 = 0;
    ypixtile2 = 0;
    //printf("ys: %i\n",scrolly);
    tilecount = tilecount + bgpattable * 0x1000;
        while(Xcounter != 0)
        {
                graphicline = NESOB.PPUmemory[tilecount];
                graphicline2 = NESOB.PPUmemory[tilecount + 8];
                if(Xcounter % 2 == 0)
                {
                    chooseRenderColorREWRITE();
                }
                while(xpixtile2 != 8)
                {
                    palResult = palBlolookup[(graphicline2[bitcount] * 2) + graphicline[bitcount]];
                    if(MMC3mirror == false)
                    {
                        curPixx = (NESOB.scanline * 256) + xpix;
                        palResult4 = palResult * 4;
                        pixelsScanF[NESOB.scanline][xpix] = pal[0 + palResult4] << 16 | pal[1 + palResult4] << 8 | pal[2 + palResult4];
                        if(xpix <= 7 && maskProperties[1] == false)
                        {
                            pixelsScanF[NESOB.scanline][xpix] = pal[0] << 16 | pal[1] << 8 | pal[2];
                        }
                    }

                    if(MMC3mirror == true)
                    {

                        palResult4 = palResult * 4;
                        pixelsScanF[NESOB.scanline][xpix] = pal[0 + palResult4] << 16 | pal[1 + palResult4] << 8 | pal[2 + palResult4];
                        if(dummy8 <= 7 && maskProperties[1] == false)
                        {
                            pixelsScanF[NESOB.scanline][xpix] = pal[0] << 16 | pal[1] << 8 | pal[2];
                        }

                    }
                    xpix++;
                    bitcount--;
                    xpixtile2++;
                }
            xpixtile++;
            Xcounter--;
            xpixtile2 = 0;
            tiledatalocation1++;
            //printf("TDL1: 0x%X\n",Xcounter);
            bitcount = 7;
            tilelocation = NESOB.PPUmemory[tiledatalocation1];
            tilecount = ((tilelocation << 4) + (NESOB.scanline % 8));
            //if(bgpattable == true)
           // {
            //    tilecount += 0x1000;
            //}
            tilecount += bgpattable * 0x1000;

        }
        currentnametable = 0;
        ppurendercount -= 1;
        if(MMC3mirror == false)
        {
            nametableAddr2 = invertedlook1[nametableuse];
        }
        if(MMC3mirror == true)
        {
            nametableAddr2 = invertedlook2[nametableuse];
        }
        //printf("Nametable2: 0x%X\n",nametableAddr2);
    }
    return 0;
    //printf("Nametableuse: %i\n",nametableuse);
    //printf("Coarse Y Scroll: 0x%X\n",coarseYscroll);
    //printf("Coarse X Scroll: 0x%X\n",coarseXscroll);
    doneRenderFrame:
    if(MMC3mirror == false)
    {

    blitsu = 1024;
    mainScreen.w = 256;
    mainScreen.h = 240;
    mainScreen.x = 0;
    mainScreen.y = 0;
    //printf("Coarse Y Scroll: 0x%X\n",coarseYscroll);
    //printf("Coarse X Scroll: 0x%X\n",coarseXscroll);
    //printf("Scrollx: 0x%X\n",scrollx);
    //printf("Scrolly: 0x%X\n",scrolly);
    //printf("Nametableuse: %i\n",nametableuse);
    SDL_UpdateTexture(textureScanlineF, NULL, pixelsScanF, blitsu);
    SDL_RenderCopy(renderer, textureScanlineF, NULL, &mainScreen);
    //mainScreen.y = 240;
    //SDL_UpdateTexture(textureScanline, NULL, pixels, blitsu);
    //SDL_RenderCopy(renderer, textureScanline, NULL, &mainScreen);
    handleSprites();
    //SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF,0,0);
    //SDL_RenderDrawLine(renderer, 0,175,512,175);
    notification[0].w = 128;
    notification[0].h = 20;
    notification[0].x = 0;
    notification[0].y = 220;
    notification[1].w = 128;
    notification[1].h = 20;
    notification[1].x = 0;
    notification[1].y = 220;
    //SDL_RenderCopy(renderer, textureScanline, NULL, &notification[0]);
    if(notificationTimer == 0)
    {
        notification[1].w = 0;
        notification[1].h = 0;
        notification[0].w = 0;
        notification[0].h = 0;
    }
    if(notificationTimer != 0)
    {
        SDL_SetRenderDrawColor(renderer, 0xA0,0xA0,0xA0,0x00);
        SDL_RenderFillRect(renderer, &notification[0]);
        SDL_RenderCopy(renderer, notiText, NULL, &notification[1]);
        notificationTimer--;
    }
    SDL_RenderPresent(renderer);
    currentPPUFrame++;
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00,0,0);
    SDL_RenderClear(renderer);
    }
    if(MMC3mirror == true)
    {
    blitsu = 1024;
    //printf("Coarse Y Scroll: 0x%X\n",coarseYscroll);
    //printf("Coarse X Scroll: 0x%X\n",coarseXscroll);
    //printf("Scrollx: 0x%X\n",scrollx);
    //printf("Scrolly: 0x%X\n",scrolly);
    //printf("Nametableuse: %i\n",nametableuse);
    mainScreen.w = 256;
    mainScreen.h = 240;
    mainScreen.x = 0;
    mainScreen.y = 0;
    SDL_UpdateTexture(textureScanlineF, NULL, pixelsScanF, blitsu);
    SDL_RenderCopy(renderer, textureScanlineF, NULL, &mainScreen);
    handleSprites();
    //SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF,0,0);
    //SDL_RenderDrawLine(renderer, 0,175,512,175);
    notification[0].w = 128;
    notification[0].h = 20;
    notification[0].x = 0;
    notification[0].y = 220;
    notification[1].w = 128;
    notification[1].h = 20;
    notification[1].x = 0;
    notification[1].y = 220;
    //SDL_RenderCopy(renderer, textureScanline, NULL, &notification[0]);
    if(notificationTimer == 0)
    {
        notification[1].w = 0;
        notification[1].h = 0;
        notification[0].w = 0;
        notification[0].h = 0;
    }
    if(notificationTimer != 0)
    {
        SDL_SetRenderDrawColor(renderer, 0xA0,0xA0,0xA0,0x00);
        SDL_RenderFillRect(renderer, &notification[0]);
        SDL_RenderCopy(renderer, notiText, NULL, &notification[1]);
        notificationTimer--;
    }
    SDL_RenderPresent(renderer);
    currentPPUFrame++;
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00,0,0);
    SDL_RenderClear(renderer);
    }
    return 0;
}
