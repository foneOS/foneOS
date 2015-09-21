#include "stdafx.h"
#include "Display.h"

/*
 * This file is based off of the Adafruit_ILI9341 library, available here: https://github.com/adafruit/Adafruit_ILI9341
 * The license header of that library is as follows:
*/
/***************************************************
  This is our library for the Adafruit ILI9341 Breakout and Shield
  ----> http://www.adafruit.com/products/1651
  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!
  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

/* TODO: Right now, it's assumed that the grayscale renderer for FreeType
supports 256 shades of gray, but this should instead key off of num_grays
in the FT_Bitmap after the FT_Render_Glyph() call. */
#define NUM_GRAYS 256

#ifdef EDISON
#define LOW 0
#define HIGH 1

#define INPUT MRAA_GPIO_IN
#define OUTPUT MRAA_GPIO_OUT

//Measured ADC values for (0,0) and (210-1,320-1)
//TS_MINX corresponds to ADC value when X = 0
//TS_MINY corresponds to ADC value when Y = 0
//TS_MAXX corresponds to ADC value when X = 240 -1
//TS_MAXY corresponds to ADC value when Y = 320 -1

#define TS_MINX 140
#define TS_MAXX 900

#define TS_MINY 120
#define TS_MAXY 940

void Display_ILI9341::Init()
{
    this->pins = std::map<unsigned int, mraa_gpio_context>();

    // hardware SPI and pinout is always assumed
    _cs   = 10;//cs;
    _dc   = 9;//dc;
    _rst  = 0;

    // TODO: Error handling
    _spi = mraa_spi_init(1);

    if (_rst != 0) {
        this->SetPin(_rst, OUTPUT);
        this->DigitalWrite(_rst, LOW);
    }

    this->SetPin(_dc, OUTPUT);
    this->SetPin(_cs, OUTPUT);

    if (_rst > 0) {
        this->DigitalWrite(_rst, HIGH);
        Utils::Delay(5);
        this->DigitalWrite(_rst, LOW);
        Utils::Delay(20);
        this->DigitalWrite(_rst, HIGH);
        Utils::Delay(150);
    }

    this->SPIBegin();
    this->WriteCommand(0xEF);
    this->WriteData(0x03);
    this->WriteData(0x80);
    this->WriteData(0x02);

    this->WriteCommand(0xCF);
    this->WriteData(0x00);
    this->WriteData(0XC1);
    this->WriteData(0X30);

    this->WriteCommand(0xED);
    this->WriteData(0x64);
    this->WriteData(0x03);
    this->WriteData(0X12);
    this->WriteData(0X81);

    this->WriteCommand(0xE8);
    this->WriteData(0x85);
    this->WriteData(0x00);
    this->WriteData(0x78);

    this->WriteCommand(0xCB);
    this->WriteData(0x39);
    this->WriteData(0x2C);
    this->WriteData(0x00);
    this->WriteData(0x34);
    this->WriteData(0x02);

    this->WriteCommand(0xF7);
    this->WriteData(0x20);

    this->WriteCommand(0xEA);
    this->WriteData(0x00);
    this->WriteData(0x00);

    this->WriteCommand(ILI9341_PWCTR1);    //Power control
    this->WriteData(0x23);   //VRH[5:0]

    this->WriteCommand(ILI9341_PWCTR2);    //Power control
    this->WriteData(0x10);   //SAP[2:0];BT[3:0]

    this->WriteCommand(ILI9341_VMCTR1);    //VCM control
    this->WriteData(0x3e);
    this->WriteData(0x28);

    this->WriteCommand(ILI9341_VMCTR2);    //VCM control2
    this->WriteData(0x86);  //--

    this->WriteCommand(ILI9341_MADCTL);    // Memory Access Control
    this->WriteData(0x48);

    this->WriteCommand(ILI9341_PIXFMT);
    this->WriteData(0x55);

    this->WriteCommand(ILI9341_FRMCTR1);
    this->WriteData(0x00);
    this->WriteData(0x18);

    this->WriteCommand(ILI9341_DFUNCTR);    // Display Function Control
    this->WriteData(0x08);
    this->WriteData(0x82);
    this->WriteData(0x27);

    this->WriteCommand(0xF2);    // 3Gamma Function Disable
    this->WriteData(0x00);

    this->WriteCommand(ILI9341_GAMMASET);    //Gamma curve selected
    this->WriteData(0x01);

    this->WriteCommand(ILI9341_GMCTRP1);    //Set Gamma
    this->WriteData(0x0F);
    this->WriteData(0x31);
    this->WriteData(0x2B);
    this->WriteData(0x0C);
    this->WriteData(0x0E);
    this->WriteData(0x08);
    this->WriteData(0x4E);
    this->WriteData(0xF1);
    this->WriteData(0x37);
    this->WriteData(0x07);
    this->WriteData(0x10);
    this->WriteData(0x03);
    this->WriteData(0x0E);
    this->WriteData(0x09);
    this->WriteData(0x00);

    this->WriteCommand(ILI9341_GMCTRN1);    //Set Gamma
    this->WriteData(0x00);
    this->WriteData(0x0E);
    this->WriteData(0x14);
    this->WriteData(0x03);
    this->WriteData(0x11);
    this->WriteData(0x07);
    this->WriteData(0x31);
    this->WriteData(0xC1);
    this->WriteData(0x48);
    this->WriteData(0x08);
    this->WriteData(0x0F);
    this->WriteData(0x0C);
    this->WriteData(0x31);
    this->WriteData(0x36);
    this->WriteData(0x0F);

    this->WriteCommand(ILI9341_SLPOUT);    //Exit Sleep
    this->SPIEnd();

    Utils::Delay(120);

    this->SPIBegin();
    this->WriteCommand(ILI9341_DISPON);    //Display on
    this->SPIEnd();

    uint8_t x = this->readcommand8(ILI9341_RDMODE);
    printf("%02X\n", x);
    x = this->readcommand8(ILI9341_RDMADCTL);
    printf("%02X\n", x);
    x = this->readcommand8(ILI9341_RDPIXFMT);
    printf("%02X\n", x);
    x = this->readcommand8(ILI9341_RDIMGFMT);
    printf("%02X\n", x);
    x = this->readcommand8(ILI9341_RDSELFDIAG);
    printf("%02X\n", x);
}

