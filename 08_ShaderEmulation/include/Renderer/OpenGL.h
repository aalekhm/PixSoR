#pragma once
#include <stdint.h>
#include "Common/Pixel.h"
#include <memory>
#include "Renderer/Edge.h"
#include <unordered_map>
#include "Common/Defines.h"

#define GL_ARRAY_BUFFER 0

class Graphics;
class IShader;
class Sprite;
class OpenGL
{
	public:
		static void							glBindBuffer(GLenum target, GLuint buffer);
		static void							glBufferData(const void* pointer, uint32_t iSize, uint32_t iStride);

		static void							glVertexAttribPointer(uint32_t index, uint32_t size, uint32_t type, uint32_t stride, const void* pointer);
		static void							glDrawArrays(GLenum eMode, GLint iDrawIndex, GLsizei iDrawCount);
		static void							useProgram(IShader* pShader);

		static void							glGenBuffers(GLsizei n, GLuint* buffers);
		static void							glDeleteBuffers(GLsizei n, GLuint* buffers);

		static IShader*						m_pCurShader;
		static Graphics*					m_pGraphics;
	protected:
	private:
		// Each drawing triangle will re-use these
		static VertexInternal				m_sMinVert;
		static VertexInternal				m_sMidVert;
		static VertexInternal				m_sMaxVert;

		static uint32_t						m_iBufferCount;
		static uint32_t						m_iCurrentBufferTarget;
		static uint32_t						m_iCurrentBuffer;

		static std::unordered_map<uint32_t, BufferDetails>	m_ArrayBufferMap;
		static uint32_t										m_iVaryingsCount;
};