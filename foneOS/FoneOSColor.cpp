#include "stdafx.h"
#include "FoneOSColor.h"


FoneOSColor::FoneOSColor()
{
    this->r = 255;
    this->g = 10;
    this->b = 10;
    this->a = 255;
}

FoneOSColor::FoneOSColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
    this->r = red;
    this->g = green;
    this->b = blue;
    this->a = alpha;
}