void Display_ILI9341::Clear(FoneOSColor color)
{
    this->FillRectangle(0, 0, this->GetWidth(), this->GetHeight(), color);
}

void Display_ILI9341::DrawRectangle(int x, int y, int w, int h, FoneOSColor color)
{
    this->DrawHorizontalLine(x, y, w, color);
    this->DrawVerticalLine(x, y, h, color);

    this->DrawHorizontalLine(x, y + h, w, color);
    this->DrawVerticalLine(x + w, y, h, color);
}

void Display_ILI9341::FillRectangle(int x, int y, int w, int h, FoneOSColor color)
{
    // rudimentary clipping (drawChar w/big text requires this)
    if ((x >= this->GetWidth()) || (y >= this->GetHeight()))
    {
        return;
    }
    if ((x + w - 1) >= this->GetWidth())
    {
        w = this->GetWidth() - x;
    }
    if ((y + h - 1) >= this->GetHeight())
    {
        h = this->GetHeight() - y;
    }

    this->SPIBegin();
    this->SetAddrWindow(x, y, x+w-1, y+h-1);

    uint8_t hi = this->CreateColor(color) >> 8, lo = this->CreateColor(color);

    this->DigitalWrite(_dc, HIGH);
    this->DigitalWrite(_cs, LOW);

    for(y=h; y>0; y--) {
        for(x=w; x>0; x--) {
            this->SPIWrite(hi);
            this->SPIWrite(lo);
        }
    }
    this->DigitalWrite(_cs, HIGH);

    this->SPIEnd();
}


unsigned int ibmpWidth, ibmpHeight;
uint8_t ibmpDepth, ibmpImageoffset;

uint8_t Display_ILI9341::readByte(std::ifstream * f)
{
    char buffer[2];
    f->read(buffer, 1);
    return buffer[0];
}

