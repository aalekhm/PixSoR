#include "Renderer/Graphics.h"
#include "Renderer/Edge.h"
#include "Renderer/OpenGL.h"
#include "Renderer/IShader.h"

#define FRAG_SHADER OpenGL::m_pCurShader->fragment()

#define TRANSFORM_HOMOGENEOUS_TO_SCREENCOORDINATES(__vertexInternal__) \
{ \
	__vertexInternal__.m_vVertPosition.x = (__vertexInternal__.m_vVertPosition.x * m_fHalfWidth) + m_fHalfWidth; \
	__vertexInternal__.m_vVertPosition.y = (__vertexInternal__.m_vVertPosition.y * -m_fHalfHeight) + m_fHalfHeight; \
} \

#define INTERPOLATE_VARYINGS(__leftEdge__, __rightEdge__, __lerpAmount__, __InterpolateVaryings__) \
/* Calculate 1/z for Perspective Correct Varyings */ \
float fLerpedZ = 1.0f; \
{ \
	float fOneOverZLerped = LERP( GET_EDGE_VARYING_INTERNAL(__leftEdge__, EVaryingsInternal::ONEOVERZ_VARYING), GET_EDGE_VARYING_INTERNAL(__rightEdge__, EVaryingsInternal::ONEOVERZ_VARYING), __lerpAmount__ ); \
	fLerpedZ = 1.0f / fOneOverZLerped; \
} \
\
for(int32_t i = 0; i < __leftEdge__->m_pVertexVaryingsCount; i++) \
{ \
	__InterpolateVaryings__[i] = LERP( GET_EDGE_VARYING(__leftEdge__, i), GET_EDGE_VARYING(__rightEdge__, i), __lerpAmount__); \
\
	/* Perspective Correct Varying */ \
	__InterpolateVaryings__[i] *= fLerpedZ; \
} \

#define PERSPECTIVE_DIVIDE(__vertexInternal__) \
{ \
	float fW = __vertexInternal__.m_vVertPosition.w; \
	__vertexInternal__.m_vVertPosition /= fW;		/* Divide by 'z' */\
	__vertexInternal__.m_vVertPosition.w = fW;		/* Store it back */\
\
	for(int32_t ii = 0; ii < __vertexInternal__.m_iVaryingsCount; ii++) \
	{ \
		__vertexInternal__.m_vVertVaryings[ii] /= fW; /* Do the same for all the Varyings */\
	} \
} \

#define RENDER_PIXEL_CALLBACK std::function<void(int32_t x, int32_t y)> funcRenderPixel = [&](int32_t x, int32_t y)

#define GET_EDGE_VARYING_INTERNAL_START(__edge__, __varying__)	__edge__->m_pInternalVaryingsStart[(int32_t)__varying__]
#define GET_EDGE_VARYING_INTERNAL_END(__edge__, __varying__)	__edge__->m_pInternalVaryingsEnd[(int32_t)__varying__]
#define GET_EDGE_VARYING_INTERNAL(__edge__, __varying__)		__edge__->m_pInternalVaryings[(int32_t)__varying__]
#define GET_EDGE_VARYING(__edge__, __varying__)					__edge__->m_pVertexVaryings[(int32_t)__varying__]

#define VERTEX_TO_VERTEXINTERNAL(__vertex__, __vertexInternal__) \
VertexInternal __vertexInternal__; \
__vertexInternal__.m_vVertPosition = __vertex__.m_vPosition; \
memcpy(__vertexInternal__.m_vVertVaryings, &__vertex__, sizeof(Vertex)); \
__vertexInternal__.m_iVaryingsCount = sizeof(Vertex) / sizeof(float); \

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
, m_fHalfWidth(DEFAULT_WIDTH >> 1)
, m_fHalfHeight(DEFAULT_HEIGHT >> 1)
{
	init();
}

Graphics::Graphics(_cookie, uint32_t iWidth, uint32_t iHeight)
: m_iWidth(iWidth)
, m_iHeight(iHeight)
, m_fHalfWidth(iWidth >> 1)
, m_fHalfHeight(iHeight >> 1)
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

