#pragma once
class TouchInput
{
public:
    virtual ~TouchInput() { }

    // Initializes the touch input device.
    virtual void Init() = 0;

    // Updates the touch input device.
	virtual void Update() = 0;

    // Frees memory associated with the touch input device.
    virtual void Cleanup() = 0;
};

#ifdef SIMULATOR_BUILD
class TouchInput_SDL : public TouchInput
{
    virtual void Init();
	virtual void Update();
    virtual void Cleanup();
};
#endif

#ifdef PRODUCTION_BUILD
class TouchInput_Resistive : public TouchInput
{
public:
    virtual void Init();
    virtual void Update();
    virtual void Cleanup();
private:
    std::map<unsigned int, mraa_gpio_context> pins;
    std::map<unsigned int, mraa_aio_context> apins;

    void SetPin(unsigned int pin, mraa_gpio_dir_t dir);
    void SetAPin(unsigned int pin);

    void DigitalWrite(unsigned int pin, unsigned int value);
    int DigitalRead(unsigned int pin);
    int AnalogRead(unsigned int pin);
};


class TouchInput_FT6206 : public TouchInput
{
public:
    virtual void Init();
    virtual void Update();
    virtual void Cleanup();
private:
/*    std::map<unsigned int, mraa_gpio_context> pins;
    int _cs, _dc, _rst;
    mraa_spi_context _spi;

    void SetPin(unsigned int pin, mraa_gpio_dir_t dir);

    void DigitalWrite(unsigned int pin, unsigned int value);

    void SetAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

    void SPIBegin();
    void SPIEnd();

    void SPIWrite(uint8_t c);
    uint8_t SPIRead();

    uint8_t readcommand8(uint8_t c, uint8_t index = 0);

    void WriteCommand(uint8_t c);
    void WriteData(uint8_t c);

    uint16_t CreateColor(FoneOSColor color);

    void PushColor(uint16_t color);

    void DrawHorizontalLine(unsigned int x, unsigned int y, unsigned int length, FoneOSColor color);
    void DrawVerticalLine(unsigned int x, unsigned int y, unsigned int length, FoneOSColor color);

    //void SetOrientation(unsigned int orientation);

    uint8_t readByte(std::ifstream * f);
    uint16_t read16(std::ifstream * f);
    uint32_t read32(std::ifstream * f);

    bool bmpReadHeader(std::ifstream * f);

    void bmpdraw(std::ifstream * bmpFile, int x, int y);*/
};
#endif
