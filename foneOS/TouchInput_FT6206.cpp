#include "stdafx.h"
#include "TouchInput.h"

#ifdef PRODUCTION_BUILD
#define FT6206_ADDR           0x38
#define FT6206_G_FT5201ID     0xA8
#define FT6206_REG_NUMTOUCHES 0x02

#define FT6206_NUM_X             0x33
#define FT6206_NUM_Y             0x34

#define FT6206_REG_MODE 0x00
#define FT6206_REG_CALIBRATE 0x02
#define FT6206_REG_WORKMODE 0x00
#define FT6206_REG_FACTORYMODE 0x40
#define FT6206_REG_THRESHHOLD 0x80
#define FT6206_REG_POINTRATE 0x88
#define FT6206_REG_FIRMVERS 0xA6
#define FT6206_REG_CHIPID 0xA3
#define FT6206_REG_VENDID 0xA8

// calibrated for Adafruit 2.8" ctp screen
#define FT6206_DEFAULT_THRESSHOLD 128

void TouchInput_FT6206::Init()
{
    this->i2c = mraa_i2c_init(6);
    mraa_i2c_address(i2c, FT6206_ADDR);

    // TODO: check vend and chip IDs

    std::cout << ("Vend ID: "); printf("%02X\n", this->ReadRegister8(FT6206_REG_VENDID));
    std::cout << ("Chip ID: "); printf("%02X\n", this->ReadRegister8(FT6206_REG_CHIPID));
    std::cout << ("Firm V: "); printf("%02X\n", this->ReadRegister8(FT6206_REG_FIRMVERS));
    std::cout << ("Point Rate Hz: "); printf("%02X\n", this->ReadRegister8(FT6206_REG_POINTRATE));
    std::cout << ("Thresh: "); printf("%02X\n", this->ReadRegister8(FT6206_REG_THRESHHOLD));
}

void TouchInput_FT6206::Update()
{
    if (!this->Touched())
    {
        return;
    }

    FoneOSPoint pt = this->GetPoint();
    if (pt.z > 0)
    {
        Input::SendTouch(pt);
    }
}

void TouchInput_FT6206::Cleanup()
{

}

bool TouchInput_FT6206::Touched()
{
    uint8_t n = this->ReadRegister8(FT6206_REG_NUMTOUCHES);
    if ((n == 1) || (n == 2)) return true;
    return false;
}

void TouchInput_FT6206::ReadData(uint16_t *x, uint16_t *y)
{
    uint8_t i2cdat[32];

    mraa_i2c_address(i2c, FT6206_ADDR);
    mraa_i2c_read_bytes_data(i2c, 0, i2cdat, 32);

    touches = i2cdat[0x02];

    if (touches > 2) {
        touches = 0;
        *x = *y = 0;
    }
    if (touches == 0) {
        *x = *y = 0;
        return;
    }

    for (uint8_t i=0; i<2; i++) {
        touchX[i] = i2cdat[0x03 + i*6] & 0x0F;
        touchX[i] <<= 8;
        touchX[i] |= i2cdat[0x04 + i*6];
        touchY[i] = i2cdat[0x05 + i*6] & 0x0F;
        touchY[i] <<= 8;
        touchY[i] |= i2cdat[0x06 + i*6];
        touchID[i] = i2cdat[0x05 + i*6] >> 4;
    }

    *x = touchX[0]; *y = touchY[0];
}

FoneOSPoint TouchInput_FT6206::GetPoint()
{
    uint16_t x, y;
    uint8_t z; // TODO: read pressure

    this->ReadData(&x, &y);

    FoneOSPoint pt = FoneOSPoint();
    // for some reason, the x and y are flipped...
    pt.x = (HardwareManager::GetDisplay()->GetWidth() - (int)x);
    pt.y = (HardwareManager::GetDisplay()->GetHeight() - (int)y);
    pt.z = (int)1;

    return pt;
}

uint8_t TouchInput_FT6206::ReadRegister8(uint8_t reg)
{
    uint8_t x;

    mraa_i2c_address(i2c, FT6206_ADDR);
    /*mraa_i2c_write_byte(i2c, reg);

    mraa_i2c_address(i2c, FT6206_ADDR);*/
    x = mraa_i2c_read_byte_data(i2c, reg);

    return x;
}

void TouchInput_FT6206::WriteRegister8(uint8_t reg, uint8_t val)
{
    mraa_i2c_address(i2c, FT6206_ADDR);
    mraa_i2c_write_byte(i2c, reg);
    mraa_i2c_write_byte(i2c, val);
}

#endif
