#include "stdafx.h"
#include "Display.h"

/* TODO: Right now, it's assumed that the grayscale renderer for FreeType
supports 256 shades of gray, but this should instead key off of num_grays
in the FT_Bitmap after the FT_Render_Glyph() call. */
#define NUM_GRAYS 256

static SDL_Window * _window;
static SDL_Renderer * _renderer;

template <typename T> FoneOSString ToString(const T& t);

void Display::Init()
{
	// TODO: Handle error- SDL_GetError()
	SDL_Init(SDL_INIT_EVERYTHING);
	_window = SDL_CreateWindow("foneOS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 240, 320, 0);
	_renderer = SDL_CreateRenderer(_window, -1, 0);
}

void Display::Clear(FoneOSColor color)
{
	SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(_renderer);
}

void Display::DrawRectangle(int x, int y, int w, int h, FoneOSColor color)
{
	SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	SDL_RenderDrawRect(_renderer, &rect);
}

void Display::FillRectangle(int x, int y, int w, int h, FoneOSColor color)
{
	SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	SDL_RenderFillRect(_renderer, &rect);
}


bool Display::DrawImage(FoneOSString filename, int x, int y)
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

	return true;
}

void Display::DrawCharacter(FoneOSChar character, int x, int y, int size, FoneOSColor color)
{
	// TODO: replace GPL code
	char ascii = character;
	if ((ascii < 0x20) || (ascii > 0x7e))//Unsupported char.
	{
		ascii = '?';
	}
	for (unsigned char i = 0; i<8; i++)
	{
		unsigned char temp = font[ascii - 0x20][i];
		for (unsigned char f = 0; f<8; f++)
		{
			if ((temp >> f) & 0x01)
			{
				Display::FillRectangle(x + i*size, y + f*size, size, size, color);
			}
		}
	}
}

void Display::DrawString(FoneOSString string, int x, int y, FoneFontDesc font, int size, FoneOSColor color, FoneOSColor bg)
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

			Display::FillRectangle(x + j, y + i, 1, 1, drawColor);// { 255 - color, 255 - color, 255 - color, 255 });
			count++;
		}
	}
	// TODO: replace GPL code
	/*
	int origX = x;
	const FoneOSChar * str = string.c_str();
	while (*str)
	{
		for (FoneOSChar i = 0; i<8; i++)
		{
			if (*str != '\n')
			{
				Display::DrawCharacter(*str, x, y, size, color);
			}
		}
		if (*str == '\n')
		{
			y += 8 * size;
			x = origX;
		}
		else
		{
			x += 8 * size;
		}
		*str++;
	}*/
}

void Display::DrawString(FoneOSString string, int x, int y, int size, FoneOSColor color, FoneOSColor bg)
{
	Display::DrawString(string, x, y, DEFAULT_FONT, size * 12, color, bg);
}

void Display::Flush()
{
	SDL_UpdateWindowSurface(_window);
	SDL_RenderPresent(_renderer);
}

void Display::Update()
{
	Display::Flush();

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

void Display::Cleanup()
{
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
}