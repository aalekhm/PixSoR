#include "Renderer/OpenGL.h"
#include "Renderer/Edge.h"
#include "Renderer/Graphics.h"
#include <stdlib.h>
#include "Renderer/IShader.h"
#include "Common/Sprite.h"
#include <bitset>
#include "3D/Vertex.h"

#define VERTEX_SHADER(__VertexBuffer__, __VertexBufferStride__, __VertexInternal__) \
{ \
	m_pCurShader->preVertex(__VertexBuffer__, __VertexInternal__.m_vVertVaryings); \
	m_pCurShader->vertex(); \
	__VertexInternal__.m_iVaryingsCount = m_pCurShader->m_iVaryingsByteSize; \
	__VertexInternal__.m_vVertPosition = m_pCurShader->gl_Position; \
	__VertexBuffer__ += __VertexBufferStride__; \
} \

IShader*									OpenGL::m_pCurShader;
Graphics*									OpenGL::m_pGraphics;

VertexInternal								OpenGL::m_sMinVert;
VertexInternal								OpenGL::m_sMidVert;
VertexInternal								OpenGL::m_sMaxVert;

uint32_t									OpenGL::m_iBufferCount;
uint32_t									OpenGL::m_iTextureCount;

uint32_t									OpenGL::m_iCurrentBufferTarget;
Sprite*										OpenGL::m_pCurrentTexture;

uint32_t									OpenGL::m_iCurrentBuffer;
uint32_t									OpenGL::m_iCurrentTexture;

std::unordered_map<uint32_t, BufferDetails>	OpenGL::m_ArrayBufferMap;
uint32_t									OpenGL::m_iVaryingsCount;
std::unique_ptr<Sprite>						OpenGL::m_pTextures[MAX_TEXTURES];

void OpenGL::glBindBuffer(GLenum target, GLuint buffer)
{
	m_iCurrentBufferTarget = target;
	m_iCurrentBuffer = buffer;
}

void OpenGL::glBufferData(const void* pointer, uint32_t iSize, uint32_t iStride)
{
	switch (m_iCurrentBufferTarget)
	{
		case GL_ARRAY_BUFFER:
		{
			BufferDetails buff;
			buff.m_pBufferPointer = (void*)(std::malloc(iSize));
			memcpy_s(buff.m_pBufferPointer, iSize, pointer, iSize);

			buff.m_iSize = iSize;
			buff.m_iStride = iStride;

			m_ArrayBufferMap[m_iCurrentBuffer] = buff;
		}
		break;
	}
}

void OpenGL::glBindTexture(GLenum target, GLuint buffer)
{
	m_iCurrentTexture = buffer;
	m_pCurrentTexture = m_pTextures[m_iCurrentTexture].get();
}

void OpenGL::glTexImage2D(GLenum target, std::unique_ptr<Sprite>& pTexture)
{
	m_pTextures[m_iCurrentTexture] = std::move(pTexture);
	pTexture = nullptr;
}

Pixel OpenGL::sample2D(float fU, float fV)
{
	int32_t tX = (fU * (m_pCurrentTexture->m_iWidth - 1) + 0.5f);
	int32_t tY = (fV * (m_pCurrentTexture->m_iHeight - 1) + 0.5f);

	if (tX >= 0 && tX < m_pCurrentTexture->m_iWidth && tY >= 0 && tY < m_pCurrentTexture->m_iHeight)
	{
		return m_pCurrentTexture->m_pColourData[ tY * m_pCurrentTexture->m_iWidth + tX];
	}
	else
	{
		return Pixel(0, 0, 0, 0);
	}
}

void OpenGL::glDrawArrays(GLenum eMode, GLint iDrawIndex, GLsizei iDrawCount)
{
	switch (m_iCurrentBufferTarget)
	{
		case GL_ARRAY_BUFFER:
		{
			BufferDetails& bufferDetails = m_ArrayBufferMap[m_iCurrentBuffer];
			float* pVertexBuffer = ((float*)bufferDetails.m_pBufferPointer) + (iDrawIndex * bufferDetails.m_iStride);

			if (eMode == GL_TRIANGLES)
			{
				int32_t iMaxDraws = (iDrawIndex + iDrawCount);
				for (int32_t i = iDrawIndex; i < iMaxDraws; i += 3)
				{
					// Call 'Vertex SHader' for each Vertex.
					VERTEX_SHADER(pVertexBuffer, bufferDetails.m_iStride, m_sMinVert)
					VERTEX_SHADER(pVertexBuffer, bufferDetails.m_iStride, m_sMidVert)
					VERTEX_SHADER(pVertexBuffer, bufferDetails.m_iStride, m_sMaxVert)
					{
						// All 'Vertices' by now should be 'World - View - Projected'
						m_pGraphics->fillTriangleInternal(m_sMinVert, m_sMidVert, m_sMaxVert);
					}
				}
			}
		}
		break;
	}
}

void OpenGL::useProgram(IShader* pShader)
{
	m_pCurShader = pShader;
}

void OpenGL::glGenBuffers(GLsizei n, GLuint* buffers)
{
	for (int32_t i = 0; i < n; i++)
	{
		buffers[i] = m_iBufferCount++;
	}
}

void OpenGL::glDeleteBuffers(GLsizei n, GLuint* buffers)
{
	for (int32_t i = 0; i < n; i++)
	{
		int32_t iBuffer = buffers[i];
		BufferDetails& bufferDetails = m_ArrayBufferMap[iBuffer];
		std::free(bufferDetails.m_pBufferPointer);

		m_ArrayBufferMap.erase(iBuffer);
	}
}

void OpenGL::glGenTextures(GLsizei n, GLuint* buffers)
{
	for (int32_t i = 0; i < n; i++)
	{
		buffers[i] = m_iTextureCount++;
	}
}

void OpenGL::glDeleteTextures(GLsizei n, GLuint* buffers)
{
	for (int32_t i = 0; i < n; i++)
	{
		int32_t iBuffer = buffers[i];
		if (m_pTextures[iBuffer] != nullptr)
		{
			m_pTextures[iBuffer].release();
		}
	}
}
