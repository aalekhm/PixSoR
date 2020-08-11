#pragma once
#include <memory>
#include "Common/Pixel.h"
#include "Common/Sprite.h"
#include "Common/Defines.h"
#include <functional>

class Graphics
{
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
protected:
	private:
		void									init();

		void									drawDDALine(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
		void									drawBresenhamLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2);

		std::unique_ptr<Sprite>					m_pFrameBuffer;
		std::unique_ptr<Sprite>					m_pClearFrameBuffer;

		uint32_t								m_iMaxBufferSize;
		GLuint									m_glFrameBufferID;
		uint32_t								m_iWidth;
		uint32_t								m_iHeight;

		Pixel									m_ClearColour;
		Pixel									m_RenderColour;
};