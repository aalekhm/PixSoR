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
std::unique_ptr<Camera>						OpenGL::m_pCamera = nullptr;
std::unique_ptr<Light>						OpenGL::m_pLightSource = nullptr;

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
						clipAndFillTriangle(m_sMinVert, m_sMidVert, m_sMaxVert);
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

void OpenGL::clipAndFillTriangle(	VertexInternal& v1, 
									VertexInternal& v2, 
									VertexInternal& v3)
{
	int32_t iVertexClipStatus = ECLIP_STATUS::DISCARD;

	iVertexClipStatus |= v1.isInsideViewFrustum() ? (1 << 0) : 0;
	iVertexClipStatus |= v2.isInsideViewFrustum() ? (1 << 1) : 0;
	iVertexClipStatus |= v3.isInsideViewFrustum() ? (1 << 2) : 0;

	switch (iVertexClipStatus)
	{
		case 0:		// All(3) Vertices outside the Frustum
		{
			return;
		}
		case 7:		// All(3) Vertices inside the Frustum
		{
			m_pGraphics->fillTriangleInternal(v1, v2, v3);
			return;
		}
		case 1:		// 1 Vertice inside the Frustum
		case 2:
		case 4:
		{
			VertexInternal					vInsider;
			std::vector<VertexInternal>		vOutsiders;
			std::vector<VertexInternal>		vResult;
			{
				vResult.push_back(VertexInternal());
				vResult.push_back(VertexInternal());
			}

			if (iVertexClipStatus == 1)
			{
				vInsider = v1;
				vOutsiders.push_back(v2);
				vOutsiders.push_back(v3);
			}
			else
			if (iVertexClipStatus == 2)
			{
				vOutsiders.push_back(v1);
				vInsider = v2;
				vOutsiders.push_back(v3);
			}
			else
			if (iVertexClipStatus == 4)
			{
				vOutsiders.push_back(v1);
				vOutsiders.push_back(v2);
				vInsider = v3;
			}

			for (int32_t i = 0; i < 2; i++)
			{
				clipPolygonAxis(vInsider, vOutsiders[i], EAXIS_COMPONENT::AXIS_X, vResult[i]);
				clipPolygonAxis(vInsider, vOutsiders[i], EAXIS_COMPONENT::AXIS_Y, vResult[i]);
				clipPolygonAxis(vInsider, vOutsiders[i], EAXIS_COMPONENT::AXIS_Z, vResult[i]);
			}

			m_pGraphics->fillTriangleInternal(	VertexInternal(vInsider), 
												VertexInternal(vResult[0]),
												VertexInternal(vResult[1]));
			return;
		}
		case 3:		// 2 Vertices inside the Frustum
		case 5:
		case 6:
		{
			std::vector<VertexInternal>		vInsiders;
			VertexInternal					vOutsider;
			std::vector<VertexInternal>		vResult;
			{
				vResult.push_back(VertexInternal());
				vResult.push_back(VertexInternal());
			}

			if (iVertexClipStatus == 3)
			{
				vInsiders.push_back(v1);
				vInsiders.push_back(v2);
				vOutsider = v3;
			}
			else
			if (iVertexClipStatus == 5)
			{
				vInsiders.push_back(v1);
				vOutsider = v2;
				vInsiders.push_back(v3);
			}
			else
			if (iVertexClipStatus == 6)
			{
				vOutsider = v1;
				vInsiders.push_back(v2);
				vInsiders.push_back(v3);
			}

			for (int32_t i = 0; i < 2; i++)
			{
				clipPolygonAxis(vInsiders[i], vOutsider, EAXIS_COMPONENT::AXIS_X, vResult[i]);
				clipPolygonAxis(vInsiders[i], vOutsider, EAXIS_COMPONENT::AXIS_Y, vResult[i]);
				clipPolygonAxis(vInsiders[i], vOutsider, EAXIS_COMPONENT::AXIS_Z, vResult[i]);
			}

			m_pGraphics->fillTriangleInternal(	VertexInternal(vInsiders[0]), 
												VertexInternal(vResult[0]),
												VertexInternal(vInsiders[1]));

			m_pGraphics->fillTriangleInternal(	VertexInternal(vInsiders[1]), 
												VertexInternal(vResult[0]),
												VertexInternal(vResult[1]));
			return;
		}
	}
}

