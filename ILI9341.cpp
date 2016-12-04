#include "ILI9341.h"
#include <unistd.h>
#include <cstring>

static uint8_t init_commands[] =
{
    4, 0xEF, 0x03, 0x80, 0x02,
    4, 0xCF, 0x00, 0XC1, 0X30,
    5, 0xED, 0x64, 0x03, 0X12, 0X81,
    4, 0xE8, 0x85, 0x00, 0x78,
    6, 0xCB, 0x39, 0x2C, 0x00, 0x34, 0x02,
    2, 0xF7, 0x20,
    3, 0xEA, 0x00, 0x00,
    2, ILI9341_PWCTR1, 0x23, // Power control
    2, ILI9341_PWCTR2, 0x10, // Power control
    3, ILI9341_VMCTR1, 0x3e, 0x28, // VCM control
    2, ILI9341_VMCTR2, 0x86, // VCM control2
    2, ILI9341_MADCTL, 0x48, // Memory Access Control
    2, ILI9341_PIXFMT, 0x55,
    3, ILI9341_FRMCTR1, 0x00, 0x18,
    4, ILI9341_DFUNCTR, 0x08, 0x82, 0x27, // Display Function Control
    2, 0xF2, 0x00, // Gamma Function Disable
    2, ILI9341_GAMMASET, 0x01, // Gamma curve selected
    16, ILI9341_GMCTRP1, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08,
    0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00, // Set Gamma
    16, ILI9341_GMCTRN1, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07,
    0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F, // Set Gamma
    0
};


ILI9341::ILI9341(const char* DEVICE, int DC, int RST)
    :spi(DEVICE), dc(DC), rst(RST)
{
    //verify rotation
    width = ILI9341_TFTWIDTH;
    height = ILI9341_TFTHEIGHT;
    //ctor
}

void ILI9341::begin(bool reset)
{
    spi.begin();
    rst.Write(GPIO::HIGH);
    dc.Write(GPIO::LOW);
    if (reset)
    {
        // VDD (3.3V) goes high at start, lets just chill for a ms
        usleep(1000);
        // bring reset low
        rst.Write(GPIO::LOW);
        // wait 10ms
        usleep(10000);
        // bring out of reset
        rst.Write(GPIO::HIGH);
        // turn on VCC (9V?)
    }
    /*command(init_commands, sizeof(init_commands));
    command((uint8_t)ILI9341_SLPOUT);    // Exit Sleep
    usleep(120000);
    command((uint8_t)ILI9341_DISPON);    // Display on
    usleep(120000);*/
    command(0xEF);
    data(0x03);
    data(0x80);
    data(0x02);

    command(0xCF);
    data(0x00);
    data(0XC1);
    data(0X30);

    command(0xED);
    data(0x64);
    data(0x03);
    data(0X12);
    data(0X81);

    command(0xE8);
    data(0x85);
    data(0x00);
    data(0x78);

    command(0xCB);
    data(0x39);
    data(0x2C);
    data(0x00);
    data(0x34);
    data(0x02);

    command(0xF7);
    data(0x20);

    command(0xEA);
    data(0x00);
    data(0x00);

    command(ILI9341_PWCTR1);    //Power control
    data(0x23);   //VRH[5:0]

    command(ILI9341_PWCTR2);    //Power control
    data(0x10);   //SAP[2:0];BT[3:0]

    command(ILI9341_VMCTR1);    //VCM control
    data(0x3e); //¶Ô±È¶Èµ÷½Ú
    data(0x28);

    command(ILI9341_VMCTR2);    //VCM control2
    data(0x86);  //--

    command(ILI9341_MADCTL);    // Memory Access Control
    data(0x48);

    command(ILI9341_PIXFMT);
    data(0x55);

    command(ILI9341_FRMCTR1);
    data(0x00);
    data(0x18);

    command(ILI9341_DFUNCTR);    // Display Function Control
    data(0x08);
    data(0x82);
    data(0x27);

    command(0xF2);    // 3Gamma Function Disable
    data(0x00);

    command(ILI9341_GAMMASET);    //Gamma curve selected
    data(0x01);

    command(ILI9341_GMCTRP1);    //Set Gamma
    data(0x0F);
    data(0x31);
    data(0x2B);
    data(0x0C);
    data(0x0E);
    data(0x08);
    data(0x4E);
    data(0xF1);
    data(0x37);
    data(0x07);
    data(0x10);
    data(0x03);
    data(0x0E);
    data(0x09);
    data(0x00);

    command(ILI9341_GMCTRN1);    //Set Gamma
    data(0x00);
    data(0x0E);
    data(0x14);
    data(0x03);
    data(0x11);
    data(0x07);
    data(0x31);
    data(0xC1);
    data(0x48);
    data(0x08);
    data(0x0F);
    data(0x0C);
    data(0x31);
    data(0x36);
    data(0x0F);

    command(ILI9341_SLPOUT);    //Exit Sleep
    usleep(120000);
    command(ILI9341_DISPON);
    usleep(120000);
}

void ILI9341::fillScreen(uint16_t color)
{
    uint8_t _buffer[SCANLINE_PIXEL_COUNT*2];
    uint8_t hi = color >> 8, lo = color & 0xFF;
    for(int i = 0; i < SCANLINE_PIXEL_COUNT*2;i+=2)
    {
        _buffer[i] = hi;
        _buffer[i+1] = lo;
    }

    //memset(_scanline16,color, SCANLINE_PIXEL_COUNT);
    //uint8_t hi = color >> 8, lo = color;

    setAddrAndRW_cont(0,0, width, height);
    dc.Write(GPIO::HIGH);
    for(int x = 0; x < 240; ++x)
    {
        for(int y = 0; y < 640; y+=64)
        spi.transfer(&_buffer[y], 64);
    }
}


void ILI9341::command(uint8_t* c, int size)
{
    dc.Write(GPIO::LOW);
    spi.transfer(c, size);
}

void ILI9341::command(uint8_t c)
{
    dc.Write(GPIO::LOW);
    spi.transfer(c);
}

void ILI9341::command16(uint16_t c)
{
    dc.Write(GPIO::LOW);
    spi.transfer(c >> 8);
    spi.transfer(c & 0xFF);
}

void ILI9341::data(uint8_t* c, int size)
{
    dc.Write(GPIO::HIGH);
    spi.transfer(c, size);
}

void ILI9341::data(uint8_t c)
{
    dc.Write(GPIO::HIGH);
    spi.transfer(c);
}

void ILI9341::data16(uint16_t c)
{
    dc.Write(GPIO::HIGH);
    spi.transfer(c >> 8);
    spi.transfer(c & 0xFF);
}

void ILI9341::data16(uint16_t* c, int size)
{
    uint8_t ndata[size*2];
    for(int i = 0; i < size*2;)
    {
        ndata[i] = (c[i] >> 8);
        i++;
        ndata[i] = (c[i] & 0xFF);
        i++;
    }
    dc.Write(GPIO::HIGH);
    for(int i=0; i < 320; i+=8)
    spi.transfer(&ndata[i], 8);
    //data(ndata, size);
    //data(ndata+size, size);
}


void ILI9341::setAddrAndRW_cont(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    command(ILI9341_CASET); // Column addr set
    data(x >> 8);
    data(x & 0xFF);     // XSTART
    data(w >> 8);
    data(w & 0xFF);     // XEND

    command(ILI9341_PASET); // Row addr set
    data(y>>8);
    data(y);     // YSTART
    data(h>>8);
    data(h);     // YEND

    command(ILI9341_RAMWR); // write to RAM
}


ILI9341::~ILI9341()
{
    //dtor
}
