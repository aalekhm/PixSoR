#include "Common/Sprite.h"

Sprite::Sprite(_cookie)
: m_pColourData(nullptr)
, m_iWidth(0)
, m_iHeight(0)
{
}

std::unique_ptr<Sprite>	Sprite::create(uint32_t iWidth, uint32_t iHeight)
{
	std::unique_ptr<Sprite> pSprite = std::make_unique<Sprite>(_cookie{});
	if (pSprite != nullptr)
	{
		if (pSprite->m_pColourData != nullptr)
			delete[] pSprite->m_pColourData;

		pSprite->m_iWidth = iWidth;
		pSprite->m_iHeight = iHeight;

		uint32_t iSize = iWidth * iHeight;
		pSprite->m_pColourData = new Pixel[iSize];
		for (uint32_t i = 0; i < iSize; i++)
		{
			pSprite->m_pColourData[i] = Pixel();
		}
	}

	return pSprite;
}

void Sprite::clear(Pixel p)
{
	uint32_t iSize = m_iWidth * m_iHeight;
	uint32_t iPixel = p.getPixel();
	for (int32_t i = 0; i < iSize; i++)
	{
		memcpy(&m_pColourData[i], &iPixel, sizeof(Pixel));
	}
}

void Sprite::clear(Pixel* pColourData)
{
	if (pColourData != nullptr)
	{
		memcpy(m_pColourData, pColourData, (m_iWidth * m_iHeight * sizeof(Pixel)));
	}
}

Pixel Sprite::getPixel(int32_t x, int32_t y)
{
	if (x >= 0 && x < m_iWidth && y >= 0 && y < m_iHeight)
		return m_pColourData[y * m_iWidth + x];
	else
		return Pixel(0, 0, 0, 0);
}

bool Sprite::setPixel(int32_t x, int32_t y, Pixel p)
{
	if (x >= 0 && x < m_iWidth && y >= 0 && y < m_iHeight)
	{
		m_pColourData[y * m_iWidth + x] = p;
		return true;
	}
	
	return false;
}

Pixel* Sprite::getData()
{
	return m_pColourData;
}

Sprite::~Sprite()
{
	if (m_pColourData)
		delete[] m_pColourData;
}