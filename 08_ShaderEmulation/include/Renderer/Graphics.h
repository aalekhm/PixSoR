#pragma once
#include <memory>
#include "Common/Pixel.h"
#include "Common/Sprite.h"
#include "Common/Defines.h"
#include <functional>
#include "3D/Vertex.h"

class Edge;
class Graphics
{
	friend class OpenGL;
	private:
		struct _cookie {};
	public:
												Graphics(_cookie);
												Graphics(_cookie, uint32_t iWidth, uint32_t iHeight);

		static std::unique_ptr<Graphics>		create(uint32_t iWidth, uint32_t iHeight);

		void									paint(std::function<void(Graphics*)>& pRenderFunc);

		void									clear();
		void									setPixel(int32_t iX, int32_t iY, Pixel pPixel);
		void									setClearColour(Pixel pPixel);

		void									setColour(Pixel pPixel);
		void									drawLine(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2);

		void									drawRectW(int32_t x, int32_t y, int32_t iWidth, int32_t iHeight);
		void									drawRect(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
		void									fillRectW(int32_t x, int32_t y, int32_t iWidth, int32_t iHeight);
		void									fillRect(int32_t x1, int32_t y1, int32_t x2, int32_t y2);

		void									drawTriangle(Vertex& v1, Vertex& v2, Vertex& v3);
		void									fillTriangle2D(Vertex& vMin, Vertex& vMid, Vertex& vMax);
protected:
	private:
		void									init();

		void									drawDDALine(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
		void									drawBresenhamLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2);

		void									fillTriangleInternal(VertexInternal& vMinVert, VertexInternal& vMidVert, VertexInternal& vMaxVert, bool bIsInScreenSpace = false);
		bool									isRightHandedTriangle(Vector4f* a, Vector4f* b, Vector4f* c);
		void									scanEdges(Edge* eA, Edge* eB, bool bHandedness);

		std::unique_ptr<Sprite>					m_pFrameBuffer;
		std::unique_ptr<Sprite>					m_pClearFrameBuffer;

		uint32_t								m_iMaxBufferSize;
		GLuint									m_glFrameBufferID;
		uint32_t								m_iWidth;
		uint32_t								m_iHeight;
		float									m_fHalfWidth;
		float									m_fHalfHeight;

		Pixel									m_ClearColour;
		Pixel									m_RenderColour;

		float									m_pVaryingsLerpedValues[MAX_VERTEX_VARYINGS];	// For Caching
};