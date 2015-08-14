#include "stdafx.h"
#include "TouchInput.h"

#ifdef SIMULATOR_BUILD
void TouchInput_SDL::Init()
{
    // this is handled by Display_SDL (for now)
}

void TouchInput_SDL::Update()
{

}

void TouchInput_SDL::Cleanup()
{

}
#endif
