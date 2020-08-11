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
uint32_t									OpenGL::m_iCurrentBufferTarget;
uint32_t									OpenGL::m_iCurrentBuffer;

std::unordered_map<uint32_t, BufferDetails>	OpenGL::m_ArrayBufferMap;
uint32_t									OpenGL::m_iVaryingsCount;

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

void OpenGL::glVertexAttribPointer(uint32_t index, uint32_t size, uint32_t type, uint32_t stride, const void* pointer)
{
}

void OpenGL::glDrawArrays(GLenum eMode, GLint iDrawIndex, GLsizei iDrawCount)
{
	switch (m_iCurrentBufferTarget)
	{
		case GL_ARRAY_BUFFER:
		{
			BufferDetails& bufferDetails = m_ArrayBufferMap[m_iCurrentBuffer];
			float* pVertexBuffer = ((float*)bufferDetails.m_pBufferPointer) + iDrawIndex;

			if (eMode == GL_TRIANGLES)
			{
				for (int32_t i = iDrawIndex; i < iDrawCount; i += 3)
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
