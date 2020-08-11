#pragma once
#include <stdint.h>
#include "Common/Pixel.h"
#include <memory>
#include "Renderer/Edge.h"
#include <unordered_map>
#include "Common/Defines.h"
#include "Common/Pixel.h"
#include "Renderer/Edge.h"

#define GL_ARRAY_BUFFER 0

class Graphics;
class IShader;
class Sprite;
class OpenGL
{
	public:
		static void							glBindBuffer(GLenum target, GLuint buffer);
		static void							glBufferData(const void* pointer, uint32_t iSize, uint32_t iStride);

		static void							glBindTexture(GLenum target, GLuint buffer);
		static void							glTexImage2D(GLenum target, std::unique_ptr<Sprite>& pTexture);
		static Pixel						sample2D(float fU, float fV);

		static void							glDrawArrays(GLenum eMode, GLint iDrawIndex, GLsizei iDrawCount);
		static void							useProgram(IShader* pShader);

		static void							glGenBuffers(GLsizei n, GLuint* buffers);
		static void							glDeleteBuffers(GLsizei n, GLuint* buffers);

		static void							glGenTextures(GLsizei n, GLuint* buffers);
		static void							glDeleteTextures(GLsizei n, GLuint* buffers);

		static IShader*						m_pCurShader;
		static Graphics*					m_pGraphics;
	protected:
	private:
		static void							clipAndFillTriangle(	VertexInternal& v1, 
																	VertexInternal& v2, 
																	VertexInternal& v3);
		static bool							clipPolygonAxis(	VertexInternal& vInside, 
																VertexInternal& vOutside, 
																EAXIS_COMPONENT eComponentIndex, 
																VertexInternal& vModified);
		static bool							clipPolygonComponent(	VertexInternal& vInside, 
																	VertexInternal& vOutside, 
																	EAXIS_COMPONENT eComponentIndex, 
																	float fComponentFactor, 
																	VertexInternal& vModified);

		// Each drawing triangle will re-use these
		static VertexInternal				m_sMinVert;
		static VertexInternal				m_sMidVert;
		static VertexInternal				m_sMaxVert;

		static uint32_t						m_iBufferCount;
		static uint32_t						m_iTextureCount;

		static uint32_t						m_iCurrentBufferTarget;
		static Sprite*						m_pCurrentTexture;

		static uint32_t						m_iCurrentBuffer;
		static uint32_t						m_iCurrentTexture;

		static std::unordered_map<uint32_t, BufferDetails>	m_ArrayBufferMap;
		static uint32_t										m_iVaryingsCount;

		static std::unique_ptr<Sprite>		m_pTextures[MAX_TEXTURES];
};