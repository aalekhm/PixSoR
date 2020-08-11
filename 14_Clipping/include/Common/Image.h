#pragma once

#include "Common/TGA.h"
#include <memory>

enum Format
{
	RGB,
	RGBA
};

class Image
{
	private:
		struct _cookie {};
	public:
										~Image();
										Image(_cookie);
										Image&	operator=(const Image&) = default;
		static std::unique_ptr<Image>	createImage(const char* sTexWithPath);

		unsigned char*					getPixelData() const;
		Format							getFormat() const;
		unsigned int					getWidth() const;
		unsigned int					getHeight() const;

		unsigned int					m_iWidth;
		unsigned int					m_iHeight;
		unsigned char*					m_pPixelData;
		Format							m_Format;
	protected:
	private:
};

