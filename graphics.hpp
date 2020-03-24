#include "mappers.hpp"
uint8_t bitcount;
SDL_Texture* texture = NULL;
SDL_Texture* texture2400 = NULL;
SDL_Texture* textureScanline = NULL;
SDL_Rect mainScreen;
std::bitset<8> graphicline;
std::bitset<8> graphicline2;
std::bitset<8> graphiclineS;
std::bitset<8> graphicline2S;
int blitsu = 1024;
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
int pal00[4] = {0xFF,0xFF,0xFF,0x00};
int pal01[4] = {0xC2,0xC2,0xC2,0x00};
int pal10[4] = {0x76,0x76,0x76,0x00};
int pal11[4] = {0x00,0x00,0x00,0x00};
int pal00S[4] = {0xFF,0xFF,0xFF,0x00};
int pal01S[4] = {0xC2,0xC2,0xC2,0x00};
int pal10S[4] = {0x76,0x76,0x76,0x00};
int pal11S[4] = {0x00,0x00,0x00,0x00};
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
void horipal()
{
    currentGridat = NESOB.PPUmemory[(nametableAddr2 + 0x3C0) + currentGrid];
}
void vertpal()
{
    if(nametableuse == 0 || nametableuse == 1)
    {
        if(currentnametable == 0)
        {
            currentGridat = NESOB.PPUmemory[(0x23C0) + currentGrid];
        }
        if(currentnametable == 1)
        {
            currentGridat = NESOB.PPUmemory[(0x23C0) + currentGrid];
        }
        if(currentnametable == 2)
        {
            currentGridat = NESOB.PPUmemory[(0x2BC0) + currentGrid];
        }
        if(currentnametable == 3)
        {
            currentGridat = NESOB.PPUmemory[(0x2BC0) + currentGrid];
        }
    }
    if(nametableuse == 2 || nametableuse == 3)
    {
        if(currentnametable == 0)
        {
            currentGridat = NESOB.PPUmemory[(0x23C0) + currentGrid];
        }
        if(currentnametable == 1)
        {
            currentGridat = NESOB.PPUmemory[(0x23C0) + currentGrid];
        }
        if(currentnametable == 2)
        {
            currentGridat = NESOB.PPUmemory[(0x2BC0) + currentGrid];
        }
        if(currentnametable == 3)
        {
            currentGridat = NESOB.PPUmemory[(0x2BC0) + currentGrid];
        }
    }
}
void palDetermineREWRITE()
{
    if(MMC3mirror == false)
    {
        horipal();
    }
    if(MMC3mirror == true)
    {
        vertpal();
    }
    currentBlockat[0] = currentGridat[currentBlock];
    currentBlockat[1] = currentGridat[currentBlock + 1];
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
        SDL_SetRenderDrawColor(renderer,color_00.r,color_00.g,color_00.b,color_00.a);
        palResult = 0;
    }
    if(graphicline[bitcount] == 1 && graphicline2[bitcount] == 0)
    {
        SDL_SetRenderDrawColor(renderer,color_01.r,color_01.g,color_01.b,color_01.a);
        palResult = 1;
    }
    if(graphicline[bitcount] == 0 && graphicline2[bitcount] == 1)
    {
        palResult = 2;
        SDL_SetRenderDrawColor(renderer,color_02.r,color_02.g,color_02.b,color_02.a);
    }
    if(graphicline[bitcount] == 1 && graphicline2[bitcount] == 1)
    {
        palResult = 3;
        SDL_SetRenderDrawColor(renderer,color_03.r,color_03.g,color_03.b,color_03.a);
    }
}
uint8_t xpixti;
uint8_t ypixti;
void chooseRenderColorREWRITE()
{
    //handleBlock();
    xpixti = xpix / 8;
    ypixti = ypix / 8;
    currentblockx = xpix / 16;
    currentblocky = ypix / 16;
    currentgridx = xpix / 32;
    currentgridy = ypix / 32;
    currentGrid = currentgridx + currentgridy * 8;
    currentblockx = (xpixti / 2) % 2;
    currentblocky = (ypixti / 2) % 2;
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
    if(prevBlock != currentBlock)
    {
        palDetermineREWRITE();
    }
    if(graphicline[bitcount] == 0 && graphicline2[bitcount] == 0)
    {
        SDL_SetRenderDrawColor(renderer,color_00.r,color_00.g,color_00.b,color_00.a);
        palResult = 0;
    }
    if(graphicline[bitcount] == 1 && graphicline2[bitcount] == 0)
    {
        SDL_SetRenderDrawColor(renderer,color_01.r,color_01.g,color_01.b,color_01.a);
        palResult = 1;
    }
    if(graphicline[bitcount] == 0 && graphicline2[bitcount] == 1)
    {
        palResult = 2;
        SDL_SetRenderDrawColor(renderer,color_02.r,color_02.g,color_02.b,color_02.a);
    }
    if(graphicline[bitcount] == 1 && graphicline2[bitcount] == 1)
    {
        palResult = 3;
        SDL_SetRenderDrawColor(renderer,color_03.r,color_03.g,color_03.b,color_03.a);
    }
}
bool tempfixrender;
uint8_t ypix2;
uint16_t palLocateS;
std::bitset<8> OAMbitbuffer;
bool dontRenderSprite;
void chooseRenderColorSprite(uint8_t spritePal)
{
    OAMbitbuffer = spritePal;
    if(OAMbitbuffer[1] == 0 && OAMbitbuffer[0] == 0)
    {
        palLocateS = 0x3F11;
    }
    if(OAMbitbuffer[1] == 0 && OAMbitbuffer[0] == 1)
    {
        palLocateS = 0x3F15;
    }
    if(OAMbitbuffer[1] == 1 && OAMbitbuffer[0] == 0)
    {
        palLocateS = 0x3F19;
    }
    if(OAMbitbuffer[1] == 1 && OAMbitbuffer[0] == 1)
    {
        palLocateS = 0x3F1D;
    }
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
    if(graphiclineS[bitcountS] == 0 && graphicline2S[bitcountS] == 0)
    {
        SDL_SetRenderDrawColor(renderer,color_00.r,color_00.g,color_00.b,color_00.a);
        dontRenderSprite = true;
    }
    if(graphiclineS[bitcountS] == 1 && graphicline2S[bitcountS] == 0)
    {
        SDL_SetRenderDrawColor(renderer,color_01.r,color_01.g,color_01.b,color_01.a);
    }
    if(graphiclineS[bitcountS] == 0 && graphicline2S[bitcountS] == 1)
    {
        SDL_SetRenderDrawColor(renderer,color_02.r,color_02.g,color_02.b,color_02.a);
    }
    if(graphiclineS[bitcountS] == 1 && graphicline2S[bitcountS] == 1)
    {
        SDL_SetRenderDrawColor(renderer,color_03.r,color_03.g,color_03.b,color_03.a);
    }
}
uint32_t curPixx;
uint32_t curPixy;
uint32_t pixels[512 * 256];
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
int renderSprite()
{
        while(ypixS != help2)
        {
            graphiclineS = NESOB.PPUmemory[spritePatIndex];
            graphicline2S = NESOB.PPUmemory[spritePatIndex + 8];

            while(xpixS != help1)
            {
                chooseRenderColorSprite(spriteAttributes);

                if(dontRenderSprite != true)
                {
                    SDL_RenderDrawPoint(renderer,xpixS,ypixS); // Middle
                }
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
        return 0;
}
int renderHFLIPSprite()
{
        help1 = spriteXpos - 1;
        xpixS = spriteXpos;
        xpixS += 7;

        while(ypixS != help2)
        {
            graphiclineS = NESOB.PPUmemory[spritePatIndex];
            graphicline2S = NESOB.PPUmemory[spritePatIndex + 8];

            while(xpixS != help1)
            {
                chooseRenderColorSprite(spriteAttributes);

                if(dontRenderSprite != true)
                {
                    SDL_RenderDrawPoint(renderer,xpixS,ypixS); // Middle
                }
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
        return 0;
}
int renderVFLIPSprite()
{

        spritePatIndex += 7;
        while(ypixS != help2)
        {
            graphiclineS = NESOB.PPUmemory[spritePatIndex];
            graphicline2S = NESOB.PPUmemory[spritePatIndex + 8];

            while(xpixS != help1)
            {
                chooseRenderColorSprite(spriteAttributes);

                if(dontRenderSprite != true)
                {
                    SDL_RenderDrawPoint(renderer,xpixS,ypixS); // Middle
                }
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
int renderHVFLIPSprite()
{
        help1 = spriteXpos - 1;
        xpixS = spriteXpos;
        xpixS += 7;
        spritePatIndex += 7;
        while(ypixS != help2)
        {
            graphiclineS = NESOB.PPUmemory[spritePatIndex];
            graphicline2S = NESOB.PPUmemory[spritePatIndex + 8];

            while(xpixS != help1)
            {
                chooseRenderColorSprite(spriteAttributes);

                if(dontRenderSprite != true)
                {
                    SDL_RenderDrawPoint(renderer,xpixS,ypixS); // Middle
                }
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
    // Begin Sprite Code
    ypixS = 0;
    xpixS = 0;
    currentSprite = 0;
    spriteYpos = OAMmem[0];
    tempBitBuffer2 = OAMmem[1];
    if(spriteSize == 1)
    {
        tempBitBuffer2[0] == 0;
    }
    spritePatIndex = tempBitBuffer2.to_ulong() << 4;
    tempBitBuffer = OAMmem[1];
    if(sprite1000 == 1 && spriteSize == 0) // If the bank 1000 bit is set in sprite, use 0x1000 sprites instead.
    {
        spritePatIndex += 0x1000;
    }
    if(tempBitBuffer[0] == 1 && spriteSize == 1)
    {
        spritePatIndex += 0x1000;
    }
    flipBitBuffer = OAMmem[2];
    spriteAttributes = OAMmem[2];
    spriteXpos = OAMmem[3];
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
    while(currentSprite != 0x100)
    {
        if(flipBitBuffer[6] == 0 && flipBitBuffer[7] == 0)
        {
            renderSprite();
            if(spriteSize == 1)
            {
                xpixS = spriteXpos;
                help1 = spriteXpos + 8;
                help2 = spriteYpos + 16;
                spritePatIndex += 8;
                ypixS = spriteYpos + 8;
                spriteYpos += 8;
                bitcountS = 7;

                renderSprite();
            }
        }
        if(flipBitBuffer[6] == 1 && flipBitBuffer[7] == 0)
        {
            renderHFLIPSprite();
            if(spriteSize == 1)
            {
                xpixS = spriteXpos + 8;
                help1 = spriteXpos;
                help2 = spriteYpos + 16;
                spritePatIndex += 8;
                ypixS = spriteYpos + 8;
                spriteYpos += 8;
                bitcountS = 7;
                renderHFLIPSprite();
            }
        }
        if(flipBitBuffer[6] == 0 && flipBitBuffer[7] == 1)
        {
            renderVFLIPSprite();
            if(spriteSize == 1)
            {
                xpixS = spriteXpos;
                help1 = spriteXpos + 8;
                help2 = spriteYpos + 16;
                spritePatIndex += 10;
                ypixS = spriteYpos + 8;
                spriteYpos += 8;
                bitcountS = 7;
                renderVFLIPSprite();
            }
        }
        if(flipBitBuffer[6] == 1 && flipBitBuffer[7] == 1)
        {
            renderHVFLIPSprite();
            if(spriteSize == 1)
            {
                xpixS = spriteXpos + 8;
                help1 = spriteXpos;
                help2 = spriteYpos + 16;
                spritePatIndex += 10;
                ypixS = spriteYpos + 8;
                spriteYpos += 8;
                bitcountS = 7;
                renderHVFLIPSprite();
            }
        }
        currentSprite += 4;
        if(currentSprite == 0xFC)
        {
            //printf("wowie\n");
            return 0;
        }
        spriteYpos = OAMmem[currentSprite];
        tempBitBuffer = OAMmem[currentSprite + 1];
        //spritePatIndex = OAMmem[currentSprite + 1] << 4;
        tempBitBuffer2 = OAMmem[currentSprite + 1];
        if(spriteSize == 1)
        {
            tempBitBuffer2[0] == 0;
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
        spriteXpos = OAMmem[currentSprite + 3];
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
int handleGraphicsBASICSCAN()
{
    ppurendercount = 2;
    nametableAddr2 = 0x2000;
    if(nametableuse == 1 || nametableuse == 3)
    {
        nametableAddr2 = 0x2400;
    }
    while(ppurendercount != 0)
    {
    uint8_t Xcounter = 32;
    bitcount = 7;
    tiledatalocation1 = nametableAddr2 + (NESOB.scanline / 8) * 32;
    tilelocation = NESOB.PPUmemory[tiledatalocation1];
    tilecount = tilelocation << 4;
    tilecount += NESOB.scanline % 8;
    xpix = 0;
    ypix = NESOB.scanline;
    xpixtile = 0;
    ypixtile = 0;
    xpixtile2 = 0;
    ypixtile2 = 0;
    if(bgpattable == true)
    {
        tilecount += 0x1000;
    }
        while(Xcounter != 0)
        {
                graphicline = NESOB.PPUmemory[tilecount];
                graphicline2 = NESOB.PPUmemory[tilecount + 8];
                while(xpixtile2 != 8)
                {
                    chooseRenderColorREWRITE();
                    xpixsc = xpix - scrollx;
                    curPixx = xpixsc;
                    if(nametableAddr2 == 0x2400 && nametableuse == 0)
                    {
                        curPixx = curPixx + 256;
                    }
                    if(nametableAddr2 == 0x2000 && nametableuse == 1)
                    {
                        curPixx = curPixx + 256;
                    }
                    bit916convert = curPixx;
                    curPixx = bit916convert.to_ulong();
                    if(NESOB.scanline <= 255)
                    {
                        curPixx += ypix * 512;
                    }
                    else
                    {
                        curPixx += 255 * 512;
                    }
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
                    xpix++;
                    bitcount--;
                    xpixtile2++;
                }
            xpixtile++;
            Xcounter--;
            xpixtile2 = 0;
            tiledatalocation1++;
            bitcount = 7;
            tilelocation = NESOB.PPUmemory[tiledatalocation1];
            tilecount = tilelocation << 4;
            tilecount = tilecount + NESOB.scanline % 8;
            if(bgpattable == true)
            {
                tilecount += 0x1000;
            }

        }
        currentnametable = 0;
        ppurendercount -= 1;
        nametableAddr2 = 0x2400;
        if(nametableuse == 1 || nametableuse == 3)
        {
            nametableAddr2 = 0x2000;
        }
    }
    if(NESOB.scanline == 260)
    {
    mainScreen.w = 512;
    mainScreen.h = 256;
    mainScreen.x = 0;
    mainScreen.y = 0;
    SDL_UpdateTexture(textureScanline, NULL, pixels, blitsu);
    SDL_RenderCopy(renderer, textureScanline, NULL, &mainScreen);
    handleSprites();
    SDL_RenderPresent(renderer);
    currentPPUFrame++;
    SDL_SetRenderDrawColor(renderer,0x00,0x00,0xFF,0xFF);
    SDL_RenderClear(renderer);
    }

    //printf("ScrollX: 0x%X\n",scrollx);
    //printf("ScrollY: 0x%X\n",scrolly);
    //printf("Current PPU Frame: %i\n",currentPPUFrame);
    /*
    while(true)
    {
        if(newFrame == true)
        {
            newFrame = false;
            handleGraphicsBASICSCAN();
        }
        if(exitLoop == true)
        {
            return 0;
        }
    }
    */
    return 0;
}