uint16_t Display_ILI9341::read16(std::ifstream * f)
{
    uint16_t d;
    uint8_t b;
    b = readByte(f);
    d = readByte(f);
    d <<= 8;
    d |= b;
    return d;
}

// LITTLE ENDIAN!
uint32_t Display_ILI9341::read32(std::ifstream * f)
{
    uint32_t d;
    uint16_t b;

    b = read16(f);
    d = read16(f);
    d <<= 16;
    d |= b;
    return d;
}

bool Display_ILI9341::bmpReadHeader(std::ifstream * f)
{
    // read header
    uint32_t tmp;

    if (read16(f) != 0x4D42)
    {
        // magic bytes missing
        return false;
    }

    // read file size
    tmp = read32(f);

    // read and ignore creator bytes
    read32(f);

    ibmpImageoffset = read32(f);

    // read DIB header
    tmp = read32(f);
    ibmpWidth = read32(f);
    ibmpHeight = read32(f);


    if (read16(f) != 1)
        return false;

    ibmpDepth = read16(f);

    if (read32(f) != 0)
    {
        // compression not supported!
        return false;
    }

    return true;
}

#define BUFFPIXEL 20

void Display_ILI9341::bmpdraw(std::ifstream * bmpFile, int x, int y)
{
    //this->SetOrientation(0);
    bmpFile->seekg(ibmpImageoffset);

    uint16_t p;
    uint8_t g, b;
    unsigned int i, j;

    char sdbuffer[3 * BUFFPIXEL];  // 3 * pixels to buffer
    uint8_t buffidx = 3*BUFFPIXEL;

    for (i=0; i< ibmpHeight; i++)
    {
        int yayPos = y+(ibmpHeight-i);
        std::cout << yayPos << std::endl;
        this->SetAddrWindow(x, yayPos, ibmpWidth, ibmpHeight-i);

        for (j=0; j<ibmpWidth; j++) {
            // read more pixels
            if (buffidx >= 3*BUFFPIXEL) {
                bmpFile->read(sdbuffer, 3*BUFFPIXEL);
                buffidx = 0;
            }

            // convert pixel from 888 to 565
            b = sdbuffer[buffidx++];     // blue
            g = sdbuffer[buffidx++];     // green
            p = sdbuffer[buffidx++];     // red

            p >>= 3;
            p <<= 6;

            g >>= 2;
            p |= g;
            p <<= 5;

            b >>= 3;
            p |= b;

            // write out the 16 bits of color
            this->PushColor(p);
        }
    }
}

bool Display_ILI9341::DrawImage(FoneOSString filename, int x, int y)
{
    FoneOSString filePathStr = Storage::GetFullPath(filename);
    char * filePath = Utils::FoneOSStringToCharArray(filePathStr);

    std::ifstream bmpFile;
    bmpFile.open(filePath);

    if (!bmpReadHeader(&bmpFile))
    {
        Logging::LogMessage(STR("bad bmp"));
        return false;
    }

    this->bmpdraw(&bmpFile, x, y);

    bmpFile.close();

    return true;
}

void Display_ILI9341::DrawString(FoneOSString string, int x, int y, FoneFontDesc font, int size, FoneOSColor color, FoneOSColor bg)
{
    int HEIGHT, WIDTH = 0;
    std::vector<std::vector<unsigned char>> image = Type::GetBitmap(font, size, string, &WIDTH, &HEIGHT);
    int count = 0;
    int lastPerc = 0;
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            unsigned char imgColor = (image[i])[j];
            if (imgColor == 0)
            {
                continue;
            }
            uint8_t rdiff = abs(color.r - bg.r);
            uint8_t gdiff = abs(color.g - bg.g);
            uint8_t bdiff = abs(color.b - bg.b);

            bool colorIsBlack = (color.r == 0) && (color.g == 0) && (color.b == 0);

            FoneOSColor drawColor = color;
            drawColor.r = bg.r + (imgColor*rdiff) / (colorIsBlack ? 1 : (NUM_GRAYS - 1));
            drawColor.g = bg.g + (imgColor*gdiff) / (colorIsBlack ? 1 : (NUM_GRAYS - 1));
            drawColor.b = bg.b + (imgColor*bdiff) / (colorIsBlack ? 1 : (NUM_GRAYS - 1));

            this->FillRectangle(x + j, y + i, 1, 1, drawColor);// { 255 - color, 255 - color, 255 - color, 255 });
            count++;
        }
    }
}

