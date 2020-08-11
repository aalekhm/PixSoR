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
	protected:
	private:
		void									init();

		std::unique_ptr<Sprite>					m_pFrameBuffer;
		std::unique_ptr<Sprite>					m_pClearFrameBuffer;

		uint32_t								m_iMaxBufferSize;
		GLuint									m_glFrameBufferID;
		uint32_t								m_iWidth;
		uint32_t								m_iHeight;

		Pixel									m_ClearColour;
};