bool OpenGL::clipPolygonAxis(	VertexInternal& vInside, 
								VertexInternal& vOutside, 
								EAXIS_COMPONENT eComponentIndex, 
								VertexInternal& vModified)
{
	if (!clipPolygonComponent(vInside, vOutside, eComponentIndex, 1.0f, vModified))
	{
		return clipPolygonComponent(vInside, vOutside, eComponentIndex, -1.0f, vModified);
	}

	return false;
}

// Youtube: thebennybox : https://www.youtube.com/watch?v=VMD7fsCYO9o&list=PLEETnX-uPtBUbVOok816vTl1K9vV1GgH5&index=21&t=0s
/*
* 		|-------------------------------|
* 		|								|
* 		|								|
* 		|								|
* 		|						 v1.	|
* 		|						   |\	|
* 		|						   | \	|
* 		|						   |  \	|
* 		|						   |   \|
* 	-1	|..............0................\-----------> point of intersection on +ve X-Axis
* 		|						   |	|\.v2
*  		|						   |	|/
*  		|					       |	/-----------> point of intersection on +ve X-Axis
*  		|						   |   /|
*  		|						   v3./	|
*  		|                          | /  |
*		|						   ./   |
*  		|								|
*  		|-------------------------------|
*  clipPolygonComponent calculates the exact intersection point on the desired axis
*  & adds it to the results list of vertices.
*/

bool OpenGL::clipPolygonComponent(	VertexInternal& vInside,
									VertexInternal& vOutside,
									EAXIS_COMPONENT eComponentIndex,
									float fComponentFactor,
									VertexInternal& vModified)
{
	float fOutsideComponent = vOutside.get(eComponentIndex) * fComponentFactor;
	bool bInside = (fOutsideComponent <= vOutside.m_vVertPosition.w);

	if (bInside)
	{
		return false;
	}
	else
	{
		float fInsideComponent = vInside.get(eComponentIndex) * fComponentFactor;

		/*
		* 		-1			A       B   +1    C
		* 		 |..........*.......*....|....*
		*                               D
		*
		*  	So, we have to lerp from B->C so thet the result is +1.
		* 			1 		= B(1 - LerpAmt) + C * LerpAmt;
		* 			1 		= B - B*LerpAmt + C*LerpAmt + (LerpAmt - LerpAmt);
		* 			1 - B 	= LerpAmt - LerpAmt*B - LerpAmt + C*LerpAmt;
		* 			1 - B 	= LerpAmt(1 - B) - LerpAmt(1 + C);
		* 			1 - B	= LerpAmt((1 - B) - (1 + C));
		* 			LerpAmt	= (1 - B) / ((1 - B) - (1 + C))
		*
		* 		Now, Divide by ZERO from perspective ?
		* 		Simple solution, clip before perspective divide !
		* 			-1 <= 	  Xp    <= 1
		* 			-1 <=  Xp / W	<= 1
		* 			-W <= 	  Xp	<= W
		* 		,So clipping is against W & -W, NOT 1 & -1, reason being we are clipping before perspective divide.
		* 		,hence our LerpAmt equation now becomes :
		* 			LerpAmt	= (Wb - B) / ((Wb - B) - (Wc - C));
		*/
		float fLerpAmount = (vInside.m_vVertPosition.w - fInsideComponent)
							/
							(	(vInside.m_vVertPosition.w - fInsideComponent)
								-
								(vOutside.m_vVertPosition.w - fOutsideComponent)
							);

		vModified = vInside.lerp(vOutside, fLerpAmount);

		return true;
	}
}
