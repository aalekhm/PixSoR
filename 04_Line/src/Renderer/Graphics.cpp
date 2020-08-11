#include "Renderer/Graphics.h"

#define _INIT_FRAMEBUFFER_ \
m_iMaxBufferSize = m_iWidth * m_iHeight; \
m_pFrameBuffer = Sprite::create(m_iWidth, m_iHeight); \
m_pClearFrameBuffer = Sprite::create(m_iWidth, m_iHeight); \
{ \
	glEnable(GL_TEXTURE_2D); \
	glGenTextures(1, &m_glFrameBufferID); \
	glBindTexture(GL_TEXTURE_2D, m_glFrameBufferID); \
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); \
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); \
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL); \
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pFrameBuffer->getData()); \
} \

#define _SET_VIEWPORT_ glViewport(0, 0, m_iWidth, m_iHeight);

#define _RENDER_FRAMEBUFFER_ \
{ \
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_iWidth, m_iHeight, GL_RGBA, GL_UNSIGNED_BYTE, m_pFrameBuffer->getData()); \
	\
	float fSubPixelOffsetX = 0.0f, fSubPixelOffsetY = 0.0f; \
	glBegin(GL_QUADS); \
	glTexCoord2f(0.0, 1.0); glVertex3f(-1.0 + fSubPixelOffsetX, -1.0f + fSubPixelOffsetY, 0.0f); \
	glTexCoord2f(0.0, 0.0); glVertex3f(-1.0 + fSubPixelOffsetX, 1.0f + fSubPixelOffsetY, 0.0f); \
	glTexCoord2f(1.0, 0.0); glVertex3f(1.0 + fSubPixelOffsetX,  1.0f + fSubPixelOffsetY, 0.0f); \
	glTexCoord2f(1.0, 1.0); glVertex3f(1.0 + fSubPixelOffsetX, -1.0f + fSubPixelOffsetY, 0.0f); \
	glEnd(); \
} \

std::unique_ptr<Graphics> Graphics::create(uint32_t iWidth, uint32_t iHeight)
{
	return std::make_unique<Graphics>(_cookie{}, iWidth, iHeight);
}

Graphics::Graphics(_cookie)
: m_iWidth(DEFAULT_WIDTH)
, m_iHeight(DEFAULT_HEIGHT)
{
	init();
}

Graphics::Graphics(_cookie, uint32_t iWidth, uint32_t iHeight)
: m_iWidth(iWidth)
, m_iHeight(iHeight)
{
	init();
}

void Graphics::init()
{
	_INIT_FRAMEBUFFER_
}

void Graphics::paint(std::function<void(Graphics*)>& pRenderFunc) 
{
	_SET_VIEWPORT_
	pRenderFunc(this);
	_RENDER_FRAMEBUFFER_
}

void Graphics::clear()
{
	m_pFrameBuffer->clear(m_pClearFrameBuffer->getData());
}

void Graphics::setPixel(int32_t iX, int32_t iY, Pixel pPixel)
{
	m_pFrameBuffer->setPixel(iX, iY, pPixel);
}

void Graphics::setClearColour(Pixel pPixel)
{
	m_ClearColour = pPixel;

	for (int32_t i = 0; i < m_iMaxBufferSize; i++)
	{
		m_pClearFrameBuffer->setPixel(i % m_iWidth, i / m_iWidth, m_ClearColour);
	}
}

void Graphics::setColour(Pixel pPixel)
{
	m_RenderColour = pPixel;
}

void Graphics::drawLine(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2)
{
#ifdef USE_BRESENHAM
	drawBresenhamLine(x1, y1, x2, y2);
#else
	drawDDALine(x1, y1, x2, y2);
#endif
}

// Abdul Bari: 
//		DDA Line Drawing Algorithm - Computer Graphics
//		https://www.youtube.com/watch?v=W5P8GlaEOSI&t=14s
void Graphics::drawDDALine(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
{
	int32_t iDX = x2 - x1;
	int32_t iDY = y2 - y1;
	int32_t fAbs_DX = abs(iDX);
	int32_t fAbs_DY = abs(iDY);

	uint32_t iSteps = (fAbs_DX > fAbs_DY) ? fAbs_DX : fAbs_DY;
	float oneOverSteps = 1.0f / iSteps;
	float fXInc = iDX * oneOverSteps;
	float fYInc = iDY * oneOverSteps;

	float fX1 = x1;
	float fY1 = y1;
	for (int32_t i = 1; i < iSteps; i++)
	{
		m_pFrameBuffer->setPixel(fX1, fY1, m_RenderColour);
		fX1 += fXInc;
		fY1 += fYInc;
	}
}

// Abdul Bari:
//		DDA Line Drawing Algorithm - Computer Graphics
//		https://www.youtube.com/watch?v=RGB-wlatStc
// Sreelakshmi@CSIT:
//		https://www.youtube.com/watch?v=3pm1vbjYgnE - Bresenham's Line - Algorithm
//		https://www.youtube.com/watch?v=m1F2amYcTPc - Bresenham's Line - Derivation
void Graphics::drawBresenhamLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
{
	int32_t x = x1;
	int32_t y = y1;

	int32_t dX = abs(x2 - x1);
	int32_t dY = abs(y2 - y1);

	int32_t yIncr = (y2 > y1) ? 1 : -1;
	int32_t xIncr = (x2 > x1) ? 1 : -1;

	int32_t _2Dy = (2 * dY);
	int32_t _2Dx = (2 * dX);
	int32_t pDecision = 0;
	int32_t iSteps = 0;
	if (dY < dX)					// Slope(m) < 1
	{
		iSteps = dX;				// Steps incremented in 'x'
		pDecision = _2Dy - dX;		// Initial 'Decision'

		for (int32_t i = 0; i <= iSteps; i++)
		{
			m_pFrameBuffer->setPixel(x, y, m_RenderColour);

			x += xIncr;				// Always increment
			if (pDecision > 0)		// Only increment, if 'Decision' > 0
			{
				y += yIncr;
				pDecision = pDecision + _2Dy - _2Dx;
			}
			else
			{
				pDecision = pDecision + _2Dy;
			}
		}
	}
	else							// Slope(m) >= 1
	{
		iSteps = dY;				// Steps incremented in 'y'
		pDecision = _2Dx - dY;		// Initial 'Decision'

		for (int32_t i = 0; i <= iSteps; i++)
		{
			m_pFrameBuffer->setPixel(x, y, m_RenderColour);

			y += yIncr;				// Always increment
			if (pDecision > 0)		// Only increment, if 'Decision' > 0
			{
				x += xIncr;
				pDecision = pDecision + _2Dx - _2Dy;
			}
			else
			{
				pDecision = pDecision + _2Dx;
			}
		}
	}
}