void Display_ILI9341::DrawString(FoneOSString string, int x, int y, int size, FoneOSColor color, FoneOSColor bg)
{
    this->DrawString(string, x, y, DEFAULT_FONT, size * 12, color, bg);
}

void Display_ILI9341::Flush()
{
    // Draw methods don't need to be flushed; they are drawn immediately.
}

void Display_ILI9341::Update()
{
    // Does nothing so far
}

void Display_ILI9341::Cleanup()
{
    // TODO: destroy MRAA contexts
}

int Display_ILI9341::GetHorizDPI()
{
    return 72;
}

int Display_ILI9341::GetVertDPI()
{
    return 72;
}

int Display_ILI9341::GetWidth()
{
    return 240;
}

int Display_ILI9341::GetHeight()
{
    return 320;
}

void Display_ILI9341::DrawHorizontalLine(unsigned int x, unsigned int y, unsigned int length, FoneOSColor color)
{
    // Rudimentary clipping
    if((x >= this->GetWidth()) || (y >= this->GetHeight()))
    {
        return;
    }

    if((x+length-1) >= this->GetWidth())
    {
        length = this->GetWidth()-x;
    }

    this->SPIBegin();
    this->SetAddrWindow(x, y, x+length-1, y);

    uint8_t hi = this->CreateColor(color) >> 8, lo = this->CreateColor(color);
    this->DigitalWrite(_dc, HIGH);
    this->DigitalWrite(_cs, LOW);
    while (length--)
    {
        this->SPIWrite(hi);
        this->SPIWrite(lo);
    }
    this->DigitalWrite(_cs, HIGH);
    this->SPIEnd();
}

void Display_ILI9341::DrawVerticalLine(unsigned int x, unsigned int y, unsigned int length, FoneOSColor color)
{
    // Rudimentary clipping
    if((x >= this->GetWidth()) || (y >= this->GetHeight()))
    {
        return;
    }

    if((y+length-1) >= this->GetHeight())
    {
        length = this->GetHeight()-y;
    }

    this->SPIBegin();
    this->SetAddrWindow(x, y, x, y+length-1);

    uint8_t hi = this->CreateColor(color) >> 8, lo = this->CreateColor(color);

    this->DigitalWrite(_dc, HIGH);
    this->DigitalWrite(_cs, LOW);

    while (length--)
    {
        this->SPIWrite(hi);
        this->SPIWrite(lo);
    }

    this->DigitalWrite(_cs, HIGH);

    this->SPIEnd();
}

uint16_t Display_ILI9341::CreateColor(FoneOSColor color)
{
    return ((color.r & 0xF8) << 8 | ((color.g & 0xFC) << 3) | (color.b >> 3));
}

void Display_ILI9341::SPIBegin()
{
    // MRAA doesn't seem to have SPI transaction support, so this function doesn't do anything except set mode parameters.
    mraa_spi_frequency(_spi, 8000000);
    mraa_spi_lsbmode(_spi, false);
    mraa_spi_mode(_spi, MRAA_SPI_MODE0);
}

void Display_ILI9341::SPIEnd()
{
    // See the comment for SPIBegin()
}

void Display_ILI9341::SetPin(unsigned int pin, mraa_gpio_dir_t dir)
{
    if (this->pins.find(pin) == this->pins.end())
    {
        this->pins[pin] = mraa_gpio_init(pin);
        mraa_gpio_use_mmaped(this->pins[pin], 1);
    }
    mraa_gpio_dir(this->pins[pin], dir);
}

void Display_ILI9341::DigitalWrite(unsigned int pin, unsigned int value)
{
    mraa_gpio_write(this->pins[pin], value);
}

