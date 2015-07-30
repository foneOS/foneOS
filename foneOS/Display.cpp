#include "stdafx.h"
#include "Display.h"

#ifdef EDISON
#define LOW 0
#define HIGH 1

#define INPUT MRAA_GPIO_IN
#define OUTPUT MRAA_GPIO_OUT

#define CS_PIN    10
#define CS_OUTPUT {this->SetPin(CS_PIN, OUTPUT);}
#define CS_HIGH   {this->DigitalWrite(CS_PIN, HIGH);}
#define CS_LOW    {this->DigitalWrite(CS_PIN, LOW);}

#define RS_PIN    11
#define RS_OUTPUT {this->SetPin(RS_PIN, OUTPUT);}
#define RS_HIGH   {this->DigitalWrite(RS_PIN, HIGH);}
#define RS_LOW    {this->DigitalWrite(RS_PIN, LOW);}

#define WR_PIN    11
#define WR_OUTPUT {this->SetPin(WR_PIN, OUTPUT);}
#define WR_HIGH   {this->DigitalWrite(WR_PIN, HIGH);}
#define WR_LOW    {this->DigitalWrite(WR_PIN, LOW);}

#define RD_PIN    11
#define RD_OUTPUT {this->SetPin(RD_PIN, OUTPUT);}
#define RD_HIGH   {this->DigitalWrite(RD_PIN, HIGH);}
#define RD_LOW    {this->DigitalWrite(RD_PIN, LOW);}

void Display_ST7781R::SetPin(unsigned int pin, mraa_gpio_dir_t dir)
{
    if (this->pins.find(pin) == this->pins.end())
    {
        this->pins[pin] = mraa_gpio_init(pin);
        mraa_gpio_use_mmaped(this->pins[pin], 1);
    }
    mraa_gpio_dir(this->pins[pin], dir);
}

void Display_ST7781R::DigitalWrite(unsigned int pin, unsigned int value)
{
    mraa_gpio_write(this->pins[pin], value);
}

int Display_ST7781R::DigitalRead(unsigned int pin)
{
    return mraa_gpio_read(this->pins[pin]);
}

void Display_ST7781R::AllPinLow()
{
    this->DigitalWrite(2, LOW);
    this->DigitalWrite(3, LOW);
    this->DigitalWrite(4, LOW);
    this->DigitalWrite(5, LOW);
    this->DigitalWrite(6, LOW);
    this->DigitalWrite(7, LOW);
    this->DigitalWrite(8, LOW);
    this->DigitalWrite(9, LOW);
}

void Display_ST7781R::AllPinOutput()
{
    this->SetPin(2, OUTPUT);
    this->SetPin(3, OUTPUT);
    this->SetPin(4, OUTPUT);
    this->SetPin(5, OUTPUT);
    this->SetPin(6, OUTPUT);
    this->SetPin(7, OUTPUT);
    this->SetPin(8, OUTPUT);
    this->SetPin(9, OUTPUT);
}

void Display_ST7781R::PushData(unsigned char data)
{
    this->AllPinLow();
    unsigned char shifted = (data<<2);
    unsigned char shiftedsix = (data>>6);

    unsigned char nine = ((shiftedsix >> 1)  & 0x01);
    unsigned char eight = ((shiftedsix >> 0)  & 0x01);
    unsigned char seven = ((shifted >> 7)  & 0x01);
    unsigned char six = ((shifted >> 6)  & 0x01);
    unsigned char five = ((shifted >> 5)  & 0x01);
    unsigned char four = ((shifted >> 4)  & 0x01);
    unsigned char three = ((shifted >> 3)  & 0x01);
    unsigned char two = ((shifted >> 2)  & 0x01);

    this->DigitalWrite(2, two);
    this->DigitalWrite(3, three);
    this->DigitalWrite(4, four);
    this->DigitalWrite(5, five);
    this->DigitalWrite(6, six);
    this->DigitalWrite(7, seven);
    this->DigitalWrite(8, eight);
    this->DigitalWrite(9, nine);
}

unsigned char Display_ST7781R::GetData()
{
    unsigned char data=0;
    Utils::Delay(1);
    data |= this->DigitalRead(2) << 0;
    data |= this->DigitalRead(3) << 1;
    data |= this->DigitalRead(4) << 2;
    data |= this->DigitalRead(5) << 3;
    data |= this->DigitalRead(6) << 4;
    data |= this->DigitalRead(7) << 5;
    data |= this->DigitalRead(8) << 6;
    data |= this->DigitalRead(9) << 7;
    return data;
}

void Display_ST7781R::SendCommand(unsigned int index)
{
    CS_LOW;
    RS_LOW;
    RD_HIGH;
    WR_HIGH;

    WR_LOW;
    this->PushData(0);
    WR_HIGH;
    WR_LOW;
    this->PushData(index&0xff);
    WR_HIGH;

    CS_HIGH;
}

