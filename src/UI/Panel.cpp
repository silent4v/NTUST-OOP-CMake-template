#include "Panel.h"

#include "../ConsoleRender.hpp"

namespace FTK
{
	namespace UI
	{
		void UIPanel::drawString(const std::string& text, int x, int y, const PixelColor& color)
		{
			int _x = x;
			int _y = y;
			for (int i = 0; i < text.size(); i++)
			{
				const auto& character = text[i];
				Vector2D coord(_x, _y);
				if (character == '\n')
				{
					_x = x;
					_y++;
				}
				else
				{
					_x++;
					ConsoleRender::Instance().DrawPixel(coord, Pixel(character, color));
				}
			}
		}

		void UIPanel::DrawPanel(const std::string& text, const PixelColor& color, bool withEdge)
		{
			const auto& [x, y] = this->Position.XY();

			this->drawString(text.c_str(), x + 1, y + 1, color);
			if (withEdge)
				ConsoleRender::Instance().DrawEdge(x, y, this->Width, this->Height);
		}

		void UIPanel::DrawPanel(const std::vector<DrawInfo>& drawInfos, bool withEdge)
		{
			const auto& [x, y] = this->Position.XY();
			for (const auto& drawInfo : drawInfos)
			{
				this->drawString(drawInfo.Content, x + drawInfo.Offset.X, y + drawInfo.Offset.Y, drawInfo.Color);
			}
			if (withEdge)
				ConsoleRender::Instance().DrawEdge(x, y, this->Width, this->Height);
		}

		void UIPanel::DrawPixel(const Vector2D& coord, const Pixel& pixel)
		{
			if (coord.X < this->Position.X || coord.X >= this->Position.X + this->Width ||
				coord.Y < this->Position.Y || coord.Y >= this->Position.Y + this->Height)
				return;
			ConsoleRender::Instance().DrawPixel(coord, pixel);
		}
		void UIPanel::DrawPixel(int x, int y, char c, const PixelColor& color)
		{
			this->DrawPixel(Vector2D(x, y) + this->Position, Pixel(c, color));
		}
		void UIPanel::DrawByLine(const std::string& text, const Vector2D& coord, const PixelColor& color)
		{
			for (int i = coord.X; i < this->Width - 1; i++)
			{
				if (i - coord.X < text.size())
				{
					this->DrawPixel(i, coord.Y, text[i - coord.X], color);
				}
				else
				{
					this->DrawPixel(i, coord.Y, ' ', color);
				}
			}
		}
		void UIPanel::DrawEdge(const PixelColor& color)
		{
			ConsoleRender::Instance().DrawEdge(this->Position.X, this->Position.Y, this->Width, this->Height, color);
		}
		void UIPanel::ClearPanel(bool withEdge, const PixelColor& color)
		{
			for (int i = 0; i < this->Width; i++)
			{
				for (int j = 0; j < this->Height; j++)
				{
					this->DrawPixel(i, j, ' ', color);
				}
			}
			if (withEdge)
			{
				ConsoleRender::Instance().DrawEdge(this->Position.X, this->Position.Y, this->Width, this->Height);
			}
		}
	}
}