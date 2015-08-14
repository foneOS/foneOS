#include "stdafx.h"
#include "TouchInput.h"

#ifdef PRODUCTION_BUILD
#define LOW 0
#define HIGH 1

#define INPUT MRAA_GPIO_IN
#define OUTPUT MRAA_GPIO_OUT
#define YP 16   // must be an analog pin, use "An" notation!
#define XM 15   // must be an analog pin, use "An" notation!
#define YPa 2   // must be an analog pin, use "An" notation!
#define XMa 1   // must be an analog pin, use "An" notation!
#define YM 14   // can be a digital pin, this is A0
#define XP 17   // can be a digital pin, this is A3
#define RXPLATE 300

//Measured ADC values for (0,0) and (210-1,320-1)
//TS_MINX corresponds to ADC value when X = 0
//TS_MINY corresponds to ADC value when Y = 0
//TS_MAXX corresponds to ADC value when X = 240 -1
//TS_MAXY corresponds to ADC value when Y = 320 -1

#define TS_MINX 140
#define TS_MAXX 900

#define TS_MINY 120
#define TS_MAXY 940

void TouchInput_Resistive::SetPin(unsigned int pin, mraa_gpio_dir_t dir)
{
    if (this->pins.find(pin) == this->pins.end())
    {
        this->pins[pin] = mraa_gpio_init(pin);
        mraa_gpio_use_mmaped(this->pins[pin], 1);
    }
    mraa_gpio_dir(this->pins[pin], dir);
}

void TouchInput_Resistive::SetAPin(unsigned int pin)
{
    if (this->apins.find(pin) == this->apins.end())
    {
        this->apins[pin] = mraa_aio_init(pin);
    }
}

void TouchInput_Resistive::DigitalWrite(unsigned int pin, unsigned int value)
{
    mraa_gpio_write(this->pins[pin], value);
}

int TouchInput_Resistive::DigitalRead(unsigned int pin)
{
    return mraa_gpio_read(this->pins[pin]);
}

int TouchInput_Resistive::AnalogRead(unsigned int pin)
{
    return mraa_aio_read(this->apins[pin]);
}

void TouchInput_Resistive::Init()
{
    this->pins = std::map<unsigned int, mraa_gpio_context>();
    this->apins = std::map<unsigned int, mraa_aio_context>();
}

void TouchInput_Resistive::Update()
{
    this->SetAPin(YPa);
    this->SetAPin(XMa);

    this->SetPin(YP, OUTPUT);
    this->SetPin(YM, OUTPUT);

    this->DigitalWrite(YP, LOW);
    this->DigitalWrite(YM, LOW);

    this->SetPin(XP, OUTPUT);
    this->SetPin(XM, OUTPUT);

    this->DigitalWrite(XP, HIGH);
    this->DigitalWrite(XM, LOW);

    int sample = this->AnalogRead(YPa);

    int x = (1023-sample);

    this->SetPin(XP, INPUT);
    this->SetPin(XM, INPUT);

    this->DigitalWrite(XP, LOW);

    this->SetPin(YP, OUTPUT);

    this->DigitalWrite(YP, HIGH);
    this->DigitalWrite(YM, HIGH);

    this->SetPin(YM, OUTPUT);

    sample = this->AnalogRead(XMa);

    int y = (1023-sample);

    this->SetPin(XP, OUTPUT);
    this->DigitalWrite(XP, LOW);

    this->DigitalWrite(YM, HIGH);

    this->DigitalWrite(YP, LOW);
    this->SetAPin(YPa);

    int z1 = this->AnalogRead(XMa);
    int z2 = this->AnalogRead(YPa);

    int z = 0;
    if (RXPLATE != 0) {
        // now read the x
        float rtouch;
        rtouch = z2;
        rtouch /= z1;
        rtouch -= 1;
        rtouch *= x;
        rtouch *= RXPLATE;
        rtouch /= 1024;

        z = rtouch;
    } else {
        z = (1023-(z2-z1));
    }

    FoneOSPoint point;
    point.x = (x - TS_MINX) * (240 - 0) / (TS_MAXX - TS_MINX) + 0; //x;
    point.y = (y - TS_MINY) * (320 - 0) / (TS_MAXY - TS_MINY) + 0;
    point.z = z;
    if (point.z > 0)
    {
        Input::SendTouch(point);
    }
}

void TouchInput_Resistive::Cleanup()
{

}
#endif