void Display_ST7781R::SendData(unsigned int data)
{
    CS_LOW;
    RS_HIGH;
    RD_HIGH;

    WR_LOW;
    this->PushData((data&0xff00)>>8);
    WR_HIGH;

    WR_LOW;
    this->PushData(data&0xff);
    WR_HIGH;

    CS_HIGH;
}

void Display_ST7781R::ExitStandBy()
{
    this->SendCommand(0x0010);
    this->SendData(0x14E0);

    Utils::Delay(100);

    this->SendCommand(0x0007);
    this->SendData(0x0133);
}

uint8_t Display_ST7781R::CreateColor(FoneOSColor color)
{
    return ((color.r & 0xF8) << 8) | ((color.g & 0xFC) << 3) | (color.b >> 3);
}

void Display_ST7781R::Init()
{
    this->pins = std::map<unsigned int, mraa_gpio_context>();

    CS_OUTPUT;
    RD_OUTPUT;
    WR_OUTPUT;
    RS_OUTPUT;

    this->AllPinOutput();
    this->AllPinLow();

    Utils::Delay(100);

    this->SendCommand(0x0001);
    this->SendData(0x0100);
    this->SendCommand(0x0002);
    this->SendData(0x0700);
    this->SendCommand(0x0003);
    this->SendData(0x1030);
    this->SendCommand(0x0004);
    this->SendData(0x0000);
    this->SendCommand(0x0008);
    this->SendData(0x0302);
    this->SendCommand(0x000A);
    this->SendData(0x0000);
    this->SendCommand(0x000C);
    this->SendData(0x0000);
    this->SendCommand(0x000D);
    this->SendData(0x0000);
    this->SendCommand(0x000F);
    this->SendData(0x0000);

    Utils::Delay(100);

    this->SendCommand(0x0030);
    this->SendData(0x0000);
    this->SendCommand(0x0031);
    this->SendData(0x0405);
    this->SendCommand(0x0032);
    this->SendData(0x0203);
    this->SendCommand(0x0035);
    this->SendData(0x0004);
    this->SendCommand(0x0036);
    this->SendData(0x0B07);
    this->SendCommand(0x0037);
    this->SendData(0x0000);
    this->SendCommand(0x0038);
    this->SendData(0x0405);
    this->SendCommand(0x0039);
    this->SendData(0x0203);
    this->SendCommand(0x003c);
    this->SendData(0x0004);
    this->SendCommand(0x003d);
    this->SendData(0x0B07);
    this->SendCommand(0x0020);
    this->SendData(0x0000);
    this->SendCommand(0x0021);
    this->SendData(0x0000);
    this->SendCommand(0x0050);
    this->SendData(0x0000);
    this->SendCommand(0x0051);
    this->SendData(0x00ef);
    this->SendCommand(0x0052);
    this->SendData(0x0000);
    this->SendCommand(0x0053);
    this->SendData(0x013f);

    Utils::Delay(100);

    this->SendCommand(0x0060);
    this->SendData(0xa700);
    this->SendCommand(0x0061);
    this->SendData(0x0001);
    this->SendCommand(0x0090);
    this->SendData(0x003A);
    this->SendCommand(0x0095);
    this->SendData(0x021E);
    this->SendCommand(0x0080);
    this->SendData(0x0000);
    this->SendCommand(0x0081);
    this->SendData(0x0000);
    this->SendCommand(0x0082);
    this->SendData(0x0000);
    this->SendCommand(0x0083);
    this->SendData(0x0000);
    this->SendCommand(0x0084);
    this->SendData(0x0000);
    this->SendCommand(0x0085);
    this->SendData(0x0000);
    this->SendCommand(0x00FF);
    this->SendData(0x0001);
    this->SendCommand(0x00B0);
    this->SendData(0x140D);
    this->SendCommand(0x00FF);
    this->SendData(0x0000);

    Utils::Delay(100);

    this->SendCommand(0x0007);
    this->SendData(0x0133);

    Utils::Delay(50);

    this->ExitStandBy();

    this->SendCommand(0x0022);

    //paint screen black
    this->Clear(COLOR_BLACK);
}

void Display_ST7781R::Clear(FoneOSColor color)
{
    uint8_t color8 = this->CreateColor(color);
    for (unsigned char i = 0; i < 2; i++)
    {
        for (unsigned int f = 0; f < 38400; f++)
        {
            this->SendData(color8);
        }
    }
}

void Display_ST7781R::DrawHorizontalLine(unsigned int x, unsigned int y, unsigned int length, FoneOSColor color)
{
    unsigned char color8 = this->CreateColor(color);
    this->SetXY(x,x);
    this->SetOrientation(0);
    if(length + x > this->GetWidth())
    {
        length = this->GetWidth() - x;
    }
    for(unsigned int i=0;i<length;i++)
    {
        this->SendData(color8);
    }
}

void Display_ST7781R::DrawVerticalLine(unsigned int x, unsigned int y, unsigned int length, FoneOSColor color)
{
    unsigned char color8 = this->CreateColor(color);
    this->SetXY(x,y);
    this->SetOrientation(1);
    if(length + x > this->GetHeight())
    {
        length = this->GetHeight() - x;
    }
    for(unsigned int i=0;i<length;i++)
    {
        this->SendData(color8);
    }
}

