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