void Display_ILI9341::SetAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    this->WriteCommand(ILI9341_CASET); // Column addr set
    this->WriteData(x0 >> 8);
    this->WriteData(x0 & 0xFF);     // XSTART
    this->WriteData(x1 >> 8);
    this->WriteData(x1 & 0xFF);     // XEND

    this->WriteCommand(ILI9341_PASET); // Row addr set
    this->WriteData(y0>>8);
    this->WriteData(y0);     // YSTART
    this->WriteData(y1>>8);
    this->WriteData(y1);     // YEND

    this->WriteCommand(ILI9341_RAMWR); // write to RAM
}

void Display_ILI9341::PushColor(uint16_t color)
{
    this->SPIBegin();
    this->DigitalWrite(_dc, HIGH);
    this->DigitalWrite(_cs, LOW);

    this->SPIWrite(color >> 8);
    this->SPIWrite(color);

    this->DigitalWrite(_cs, HIGH);
    this->SPIEnd();
}

void Display_ILI9341::SPIWrite(uint8_t c)
{
    //_spi = mraa_spi_init(0);
    mraa_spi_write(_spi, c);
}

uint8_t Display_ILI9341::SPIRead()
{
    /*uint8_t rxbuf = 0x00;
    uint8_t txbuf = 0x00;
    mraa_spi_transfer_buf(_spi, &txbuf, &rxbuf, 1);*/
    return mraa_spi_write(_spi, 0x00);
}

uint8_t Display_ILI9341::readcommand8(uint8_t c, uint8_t index)
{
    this->SPIBegin();

    this->DigitalWrite(_dc, LOW); // command
    this->DigitalWrite(_cs, LOW);
    this->SPIWrite(0xD9);  // woo sekret command?
    this->DigitalWrite(_dc, HIGH); // data
    this->SPIWrite(0x10 + index);
    this->DigitalWrite(_cs, HIGH);

    this->DigitalWrite(_dc, LOW);
    //this->DigitalWrite(_sclk, LOW);
    this->DigitalWrite(_cs, LOW);
    this->SPIWrite(c);

    this->DigitalWrite(_dc, HIGH);
    uint8_t r = this->SPIRead();
    this->DigitalWrite(_cs, HIGH);

    this->SPIEnd();
    return r;
}

void Display_ILI9341::WriteCommand(uint8_t c)
{
    this->DigitalWrite(_dc, LOW);

    //*clkport &= ~clkpinmask; // clkport is a NULL pointer when hwSPI==true
    //digitalWrite(_sclk, LOW);

    this->DigitalWrite(_cs, LOW);

    this->SPIWrite(c);

    this->DigitalWrite(_cs, HIGH);
}

void Display_ILI9341::WriteData(uint8_t c)
{
    this->DigitalWrite(_dc, HIGH);

    //*clkport &= ~clkpinmask; // clkport is a NULL pointer when hwSPI==true
    //digitalWrite(_sclk, LOW);

    this->DigitalWrite(_cs, LOW);

    this->SPIWrite(c);

    this->DigitalWrite(_cs, HIGH);
}

/*void Display_ILI9341::setRotation(uint8_t m)
{
    this->SPIBegin();

    writecommand(ILI9341_MADCTL);
    rotation = m % 4; // can't be higher than 3
    switch (rotation) {
        case 0:
            writedata(MADCTL_MX | MADCTL_BGR);
            _width  = ILI9341_TFTWIDTH;
            _height = ILI9341_TFTHEIGHT;
            break;
        case 1:
            writedata(MADCTL_MV | MADCTL_BGR);
            _width  = ILI9341_TFTHEIGHT;
            _height = ILI9341_TFTWIDTH;
            break;
        case 2:
            writedata(MADCTL_MY | MADCTL_BGR);
            _width  = ILI9341_TFTWIDTH;
            _height = ILI9341_TFTHEIGHT;
            break;
        case 3:
            writedata(MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
            _width  = ILI9341_TFTHEIGHT;
            _height = ILI9341_TFTWIDTH;
            break;
    }

    this->SPIEnd();
}*/

#endif
