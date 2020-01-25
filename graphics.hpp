#include "include.hpp"
uint8_t bitcount;
std::bitset<8> graphicline;
std::bitset<8> graphicline2;
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
uint8_t xpix;
uint8_t ypix;
uint8_t tilelocation;
uint16_t tiledatalocation1;
bool renderThrottle;
int pal00[4] = {0xFF,0xFF,0xFF,0x00};
int pal01[4] = {0xC2,0xC2,0xC2,0x00};
int pal10[4] = {0x76,0x76,0x76,0x00};
int pal11[4] = {0x00,0x00,0x00,0x00};
SDL_Color color_00;
SDL_Color color_01;
SDL_Color color_02;
SDL_Color color_03;
uint8_t pallete[192];
uint8_t bgcol;
uint8_t tilecounteback;
void handleGrid()
{
    currentgridx = xpixtile / 4;
    currentgridy = ypixtile / 4;
    currentGrid = (currentgridx * 1) + (currentgridy * 8);
    //printf("curgri: %i\n",currentGrid);
}
void handleBlock()
{
    //printf("xpixtile: %i\n",xpixtile);
    currentblockx = xpixtile % 2;
    currentblocky = ypixtile % 2;
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
    //printf("curblo: %i\n",currentBlock);
}
uint16_t palLocate;
std::bitset<8> currentGridat;
std::bitset<2> currentBlockat;
void palDetermine()
{
    currentGridat = NESOB.PPUmemory[(0x23C0) + currentGrid];
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
void chooseRenderColor()
{
    handleGrid();
    handleBlock();
    palDetermine();
    if(graphicline[bitcount] == 0 && graphicline2[bitcount] == 0)
    {
        SDL_SetRenderDrawColor(renderer,color_00.r,color_00.g,color_00.b,color_00.a);
    }
    if(graphicline[bitcount] == 1 && graphicline2[bitcount] == 0)
    {
        SDL_SetRenderDrawColor(renderer,color_01.r,color_01.g,color_01.b,color_01.a);
    }
    if(graphicline[bitcount] == 0 && graphicline2[bitcount] == 1)
    {
        SDL_SetRenderDrawColor(renderer,color_02.r,color_02.g,color_02.b,color_02.a);
    }
    if(graphicline[bitcount] == 1 && graphicline2[bitcount] == 1)
    {
        SDL_SetRenderDrawColor(renderer,color_03.r,color_03.g,color_03.b,color_03.a);
    }
}
bool tempfixrender;
uint8_t ypix2;
int handleGraphicsBASIC()
{
    bitcount = 7;
    tiledatalocation1 = nametableAddr;
    tilelocation = NESOB.PPUmemory[tiledatalocation1];
    tilelocation++;
    tilecount = tilelocation << 4;
    xpix = 0;
    ypix = 0;
    xpixtile = 0;
    ypixtile = 0;
    xpixtile2 = 0;
    ypixtile2 = 0;
    SDL_SetRenderDrawColor(renderer,0x00,0x00,0xFF,0xFF);
    SDL_RenderClear(renderer);
    while(ypixtile != 32)
    {
        while(xpixtile != 32)
        {
            while(ypixtile2 != 8)
            {
                graphicline = NESOB.PPUmemory[tilecount];
                graphicline2 = NESOB.PPUmemory[tilecount + 8];
                while(xpixtile2 != 8)
                {
                    chooseRenderColor();
                    if(showGrid == true && xpix % 32 == 0 || showGrid == true && ypix % 32 == 0)
                    {
                        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
                    }
                    if(showBlock == true && xpix % 16 == 0 || showBlock == true && ypix % 16 == 0)
                    {
                        SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
                    }
                    if(showTile == true && xpix % 8 == 0 || showTile == true && ypix % 8 == 0)
                    {
                        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
                    }
                    ypix2 = ypix;
                    //if(scrolly < 240)
                   // {
                        SDL_RenderDrawPoint(renderer,xpix,ypix2); // Middle
                        /*
                        SDL_RenderDrawPoint(renderer,(xpix - scrollx) + 0x100,(ypix2 - scrolly)); // Right
                        SDL_RenderDrawPoint(renderer,(xpix - scrollx) + 0x100,(ypix2 - scrolly) + 0x100); // Bottom-Right
                        SDL_RenderDrawPoint(renderer,(xpix - scrollx),(ypix2 - scrolly) + 0x100); // Bottom
                        SDL_RenderDrawPoint(renderer,(xpix - scrollx) - 0x100,(ypix2 - scrolly) + 0x100); // Bottom Left
                        SDL_RenderDrawPoint(renderer,(xpix - scrollx) - 0x100,(ypix2 - scrolly)); // Left
                        SDL_RenderDrawPoint(renderer,(xpix - scrollx) - 0x100,(ypix2 - scrolly) - 0x100); // Top Left
                        SDL_RenderDrawPoint(renderer,(xpix - scrollx),(ypix2 - scrolly) - 0x100); // Top
                        SDL_RenderDrawPoint(renderer,(xpix - scrollx) + 0x100,(ypix2 - scrolly) - 0x100); // Top Right

                    }
                    */
                    /*
                    if(scrolly >= 240)
                    {
                        scrolly2 = scrolly - 239;
                        SDL_RenderDrawPoint(renderer,xpix - scrollx,ypix2 + scrolly2); // Middle
                        SDL_RenderDrawPoint(renderer,(xpix - scrollx) + 0x100,(ypix2 + scrolly2)); // Right
                        SDL_RenderDrawPoint(renderer,(xpix - scrollx) + 0x100,(ypix2 + scrolly2) + 0x100); // Bottom-Right
                        SDL_RenderDrawPoint(renderer,(xpix - scrollx),(ypix2 + scrolly2) + 0x100); // Bottom
                        SDL_RenderDrawPoint(renderer,(xpix - scrollx) - 0x100,(ypix2 + scrolly2) + 0x100); // Bottom Left
                        SDL_RenderDrawPoint(renderer,(xpix - scrollx) - 0x100,(ypix2 + scrolly2)); // Left
                        SDL_RenderDrawPoint(renderer,(xpix - scrollx) - 0x100,(ypix2 + scrolly2) - 0x100); // Top Left
                        SDL_RenderDrawPoint(renderer,(xpix - scrollx),(ypix2 + scrolly2) - 0x100); // Top
                        SDL_RenderDrawPoint(renderer,(xpix - scrollx) + 0x100,(ypix2 + scrolly2) - 0x100); // Top Right
                    }
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
            ypix = (ypixtile * 8);
            xpix = (xpixtile * 8);
            xpixtile2 = 0;
            ypixtile2 = 0;
            tiledatalocation1++;
            tilelocation = NESOB.PPUmemory[tiledatalocation1];
            tilelocation += 1;
            //printf("tilelocation: 0x%X\n",tilelocation);
            tilecount = tilelocation << 4;
            if(bgpattable == true)
            {
                tilecount += 0x1000;
            }
        }
        xpixtile = 0x00;
        ypixtile++;
        ypix = (ypixtile * 8);
        xpix = (xpixtile * 8);
    }

    SDL_RenderPresent(renderer);
    while(true)
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
}
