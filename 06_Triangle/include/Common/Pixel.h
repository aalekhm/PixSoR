#pragma once
#include <stdint.h>

class Pixel
{
	public:
		union
		{
			uint32_t		m_iPixel = 0xFF000000;
			struct  
			{
				uint8_t	m_iRed;
				uint8_t	m_iGreen;
				uint8_t	m_iBlue;
				uint8_t	m_iAlpha;
			};
		};

					Pixel();
					Pixel(uint8_t iRed, uint8_t iGreen, uint8_t iBlue, uint8_t iAlpha = 255);
					Pixel(uint32_t iPixel);
		uint32_t	getPixel();

		bool operator==(const Pixel& p) const;
		bool operator!=(const Pixel& p) const;
	protected:
	private:
};