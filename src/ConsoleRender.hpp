#ifndef ConsoleRender_H

#define ConsoleRender_H

#include <iostream>
#include <vector>
#include <Windows.h>

#include "Util/Singleton.hpp"
#include "Vector2D.hpp"
#include "UI/Pixel.hpp"

namespace FTK
{
	class ConsoleRender : public Util::Singleton<ConsoleRender>
	{
	private: friend class Util::Singleton<ConsoleRender>;

#pragma region Using
	public:
		using Pixel = UI::Pixel;
		using PixelColor = UI::PixelColor;
#pragma endregion


	private: static const int OUTPUT_WIDTH = 440;
	private: static const int OUTPUT_HEIGHT = 160;
	private: CHAR_INFO outputBuffer[OUTPUT_WIDTH * OUTPUT_HEIGHT];
	private: HANDLE hConsole;
	private: HANDLE screenBuffer;

	public: static const int MAX_CONSOLE_WIDTH = 143;
	public: static const int MAX_CONSOLE_HEIGHT = 51;
	private: Pixel consoleBuffer[MAX_CONSOLE_HEIGHT][MAX_CONSOLE_WIDTH];

	private: bool validDrawPosition(int x, int y)
	{
		return x >= 0 && x < MAX_CONSOLE_WIDTH && y >= 0 && y < MAX_CONSOLE_HEIGHT;
	}
	public: bool DrawPixel(int x, int y, const Pixel& pixel)
	{
		if (!validDrawPosition(x, y))
			return false;

		if (consoleBuffer[y][x] == pixel)
			return true;

		COORD coord = { (SHORT)x, (SHORT)y };
		WORD color = pixel.Color.ForeGroundColor | pixel.Color.BackGroundColor;
		DWORD written;
		WriteConsoleOutputAttribute(
			this->screenBuffer,
			&color,
			1, coord, &written);
		WriteConsoleOutputCharacterW(
			this->screenBuffer,
			&pixel.Character,
			1, coord, &written);
		consoleBuffer[y][x] = pixel;

		return true;
	}
	public: bool DrawPixel(const Vector2D& position, const Pixel& pixel)
	{
		return DrawPixel(position.X, position.Y, pixel);
	}
	public: void DrawEdge(const int x, int y, const int width, const int height, const PixelColor& color = PixelColor())
	{
		for (int i = 0; i < width; i++)
		{
			this->DrawPixel(x + i, y, Pixel(L'-', color));
			this->DrawPixel(x + i, y + height - 1, Pixel(L'-', color));
		}
		for (int i = 0; i < height; i++)
		{
			this->DrawPixel(x, y + i, Pixel(L'|',color));
			this->DrawPixel(x + width - 1, y + i, Pixel(L'|', color));
		}
	}
	public: inline void DrawConsoleEdge() { this->DrawEdge(0, 0, MAX_CONSOLE_WIDTH, MAX_CONSOLE_HEIGHT); }

	public: void Clear()
	{
		for (int y = 0; y < MAX_CONSOLE_HEIGHT; y++)
		{
			for (int x = 0; x < MAX_CONSOLE_WIDTH; x++)
			{
				this->DrawPixel(x, y, Pixel());
			}
		}
	}

	private: ConsoleRender()
	{
		this->hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		this->screenBuffer = CreateConsoleScreenBuffer(
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			CONSOLE_TEXTMODE_BUFFER,
			NULL
		);

		if (this->screenBuffer == INVALID_HANDLE_VALUE)
		{
			std::cout << "CreateConsoleScreenBuffer failed - (%d)\n" << GetLastError();

			throw std::exception();
		}

		std::system("cls");

		for (int y = 0; y < MAX_CONSOLE_HEIGHT; y++)
		{
			for (int x = 0; x < MAX_CONSOLE_WIDTH; x++)
			{
				consoleBuffer[y][x] = Pixel();
			}
		}
	}

	private: SMALL_RECT srctReadRect = { 0, 0, OUTPUT_WIDTH - 1, OUTPUT_HEIGHT - 1 };
	private: const COORD coordBufSize = { (SHORT)OUTPUT_WIDTH, (SHORT)OUTPUT_HEIGHT };
	private: const COORD coordBufCoord = { 0, 0 };
	public: void Render()
	{
		// read from screen buffer
		if (auto fSuccess = ReadConsoleOutput(
			this->screenBuffer,
			this->outputBuffer,
			coordBufSize,
			coordBufCoord,
			&srctReadRect)
			; !fSuccess)
		{
			std::cout << "ReadConsoleOutput failed - (%d)\n" << GetLastError();
			return;
		}

		// write to console
		if (auto fSuccess = WriteConsoleOutput(
			this->hConsole,
			this->outputBuffer,
			coordBufSize,
			coordBufCoord,
			&srctReadRect)
			; !fSuccess)
		{
			std::cout << "WriteConsoleOutput failed - (%d)\n" << GetLastError();
			return;
		}

		SetConsoleActiveScreenBuffer(this->hConsole);
	}

	};
}
#endif // !ConsoleRender_H