#include "stdafx.h"
#include "Display.h"

#ifdef SIMULATOR_BUILD

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