void Display_ST7781R::DrawRectangle(int x, int y, int w, int h, FoneOSColor color)
{
    this->DrawHorizontalLine(x, y, l, color);
    this->DrawHorizontalLine(x, y + w, l, color);

    this->DrawVerticalLine(x, y, w, color);
    this->DrawVerticalLine(x + l, y, w, color);
}

void Display_ST7781R::FillRectangle(int x, int y, int w, int h, FoneOSColor color)
{
    for(unsigned int i=0;i<w;i++)
    {
        this->DrawHorizontalLine(x, y + i, h, color);
    }
}

bool Display_ST7781R::DrawImage(FoneOSString filename, int x, int y)
{
    return true;
}

void Display_ST7781R::DrawString(FoneOSString string, int x, int y, FoneFontDesc font, int size, FoneOSColor color, FoneOSColor bg)
{

}

void Display_ST7781R::DrawString(FoneOSString string, int x, int y, int size, FoneOSColor color, FoneOSColor bg)
{

}

void Display_ST7781R::Flush()
{
    // no-op
}

void Display_ST7781R::Update()
{

}

void Display_ST7781R::Cleanup()
{

}

int Display_ST7781R::GetHorizDPI()
{
    return 72;
}

int Display_ST7781R::GetVertDPI()
{
    return 72;
}

int Display_ST7781R::GetWidth()
{
    return 240;
}

int Display_ST7781R::GetHeight()
{
    return 320;
}

#endif

#ifdef SIMULATOR_BUILD

/* TODO: Right now, it's assumed that the grayscale renderer for FreeType
supports 256 shades of gray, but this should instead key off of num_grays
in the FT_Bitmap after the FT_Render_Glyph() call. */
#define NUM_GRAYS 256

static SDL_Window * _window;
static SDL_Renderer * _renderer;

template <typename T> FoneOSString ToString(const T& t);

void Display_SDL::Init()
{
	// TODO: Handle error- SDL_GetError()
	SDL_Init(SDL_INIT_EVERYTHING);
	_window = SDL_CreateWindow("foneOS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 240, 320, 0);
	_renderer = SDL_CreateRenderer(_window, -1, 0);
}

void Display_SDL::Clear(FoneOSColor color)
{
	SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(_renderer);
}

void Display_SDL::DrawRectangle(int x, int y, int w, int h, FoneOSColor color)
{
	SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	SDL_RenderDrawRect(_renderer, &rect);
}

void Display_SDL::FillRectangle(int x, int y, int w, int h, FoneOSColor color)
{
	SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	SDL_RenderFillRect(_renderer, &rect);
}


bool Display_SDL::DrawImage(FoneOSString filename, int x, int y)
{
	FoneOSString filePathStr = Storage::GetFullPath(filename);
	char * filePath = Utils::FoneOSStringToCharArray(filePathStr);
	SDL_Surface * image = SDL_LoadBMP(filePath);
	if (image == NULL)
	{
		return false;
	}
	SDL_Texture * texture = SDL_CreateTextureFromSurface(_renderer, image);
	if (texture == NULL)
	{
		return false;
	}
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);

	SDL_RenderCopy(_renderer, texture, NULL, &dst);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(image);
	free((void*)filePath);

	return true;
}

void Display_SDL::DrawString(FoneOSString string, int x, int y, FoneFontDesc font, int size, FoneOSColor color, FoneOSColor bg)
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
			Uint8 rdiff = abs(color.r - bg.r);
			Uint8 gdiff = abs(color.g - bg.g);
			Uint8 bdiff = abs(color.b - bg.b);

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

void Display_SDL::DrawString(FoneOSString string, int x, int y, int size, FoneOSColor color, FoneOSColor bg)
{
	this->DrawString(string, x, y, DEFAULT_FONT, size * 12, color, bg);
}

void Display_SDL::Flush()
{
	SDL_UpdateWindowSurface(_window);
	SDL_RenderPresent(_renderer);
}

void Display_SDL::Update()
{
	this->Flush();

	SDL_Event event;
	if (SDL_PollEvent(&event) != 0)
	{
		if (event.type == SDL_QUIT)
		{
			Logging::LogMessage(STR("Quitting via the close button is not supported; please use the power button!"));
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Warning", "Quitting via the close button is not supported; please use the power button!", _window);
		}

		if (event.type == SDL_MOUSEBUTTONUP)
		{
			FoneOSPoint point = FoneOSPoint();
			point.x = event.button.x;
			point.y = event.button.y;
			point.z = 25;
			Input::SendTouch(point);
		}
	}
}

void Display_SDL::Cleanup()
{
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
}

int Display_SDL::GetHorizDPI()
{
	return 72;
}

int Display_SDL::GetVertDPI()
{
	return 72;
}

int Display_SDL::GetWidth()
{
    return 240;
}

int Display_SDL::GetHeight()
{
    return 320;
}

#endif