void Graphics::drawRectW(int32_t x, int32_t y, int32_t iWidth, int32_t iHeight)
{
	if (x >= 0 && y >= 0 && iWidth > 0 && iHeight > 0)
	{
		int32_t xEnd = x + iWidth;
		int32_t yEnd = y + iHeight;

		for (int32_t xX = x; xX < xEnd; xX++)
		{
			m_pFrameBuffer->setPixel(xX, y,				m_RenderColour);	// Top Line
			m_pFrameBuffer->setPixel(xX, y + iHeight,	m_RenderColour);	// Bottom Line
		}

		for (int32_t yY = y; yY < yEnd; yY++)
		{
			m_pFrameBuffer->setPixel(x,			 yY,	m_RenderColour);	// Left Line
			m_pFrameBuffer->setPixel(x + iWidth, yY,	m_RenderColour);	// Right Line
		}
	}
}

void Graphics::drawRect(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
{
	if (y1 > y2)		std::swap(y1, y2);
	if (x1 > x2)		std::swap(x1, x2);

	drawRectW(x1, y1, (x2 - x1), (y2 - y1));
}

void Graphics::fillRectW(int32_t x, int32_t y, int32_t iWidth, int32_t iHeight)
{
	if (x >= 0 && y >= 0 && iWidth > 0 && iHeight > 0)
	{
		int32_t xEnd = x + iWidth;
		int32_t yEnd = y + iHeight;
		
		for (int32_t yY = y; yY < yEnd; yY++)
		{
			for (int32_t xX = x; xX < xEnd; xX++)
			{
				m_pFrameBuffer->setPixel(xX, yY, m_RenderColour);
			}
		}
	}
}

void Graphics::fillRect(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
{
	if (y1 > y2)		std::swap(y1, y2);
	if (x1 > x2)		std::swap(x1, x2);

	fillRectW(x1, y1, (x2 - x1), (y2 - y1));
}

void Graphics::drawTriangle(Vertex& v1, Vertex& v2, Vertex& v3)
{
	Vector4f v1Pos = v1.m_vPosition;
	Vector4f v2Pos = v2.m_vPosition;
	Vector4f v3Pos = v3.m_vPosition;

	drawLine(v1Pos.x, v1Pos.y, v2Pos.x, v2Pos.y);
	drawLine(v2Pos.x, v2Pos.y, v3Pos.x, v3Pos.y);
	drawLine(v1Pos.x, v1Pos.y, v3Pos.x, v3Pos.y);
}

void Graphics::fillTriangle2D(Vertex& vMin, Vertex& vMid, Vertex& vMax)
{
	VERTEX_TO_VERTEXINTERNAL(vMin, vMinInternal);
	VERTEX_TO_VERTEXINTERNAL(vMid, vMidInternal);
	VERTEX_TO_VERTEXINTERNAL(vMax, vMaxInternal);

	fillTriangleInternal(vMinInternal, vMidInternal, vMaxInternal);
}

void Graphics::fillTriangleInternal(VertexInternal& vMinVert, VertexInternal& vMidVert, VertexInternal& vMaxVert, bool bIsInScreenSpace)
{
	if (!bIsInScreenSpace)
	{
		// Screen Space Transform ==> Transform Homogeneous(-1, 1) to Screen Coordinates(Screen Width, Screen Height)
		m_matScreenSpaceTransform.initScreenSpaceTransform(m_iWidth >> 1, m_iHeight >> 1);
		{
			vMinVert.m_vVertPosition = m_matScreenSpaceTransform.transform(vMinVert.m_vVertPosition);
			vMidVert.m_vVertPosition = m_matScreenSpaceTransform.transform(vMidVert.m_vVertPosition);
			vMaxVert.m_vVertPosition = m_matScreenSpaceTransform.transform(vMaxVert.m_vVertPosition);
		}

		// Perspective Divide
		PERSPECTIVE_DIVIDE(vMinVert)
		PERSPECTIVE_DIVIDE(vMidVert)
		PERSPECTIVE_DIVIDE(vMaxVert)
	}

	// 1. Sort the Vertices w.r.t. 'y'
	if (vMidVert.m_vVertPosition.y < vMinVert.m_vVertPosition.y)	std::swap(vMinVert, vMidVert);
	if (vMaxVert.m_vVertPosition.y < vMidVert.m_vVertPosition.y)	std::swap(vMidVert, vMaxVert);
	if (vMidVert.m_vVertPosition.y < vMinVert.m_vVertPosition.y)	std::swap(vMinVert, vMidVert);

	// Set the 'Edges'
	Edge eTopToBottom;			eTopToBottom.set(vMinVert, vMaxVert);
	Edge eTopToMiddle;			eTopToMiddle.set(vMinVert, vMidVert);
	Edge eMiddleToBottom;		eMiddleToBottom.set(vMidVert, vMaxVert);
	{
		// Check the 'Handedness' of the Triangle.
		bool bHandedness = isRightHandedTriangle(&vMinVert.m_vVertPosition, &vMaxVert.m_vVertPosition, &vMidVert.m_vVertPosition);

		// Scan the 'Edges'
		scanEdges(&eTopToBottom, &eTopToMiddle, bHandedness);
		scanEdges(&eTopToBottom, &eMiddleToBottom, bHandedness);
	}
}

bool Graphics::isRightHandedTriangle(Vector4f* a, Vector4f* b, Vector4f* c)
{
	float x1 = b->x - a->x;
	float y1 = b->y - a->y;

	float x2 = c->x - a->x;
	float y2 = c->y - a->y;

	float fArea = (x1 * y2 - x2  * y1);

	return (fArea >= 0);
}

void Graphics::scanEdges(Edge* eA, Edge* eB, bool bHandedness)
{
	Edge* eLeft = eA;
	Edge* eRight = eB;
	if (bHandedness)
	{
		std::swap(eLeft, eRight);
	}

	int yStart	= GET_EDGE_VARYING_INTERNAL_START(eB, EVaryingsInternal::Y_VARYING);
	int yEnd	= GET_EDGE_VARYING_INTERNAL_END(eB, EVaryingsInternal::Y_VARYING);
	for (int y = yStart; y <= yEnd; y++)						// Increment the value of 'y' by 1 pixel at a time.
	{
		float iLeftX = GET_EDGE_VARYING_INTERNAL(eLeft, EVaryingsInternal::X_VARYING);		// Varying is stepped for each 'y' increment
		float iRightX = GET_EDGE_VARYING_INTERNAL(eRight, EVaryingsInternal::X_VARYING);	// 'x' has the value associated for each 'y' step.

		float fXLerpAmount = 0.0f;
		float fXLerpStep = 1.0f / (float)(iRightX - iLeftX);

		RENDER_PIXEL_CALLBACK
		{
			// Linearly INterpolate the Varyings value from the 'Start Value' to their 'End Value'
			INTERPOLATE_VARYINGS(eLeft, eRight, fXLerpAmount, m_pVaryingsLerpedValues)

			// Here we set the 'Position' og 'gl_Position' for the 'Frag Shader' to access.
			OpenGL::m_pCurShader->gl_Position = Vector4f(x, y, 0, 0);

			// Set the Interpolated Varyings for the 'Frag Shader' to access.
			OpenGL::m_pCurShader->preFragment(m_pVaryingsLerpedValues);

			// Call the 'Frag Shader'
			if(FRAG_SHADER)
			{	
				m_pFrameBuffer->setPixel(x, y, OpenGL::m_pCurShader->gl_Colour);
			}

			fXLerpAmount += fXLerpStep;
		};

		for (int32_t x = iLeftX; x <= iRightX; x++)
		{
			funcRenderPixel(x, y);
		}

		// DO NOT 'Step' for the last line.
		if (y < yEnd)
		{
			eLeft->step();
			eRight->step();
		}
	}
}